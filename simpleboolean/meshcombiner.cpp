#include <queue>
#include <map>
#include <set>
#include <simpleboolean/meshcombiner.h>
#include <simpleboolean/retriangulation.h>
#include <simpleboolean/edgeloop.h>
#include <simpleboolean/subsurface.h>
#include <simpleboolean/subblock.h>
#include <thirdparty/moller97/tritri_isectline.h>

namespace simpleboolean
{

size_t MeshCombiner::m_maxOctreeDepth = 10;
size_t MeshCombiner::m_minIntersectsInOctant = 2;
int MeshCombiner::m_vertexToKeyMultiplyFactor = 1000;

void MeshCombiner::setMeshes(const Mesh &first, const Mesh &second)
{
    m_firstMesh = first;
    m_secondMesh = second;
    m_firstMeshFaceAABBs.resize(m_firstMesh.faces.size());
    m_secondMeshFaceAABBs.resize(m_secondMesh.faces.size());
    for (size_t i = 0; i < m_firstMesh.faces.size(); ++i) {
        addFaceToAxisAlignedBoundingBox(m_firstMesh, m_firstMesh.faces[i], m_firstMeshFaceAABBs[i]);
    }
    for (size_t i = 0; i < m_secondMesh.faces.size(); ++i) {
        addFaceToAxisAlignedBoundingBox(m_secondMesh, m_secondMesh.faces[i], m_secondMeshFaceAABBs[i]);
    }
}

void MeshCombiner::addMeshToAxisAlignedBoundingBox(const Mesh &mesh, AxisAlignedBoudingBox &box)
{
    for (const auto &vertex: mesh.vertices) {
        box.update(vertex);
    }
}

void MeshCombiner::addFaceToAxisAlignedBoundingBox(const Mesh &mesh, const Face &face, AxisAlignedBoudingBox &box)
{
    for (size_t i = 0; i < 3; ++i) {
        box.update(mesh.vertices[face.indices[i]]);
    }
}

void MeshCombiner::searchPotentialIntersectedPairs(std::vector<std::pair<size_t, size_t>> &pairs)
{
    AxisAlignedBoudingBox firstBox;
    AxisAlignedBoudingBox secondBox;
    AxisAlignedBoudingBox intersectedBox;
    addMeshToAxisAlignedBoundingBox(m_firstMesh, firstBox);
    addMeshToAxisAlignedBoundingBox(m_secondMesh, secondBox);
    firstBox.intersectWith(secondBox, &intersectedBox);
    std::vector<size_t> firstGroupOfFacesIn;
    std::vector<size_t> secondGroupOfFacesIn;
    for (size_t i = 0; i < m_firstMeshFaceAABBs.size(); ++i) {
        if (intersectedBox.intersectWith(m_firstMeshFaceAABBs[i])) {
            firstGroupOfFacesIn.push_back(i);
        }
    }
    for (size_t i = 0; i < m_secondMeshFaceAABBs.size(); ++i) {
        if (intersectedBox.intersectWith(m_secondMeshFaceAABBs[i])) {
            secondGroupOfFacesIn.push_back(i);
        }
    }
    for (const auto &i: firstGroupOfFacesIn) {
        addFaceToAxisAlignedBoundingBox(m_firstMesh, m_firstMesh.faces[i], intersectedBox);
    }
    for (const auto &i: secondGroupOfFacesIn) {
        addFaceToAxisAlignedBoundingBox(m_secondMesh, m_secondMesh.faces[i], intersectedBox);
    }
    std::queue<std::pair<size_t, AxisAlignedBoudingBox>> octants;
    octants.push({1, intersectedBox});
    std::set<std::pair<size_t, size_t>> histories;
    while (!octants.empty()) {
        auto item = octants.front();
        octants.pop();
        std::vector<size_t> firstGroupCandidates;
        std::vector<size_t> secondGroupCandidates;
        for (const auto &i: firstGroupOfFacesIn) {
            if (item.second.intersectWith(m_firstMeshFaceAABBs[i]))
                firstGroupCandidates.push_back(i);
        }
        for (const auto &i: secondGroupOfFacesIn) {
            if (item.second.intersectWith(m_secondMeshFaceAABBs[i]))
                secondGroupCandidates.push_back(i);
        }
        if (0 == firstGroupCandidates.size() || 0 == secondGroupCandidates.size())
            continue;
        if ((firstGroupCandidates.size() < MeshCombiner::m_minIntersectsInOctant &&
                    secondGroupCandidates.size() < MeshCombiner::m_minIntersectsInOctant) ||
                item.first >= MeshCombiner::m_maxOctreeDepth) {
            for (const auto &i: firstGroupCandidates) {
                for (const auto &j: secondGroupCandidates) {
                    std::pair<size_t, size_t> candidate = {i, j};
                    if (histories.find(candidate) != histories.end())
                        continue;
                    histories.insert(candidate);
                    pairs.push_back(candidate);
                }
            }
            continue;
        }
        std::vector<AxisAlignedBoudingBox> children;
        item.second.makeOctree(children);
        for (const auto &child: children) {
            octants.push({item.first + 1, child});
        }
    }
}

bool MeshCombiner::intersectTwoFaces(size_t firstIndex, size_t secondIndex, std::pair<Vertex, Vertex> &newEdge)
{
    const auto &firstFace = m_firstMesh.faces[firstIndex];
    const auto &secondFace = m_secondMesh.faces[secondIndex];
    int coplanar = 0;
    if (!tri_tri_intersect_with_isectline(m_firstMesh.vertices[firstFace.indices[0]].xyz,
            m_firstMesh.vertices[firstFace.indices[1]].xyz,
            m_firstMesh.vertices[firstFace.indices[2]].xyz,
            m_secondMesh.vertices[secondFace.indices[0]].xyz,
            m_secondMesh.vertices[secondFace.indices[1]].xyz,
            m_secondMesh.vertices[secondFace.indices[2]].xyz,
            &coplanar,
            newEdge.first.xyz,
            newEdge.second.xyz)) {
        return false;
    }
    if (coplanar)
        return false;
    return true;
}

std::tuple<int, int, int> MeshCombiner::vertexToKey(const Vertex &vertex)
{
    return {vertex.xyz[0] * MeshCombiner::m_vertexToKeyMultiplyFactor,
        vertex.xyz[1] * MeshCombiner::m_vertexToKeyMultiplyFactor,
        vertex.xyz[2] * MeshCombiner::m_vertexToKeyMultiplyFactor
    };
}

size_t MeshCombiner::newVertexToIndex(const Vertex &vertex)
{
    const auto &key = vertexToKey(vertex);
    const auto &findResult = m_newVertexToIndexMap.find(key);
    if (findResult != m_newVertexToIndexMap.end()) {
        return findResult->second;
    }
    size_t newIndex = m_newVertices.size();
    m_newVertexToIndexMap.insert({key, newIndex});
    m_newVertices.push_back(vertex);
    return newIndex;
}

void MeshCombiner::groupEdgesToLoops(const std::vector<std::pair<size_t, size_t>> &edges,
        std::vector<std::vector<size_t>> &edgeLoops)
{
    std::map<size_t, std::vector<size_t>> linkMap;
    for (const auto &pair: edges) {
        linkMap[pair.first].push_back(pair.second);
        linkMap[pair.second].push_back(pair.first);
    }
    std::vector<std::pair<size_t, size_t>> linkMapKeys;
    for (const auto &it: linkMap) {
        linkMapKeys.push_back({it.first, it.second.size()});
    }
    // This sort would make sure the endpoints sit in the front, it's very important for the following steps of finding open edge loop
    std::sort(linkMapKeys.begin(), linkMapKeys.end(), [](const std::pair<size_t, size_t> &first,
            const std::pair<size_t, size_t> &second) {
        return first.second < second.second;
    });
    std::set<size_t> visited;
    for (const auto &key: linkMapKeys) {
        if (visited.find(key.first) != visited.end())
            continue;
        std::vector<size_t> loop;
        size_t head = key.first;
        visited.insert(head);
        size_t next = head;
        bool continueNext = true;
        while (continueNext) {
            loop.push_back(next);
            const auto &findResult = linkMap.find(next);
            if (findResult == linkMap.end())
                break;
            continueNext = false;
            for (const auto &branch: findResult->second) {
                if (visited.find(branch) != visited.end()) {
                    if (branch == head) {
                        loop.push_back(branch);
                        break;
                    }
                    continue;
                }
                next = branch;
                visited.insert(next);
                continueNext = true;
                break;
            }
        }
        for (const auto &it: visited) {
            linkMap.erase(it);
        }
        if (loop.size() <= 1)
            continue;
        edgeLoops.push_back(loop);
    }
}

void MeshCombiner::combine(Operation operation)
{
    std::vector<std::pair<size_t, size_t>> potentailPairs;
    searchPotentialIntersectedPairs(potentailPairs);
    std::map<size_t, std::vector<std::pair<size_t, size_t>>> newEdgesPerTriangleInFirstMesh;
    std::map<size_t, std::vector<std::pair<size_t, size_t>>> newEdgesPerTriangleInSecondMesh;
    std::set<size_t> reTriangulatedFacesInFirstMesh;
    std::set<size_t> reTriangulatedFacesInSecondMesh;
    for (const auto &pair: potentailPairs) {
        std::pair<Vertex, Vertex> newEdge;
        if (intersectTwoFaces(pair.first, pair.second, newEdge)) {
            std::pair<size_t, size_t> newVertexPair = {
                newVertexToIndex(newEdge.first),
                newVertexToIndex(newEdge.second)
            };
            newEdgesPerTriangleInFirstMesh[pair.first].push_back(newVertexPair);
            newEdgesPerTriangleInSecondMesh[pair.second].push_back(newVertexPair);
            reTriangulatedFacesInFirstMesh.insert(pair.first);
            reTriangulatedFacesInSecondMesh.insert(pair.second);
        }
    }
    auto doReTriangulation = [&](const Mesh *mesh, const std::map<size_t, std::vector<std::pair<size_t, size_t>>> &newEdgesPerTriangle, std::vector<Face> &toTriangles, std::vector<std::vector<size_t>> &edgeLoops) {
        for (const auto &it: newEdgesPerTriangle) {
            const auto &face = mesh->faces[it.first];
            const std::vector<std::pair<size_t, size_t>> &newEdges = it.second;
            std::vector<std::vector<size_t>> edgeLoopsPerFace;
            groupEdgesToLoops(newEdges, edgeLoopsPerFace);
            std::vector<size_t> triangleVertices = {
                newVertexToIndex(mesh->vertices[face.indices[0]]),
                newVertexToIndex(mesh->vertices[face.indices[1]]),
                newVertexToIndex(mesh->vertices[face.indices[2]]),
            };
            std::vector<Face> reTriangulatedTriangles;
            ReTriangulation re;
            re.reTriangulate(m_newVertices, triangleVertices, edgeLoopsPerFace, reTriangulatedTriangles);
            for (const auto &loop: edgeLoopsPerFace) {
                edgeLoops.push_back(loop);
            }
            for (const auto &triangle: reTriangulatedTriangles) {
                toTriangles.push_back(triangle);
            }
        }
    };
    auto addUnIntersectedFaces = [&](const Mesh *mesh, const std::set<size_t> &reTriangulatedFaces,
            std::vector<Face> &toTriangles) {
        for (size_t i = 0; i < mesh->faces.size(); ++i) {
            if (reTriangulatedFaces.find(i) != reTriangulatedFaces.end())
                continue;
            const auto &face = mesh->faces[i];
            Face triangle = {{
                newVertexToIndex(mesh->vertices[face.indices[0]]),
                newVertexToIndex(mesh->vertices[face.indices[1]]),
                newVertexToIndex(mesh->vertices[face.indices[2]]),
            }};
            toTriangles.push_back(triangle);
        }
    };
    std::vector<SubSurface> firstSubSurfaces;
    std::vector<SubSurface> secondSubSurfaces;
    {
        std::vector<std::vector<size_t>> edgeLoops;
        std::vector<Face> triangles;
        doReTriangulation(&m_firstMesh, newEdgesPerTriangleInFirstMesh, triangles, edgeLoops);
        addUnIntersectedFaces(&m_firstMesh, reTriangulatedFacesInFirstMesh, triangles);
        EdgeLoop::merge(edgeLoops);
        SubSurface::createSubSurfaces(edgeLoops, triangles, firstSubSurfaces);
    }
    {
        std::vector<std::vector<size_t>> edgeLoops;
        std::vector<Face> triangles;
        doReTriangulation(&m_secondMesh, newEdgesPerTriangleInSecondMesh, triangles, edgeLoops);
        addUnIntersectedFaces(&m_secondMesh, reTriangulatedFacesInSecondMesh, triangles);
        EdgeLoop::merge(edgeLoops);
        SubSurface::createSubSurfaces(edgeLoops, triangles, secondSubSurfaces);
    }
    std::vector<SubBlock> subBlocks;
    SubBlock::createSubBlocks(firstSubSurfaces, secondSubSurfaces, subBlocks);
    std::vector<Operation> operations;
    Distinguish::distinguish(subBlocks, operations);
    for (size_t i = 0; i < operations.size(); ++i) {
        if (operations[i] == operation) {
            subBlockToMesh(subBlocks[i], m_resultMesh);
            break;
        }
    }
}

void MeshCombiner::subBlockToMesh(const SubBlock &subBlock, Mesh &mesh)
{
    // TODO:
}

const Mesh &MeshCombiner::getResult()
{
    return m_resultMesh;
}

}

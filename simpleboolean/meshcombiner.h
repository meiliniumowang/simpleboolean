#ifndef SIMPLEBOOLEAN_MESH_COMBINER_H
#define SIMPLEBOOLEAN_MESH_COMBINER_H
#include <vector>
#include <map>
#include <simpleboolean/meshdatatype.h>
#include <simpleboolean/axisalignedboundingbox.h>
#include <simpleboolean/distinguish.h>

namespace simpleboolean 
{

class MeshCombiner
{
public:
    void setMeshes(const Mesh &first, const Mesh &second);
    void combine(Operation operation);
    const Mesh &getResult();
    
    // Debug only
    Mesh m_debugFirstMeshReTriangulated;
    
private:
    void searchPotentialIntersectedPairs(std::vector<std::pair<size_t, size_t>> &pairs);
    bool intersectTwoFaces(size_t firstIndex, size_t secondIndex, std::pair<Vertex, Vertex> &newEdge);
    void addMeshToAxisAlignedBoundingBox(const Mesh &mesh, AxisAlignedBoudingBox &box);
    void addFaceToAxisAlignedBoundingBox(const Mesh &mesh, const Face &face, AxisAlignedBoudingBox &box);
    std::tuple<int, int, int> vertexToKey(const Vertex &vertex);
    size_t newVertexToIndex(const Vertex &vertex);
    void groupEdgesToLoops(const std::vector<std::pair<size_t, size_t>> &edges,
        std::vector<std::vector<size_t>> &edgeLoops);
    void subBlockToMesh(const SubBlock &subBlock, Mesh &mesh);
    Mesh m_firstMesh;
    Mesh m_secondMesh;
    Mesh m_resultMesh;
    static size_t m_maxOctreeDepth;
    static size_t m_minIntersectsInOctant;
    static int m_vertexToKeyMultiplyFactor;
    std::vector<AxisAlignedBoudingBox> m_firstMeshFaceAABBs;
    std::vector<AxisAlignedBoudingBox> m_secondMeshFaceAABBs;
    std::vector<Vertex> m_newVertices;
    std::map<std::tuple<int, int, int>, size_t> m_newVertexToIndexMap;
};

}

#endif

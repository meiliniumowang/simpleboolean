#include <simpleboolean/retriangulation.h>
#include <simpleboolean/util.h>
#include <simpleboolean/triangulate.h>
#include <set>
#include <vector>
#include <map>
#include <cmath>

namespace simpleboolean
{

void ReTriangulation::reTriangulate(const std::vector<Vertex> &vertices,
        const std::vector<size_t> &triangle,
        const std::vector<std::vector<size_t>> &edgeLoops,
        std::vector<Face> &reTriangulatedTriangles)
{
    std::vector<std::pair<size_t, size_t>> newEdges;
    
    // Test all heads and tails of open edge loops with the edges of triangle,
    // order by distances per each triangle edge.
    std::set<size_t> endpoints;
    for (const auto &edgeLoop: edgeLoops) {
        if (edgeLoop.front() != edgeLoop.back()) {
            endpoints.insert(edgeLoop.front());
            endpoints.insert(edgeLoop.back());
        }
    }
    std::vector<float> triangleEdgeLengths;
    for (size_t i = 0; i < 3; ++i) {
        size_t j = (i + 1) % 3;
        triangleEdgeLengths.push_back(distanceOfVertices(vertices[triangle[i]], vertices[triangle[j]]));
    }
    std::map<std::pair<size_t, size_t>, float> distancesBetweenEndpointAndCorner;
    for (const auto index: endpoints) {
        for (size_t i = 0; i < 3; ++i) {
            distancesBetweenEndpointAndCorner.insert({{index, i},
                distanceOfVertices(vertices[index], vertices[triangle[i]])
            });
        }
    }
    std::map<size_t, std::vector<std::pair<size_t, float>>> endpointsAttachedToEdges;
    for (const auto index: endpoints) {
        std::vector<std::tuple<size_t, float, float>> offsets;
        for (size_t i = 0; i < 3; ++i) {
            size_t j = (i + 1) % 3;
            float firstHalf = distancesBetweenEndpointAndCorner[{index, i}];
            float secondHalf = distancesBetweenEndpointAndCorner[{index, j}];
            offsets.push_back({i, abs(firstHalf + secondHalf - triangleEdgeLengths[i]), firstHalf});
        }
        std::sort(offsets.begin(), offsets.end(), [](const std::tuple<size_t, float, float> &first,
                const std::tuple<size_t, float, float> &second) {
            return std::get<1>(first) < std::get<1>(second);
        });
        endpointsAttachedToEdges[std::get<0>(offsets.front())].push_back({index,
            std::get<2>(offsets.front())});
    }
    for (auto &it: endpointsAttachedToEdges) {
        size_t startCorner = triangle[it.first];
        size_t stopCorner = triangle[(it.first + 1) % 3];
        std::sort(it.second.begin(), it.second.end(), [](const std::pair<size_t, float> &first,
                const std::pair<size_t, float> &second) {
            return first.second < second.second;
        });
        std::vector<size_t> points;
        points.push_back(startCorner);
        for (const auto &it: it.second) {
            points.push_back(it.first);
        }
        points.push_back(stopCorner);
        for (size_t i = 1; i < points.size(); ++i) {
            newEdges.push_back({points[i - 1], points[i]});
        }
    }
    
    // Closed edge loops should have one vertex connected with other vertices.
    // FIXME: Optimize me
    std::vector<std::pair<size_t, long>> edgeVertices;
    for (size_t i = 0; i < 3; ++i) {
        edgeVertices.push_back({triangle[i], -1});
    }
    for (size_t loopIndex = 0; loopIndex < edgeLoops.size(); ++loopIndex) {
        const auto &edgeLoop = edgeLoops[loopIndex];
        bool isClosed = edgeLoop[0] == edgeLoop[edgeLoop.size() - 1];
        for (size_t i = isClosed ? 1 : 0; i < edgeLoop.size(); ++i) {
            edgeVertices.push_back({edgeLoop[i], loopIndex});
        }
        for (size_t i = 1; i < edgeLoop.size(); ++i) {
            newEdges.push_back({edgeLoop[i - 1], edgeLoop[i]});
            newEdges.push_back({edgeLoop[i], edgeLoop[i - 1]});
        }
    }
    for (size_t loopIndex = 0; loopIndex < edgeLoops.size(); ++loopIndex) {
        const auto &edgeLoop = edgeLoops[loopIndex];
        bool isClosed = edgeLoop[0] == edgeLoop[edgeLoop.size() - 1];
        if (!isClosed)
            continue;
        std::tuple<long, long, float> shortest = {-1, -1, std::numeric_limits<float>::max()};
        for (const auto &point: edgeLoop) {
            for (const auto &other: edgeVertices) {
                if (other.second == (long)loopIndex)
                    continue;
                float distance = distanceOfVertices(vertices[point], vertices[other.first]);
                if (distance < std::get<2>(shortest))
                    shortest = {point, other.first, distance};
            }
        }
        if (-1 == std::get<0>(shortest))
            continue;
        newEdges.push_back({std::get<0>(shortest), std::get<1>(shortest)});
        newEdges.push_back({std::get<1>(shortest), std::get<0>(shortest)});
    }

    // Create edge loops from new edges
    std::vector<std::vector<size_t>> newEdgeLoops;
    std::map<size_t, std::vector<size_t>> halfEdgeLinkMap;
    for (const auto &edge: newEdges) {
        halfEdgeLinkMap[edge.first].push_back(edge.second);
    }
    std::set<std::pair<size_t, size_t>> visited;
    while (!halfEdgeLinkMap.empty()) {
        std::vector<size_t> loop;
        size_t startVert = halfEdgeLinkMap.begin()->first;
        size_t loopVert = startVert;
        bool newEdgeLoopGenerated = false;
        while (true) {
            loop.push_back(loopVert);
            auto findNext = halfEdgeLinkMap.find(loopVert);
            if (findNext == halfEdgeLinkMap.end())
                break;
            bool foundNextVert = false;
            for (auto it = findNext->second.begin(); it != findNext->second.end(); ++it) {
                if (visited.find({loopVert, *it}) == visited.end()) {
                    visited.insert({loopVert, *it});
                    foundNextVert = true;
                    loopVert = *it;
                    findNext->second.erase(it);
                    break;
                }
            }
            if (!foundNextVert)
                break;
            if (loopVert == startVert) {
                for (const auto &vert: loop) {
                    auto findVert = halfEdgeLinkMap.find(vert);
                    if (findVert == halfEdgeLinkMap.end())
                        continue;
                    if (findVert->second.empty())
                        halfEdgeLinkMap.erase(findVert);
                }
                newEdgeLoops.push_back(loop);
                newEdgeLoopGenerated = true;
                break;
            }
        }
        if (!newEdgeLoopGenerated)
            break;
    }
    
    // Triangle edge loops
    for (const auto &it: newEdgeLoops) {
        triangulate(vertices, reTriangulatedTriangles, it);
    }
}

}


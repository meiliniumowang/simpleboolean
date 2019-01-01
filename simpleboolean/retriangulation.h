#ifndef SIMPLEBOOLEAN_RETRIANGULATION_H
#define SIMPLEBOOLEAN_RETRIANGULATION_H
#include <simpleboolean/meshdatatype.h>

namespace simpleboolean
{
  
class ReTriangulation
{
public:
    void reTriangulate(const std::vector<Vertex> &vertices,
        const std::vector<size_t> &triangle,
        const std::vector<std::vector<size_t>> &edgeLoops,
        std::vector<std::vector<size_t>> &reTriangulatedTriangles);
};
  
}

#endif

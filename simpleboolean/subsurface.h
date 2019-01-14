#ifndef SIMPLEBOOLEAN_SUB_SURFACE_H
#define SIMPLEBOOLEAN_SUB_SURFACE_H
#include <vector>
#include <simpleboolean/meshdatatype.h>

namespace simpleboolean
{

class SubSurface
{
public:
    static void createSubSurfaces(const std::vector<std::vector<size_t>> &edgeLoops,
        const std::vector<Face> &triangles,
        std::vector<SubSurface> &subSurfaces);
};

}

#endif

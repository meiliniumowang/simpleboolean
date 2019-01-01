#ifndef SIMPLEBOOLEAN_SUB_SURFACE_H
#define SIMPLEBOOLEAN_SUB_SURFACE_H
#include <vector>

namespace simpleboolean
{

class SubSurface
{
public:
    static void createSubSurfaces(const std::vector<std::vector<size_t>> &edgeLoops,
        const std::vector<std::vector<size_t>> &triangles,
        std::vector<SubSurface> &subSurfaces);
};

}

#endif

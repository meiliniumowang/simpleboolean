#ifndef SIMPLEBOOLEAN_SUB_BLOCK_H
#define SIMPLEBOOLEAN_SUB_BLOCK_H
#include <simpleboolean/subsurface.h>

namespace simpleboolean
{

class SubBlock
{
public:
    static void createSubBlocks(const std::vector<SubSurface> &firstSubSurfaces,
        const std::vector<SubSurface> &secondSubSurfaces,
        std::vector<SubBlock> &subBlocks);
};

}

#endif

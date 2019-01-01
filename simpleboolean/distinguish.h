#ifndef SIMPLEBOOLEAN_DISTINGUISH_H
#define SIMPLEBOOLEAN_DISTINGUISH_H
#include <simpleboolean/subblock.h>

namespace simpleboolean
{

enum class Operation
{
    Union,
    Intersection,
    Subtraction
};

class Distinguish
{
public:
    static void distinguish(const std::vector<SubBlock> &subBlocks,
        std::vector<Operation> &operations);
};

}

#endif

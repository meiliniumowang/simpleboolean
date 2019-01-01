#ifndef SIMPLEBOOLEAN_EDGE_LOOP_H
#define SIMPLEBOOLEAN_EDGE_LOOP_H
#include <vector>

namespace simpleboolean
{

class EdgeLoop
{
public:
    static void merge(std::vector<std::vector<size_t>> &edgeLoops);
};

}

#endif

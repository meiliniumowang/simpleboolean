#ifndef SIMPLEBOOLEAN_UTIL_H
#define SIMPLEBOOLEAN_UTIL_H
#include <simpleboolean/meshdatatype.h>

namespace simpleboolean
{

float distanceSquaredOfVertices(const Vertex &first, const Vertex &second);
float distanceOfVertices(const Vertex &first, const Vertex &second);

}

#endif

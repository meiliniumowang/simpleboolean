#ifndef SIMPLEBOOLEAN_TRIANGULATE_H
#define SIMPLEBOOLEAN_TRIANGULATE_H
#include <simpleboolean/meshdatatype.h>

namespace simpleboolean
{

void triangulate(const std::vector<Vertex> &vertices, std::vector<Face> &faces, const std::vector<size_t> &ring);

}

#endif

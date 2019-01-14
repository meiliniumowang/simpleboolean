#include <simpleboolean/util.h>
#include <QVector3D>

namespace simpleboolean
{

static QVector3D vertexToQVector3D(const Vertex &first)
{
    return QVector3D(first.xyz[0], first.xyz[1], first.xyz[2]);
}

float distanceSquaredOfVertices(const Vertex &first, const Vertex &second)
{
    return (vertexToQVector3D(first) - vertexToQVector3D(second)).lengthSquared();
}

float distanceOfVertices(const Vertex &first, const Vertex &second)
{
    return (vertexToQVector3D(first) - vertexToQVector3D(second)).length();
}

}

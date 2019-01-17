#include <qmath.h>
#include "testmesh.h"

// Modified from http://doc.qt.io/qt-5/qtopengl-hellogl2-logo-cpp.html

TestMesh::TestMesh()
{
    const GLfloat x1 = +0.06f;
    const GLfloat y1 = -0.14f;
    const GLfloat x2 = +0.14f;
    const GLfloat y2 = -0.06f;
    const GLfloat x3 = +0.08f;
    const GLfloat y3 = +0.00f;
    const GLfloat x4 = +0.30f;
    const GLfloat y4 = +0.22f;

    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);
}

void TestMesh::add(const QVector3D &v)
{
    m_mesh.vertices.push_back({{
        v.x(),
        v.y(),
        v.z()
    }});
    size_t vertexNum = m_mesh.vertices.size();
    if (0 == vertexNum % 3) {
        m_mesh.faces.push_back({{
            vertexNum - 3,
            vertexNum - 2,
            vertexNum - 1
        }});
    }
}

void TestMesh::quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4)
{
    add(QVector3D(x1, y1, -0.05f));
    add(QVector3D(x4, y4, -0.05f));
    add(QVector3D(x2, y2, -0.05f));

    add(QVector3D(x3, y3, -0.05f));
    add(QVector3D(x2, y2, -0.05f));
    add(QVector3D(x4, y4, -0.05f));

    add(QVector3D(x4, y4, 0.05f));
    add(QVector3D(x1, y1, 0.05f));
    add(QVector3D(x2, y2, 0.05f));

    add(QVector3D(x2, y2, 0.05f));
    add(QVector3D(x3, y3, 0.05f));
    add(QVector3D(x4, y4, 0.05f));
}

void TestMesh::extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    add(QVector3D(x1, y1, +0.05f));
    add(QVector3D(x1, y1, -0.05f));
    add(QVector3D(x2, y2, +0.05f));

    add(QVector3D(x2, y2, -0.05f));
    add(QVector3D(x2, y2, +0.05f));
    add(QVector3D(x1, y1, -0.05f));
}

const simpleboolean::Mesh &TestMesh::result()
{
    return m_mesh;
}

void TestMesh::translate(const QVector3D &translation)
{
    for (auto &vertex: m_mesh.vertices) {
        vertex.xyz[0] += translation.x();
        vertex.xyz[1] += translation.y();
        vertex.xyz[2] += translation.z();
    }
}

void TestMesh::transform(const QMatrix4x4 &matrix)
{
    for (auto &vertex: m_mesh.vertices) {
        QVector3D v = {vertex.xyz[0],
            vertex.xyz[1],
            vertex.xyz[2]
        };
        v = matrix * v;
        vertex.xyz[0] += v.x();
        vertex.xyz[1] += v.y();
        vertex.xyz[2] += v.z();
    }
}

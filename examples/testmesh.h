#ifndef TEST_MESH_H
#define TEST_MESH_H
#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
#include <simpleboolean/meshdatatype.h>

class TestMesh
{
public:
    TestMesh();
    const simpleboolean::Mesh &result();
    void translate(const QVector3D &translation);
    void transform(const QMatrix4x4 &matrix);

private:
    void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);
    void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void add(const QVector3D &v);

    simpleboolean::Mesh m_mesh;
};

#endif // LOGO_H

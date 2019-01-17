#ifndef MODEL_H
#define MODEL_H
#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <simpleboolean/meshdatatype.h>

class Model
{
public:
    Model(const simpleboolean::Mesh &mesh);
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_data.size(); }
    int vertexCount() const { return m_data.size() / 6; }

private:
    QVector<GLfloat> m_data;
};

#endif

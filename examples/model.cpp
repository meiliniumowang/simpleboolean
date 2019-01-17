#include "model.h"

Model::Model(const simpleboolean::Mesh &mesh)
{
    for (const auto &face: mesh.faces) {
        const auto &a = mesh.vertices[face.indices[0]];
        const auto &b = mesh.vertices[face.indices[1]];
        const auto &c = mesh.vertices[face.indices[2]];
        QVector3D v1 = {a.xyz[0], a.xyz[1], a.xyz[2]};
        QVector3D v2 = {b.xyz[0], b.xyz[1], b.xyz[2]};
        QVector3D v3 = {c.xyz[0], c.xyz[1], c.xyz[2]};
        auto norm = QVector3D::normal(v1, v2, v3);
        m_data.push_back(a.xyz[0]);
        m_data.push_back(a.xyz[1]);
        m_data.push_back(a.xyz[2]);
        m_data.push_back(norm.x());
        m_data.push_back(norm.y());
        m_data.push_back(norm.z());
        m_data.push_back(b.xyz[0]);
        m_data.push_back(b.xyz[1]);
        m_data.push_back(b.xyz[2]);
        m_data.push_back(norm.x());
        m_data.push_back(norm.y());
        m_data.push_back(norm.z());
        m_data.push_back(c.xyz[0]);
        m_data.push_back(c.xyz[1]);
        m_data.push_back(c.xyz[2]);
        m_data.push_back(norm.x());
        m_data.push_back(norm.y());
        m_data.push_back(norm.z());
        
    }
}

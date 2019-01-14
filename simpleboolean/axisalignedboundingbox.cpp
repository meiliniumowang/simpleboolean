#include <simpleboolean/axisalignedboundingbox.h>
#include <cmath>

namespace simpleboolean
{

void AxisAlignedBoudingBox::update(const Vertex &vertex)
{
    for (size_t i = 0; i < 3; ++i) {
        if (vertex.xyz[i] > m_max.xyz[i])
            m_max.xyz[i] = vertex.xyz[i];
        if (vertex.xyz[i] < m_min.xyz[i])
            m_min.xyz[i] = vertex.xyz[i];
    }
}

const Vertex &AxisAlignedBoudingBox::lowerBound() const
{
    return m_min;
}

const Vertex &AxisAlignedBoudingBox::upperBound() const
{
    return m_max;
}

Vertex &AxisAlignedBoudingBox::lowerBound()
{
    return m_min;
}

Vertex &AxisAlignedBoudingBox::upperBound()
{
    return m_max;
}

bool AxisAlignedBoudingBox::intersectWith(const AxisAlignedBoudingBox &other, AxisAlignedBoudingBox *result) const
{
    const Vertex &otherMin = other.lowerBound();
    const Vertex &otherMax = other.upperBound();
    for (size_t i = 0; i < 3; ++i) {
        if (m_min.xyz[i] <= otherMax.xyz[i] && m_max.xyz[i] >= otherMin.xyz[i])
            continue;
        return false;
    }
    if (nullptr == result)
        return true;
    for (size_t i = 0; i < 3; ++i) {
        std::vector<float> points = {
            m_min.xyz[i], otherMax.xyz[i], m_max.xyz[i], otherMin.xyz[i]
        };
        std::sort(points.begin(), points.end(), [](const float &first, const float &second) {
            return first < second;
        });
        result->lowerBound().xyz[i] = points[1];
        result->upperBound().xyz[i] = points[2];
    }
    return true;
}

void AxisAlignedBoudingBox::makeOctree(std::vector<AxisAlignedBoudingBox> &octants)
{
    Vertex origin = {{(m_min.xyz[0] + m_max.xyz[0]) * 0.5f,
        (m_min.xyz[1] + m_max.xyz[1]) * 0.5f,
        (m_min.xyz[2] + m_max.xyz[2]) * 0.5f
    }};
    Vector radius = {{
        abs(m_min.xyz[0] - m_max.xyz[0]) * 0.5f,
        abs(m_min.xyz[1] - m_max.xyz[1]) * 0.5f,
        abs(m_min.xyz[2] - m_max.xyz[2]) * 0.5f
    }};
    octants.resize(8);
    for (size_t i = 0; i < octants.size(); ++i) {
        Vertex newOrigin = origin;
        newOrigin.xyz[0] += radius.xyz[0] * ((i & 4) ? 0.5f : -0.5f);
        newOrigin.xyz[1] += radius.xyz[1] * ((i & 2) ? 0.5f : -0.5f);
        newOrigin.xyz[2] += radius.xyz[2] * ((i & 1) ? 0.5f : -0.5f);
        for (size_t j = 0; j < 3; ++j) {
            octants[i].lowerBound().xyz[j] = newOrigin.xyz[j] - radius.xyz[j] * 0.5f;
        }
        for (size_t j = 0; j < 3; ++j) {
            octants[i].upperBound().xyz[j] = newOrigin.xyz[j] + radius.xyz[j] * 0.5f;
        }
    }
}

}

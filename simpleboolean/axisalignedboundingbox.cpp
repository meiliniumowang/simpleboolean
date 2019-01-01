#include <simpleboolean/axisalignedboundingbox.h>

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
    // TODO:
}

}

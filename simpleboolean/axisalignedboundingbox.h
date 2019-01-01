#ifndef SIMPLEBOOLEAN_AXIS_ALIGNED_BOUNDING_BOX_H
#define SIMPLEBOOLEAN_AXIS_ALIGNED_BOUNDING_BOX_H
#include <simpleboolean/meshdatatype.h>

namespace simpleboolean
{
  
class AxisAlignedBoudingBox
{
public:
    void update(const Vertex &vertex);
    const Vertex &lowerBound() const;
    const Vertex &upperBound() const;
    Vertex &lowerBound();
    Vertex &upperBound();
    bool intersectWith(const AxisAlignedBoudingBox &other, AxisAlignedBoudingBox *result=nullptr) const;
    void makeOctree(std::vector<AxisAlignedBoudingBox> &octants);
    
private:
    Vertex m_min = {
        {
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
        }
    };
    Vertex m_max = {
        {
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min(),
        }
    };
};
  
}


#endif

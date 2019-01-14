#include <simpleboolean/axisalignedboundingbox.h>

static void printBox(const simpleboolean::AxisAlignedBoudingBox &box)
{
    printf("{%.02f, %.02f, %.02f} - {%.02f, %.02f, %.02f}",
        box.lowerBound().xyz[0],
        box.lowerBound().xyz[1],
        box.lowerBound().xyz[2],
        box.upperBound().xyz[0],
        box.upperBound().xyz[1],
        box.upperBound().xyz[2]);
}

int main(int argc , char *argv[])
{
    simpleboolean::AxisAlignedBoudingBox box;
    box.update({{0.1, 0.1, 0.1}});
    box.update({{0.2, 0.2, 0.2}});
    printf("    ");
    printBox(box);
    printf("\r\n");
    std::vector<simpleboolean::AxisAlignedBoudingBox> octants;
    box.makeOctree(octants);
    for (size_t i = 0; i < 8; ++i) {
        const auto &subBox = octants[i];
        printf("[%d] ", i);
        printBox(subBox);
        printf("\r\n");
    }
    return 0;
}

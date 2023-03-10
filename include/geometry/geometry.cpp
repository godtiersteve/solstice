#include "geometry.h"

namespace solstice {
    GridCell GetCells(int minX, int minY, int celSize, Rectanglef& rect) {
        GridCell ret;
        ret.left = (int)(rect.GetLeft() - minX) / celSize;
        ret.right = (int)(rect.GetRight() - minX) / celSize;
        ret.top = (int)(rect.GetTop() - minY) / celSize;
        ret.bottom = (int)(rect.GetBottom() - minY) / celSize;
        return ret;
    }
}

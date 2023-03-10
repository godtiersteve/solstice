#include "line_funcs.h"

namespace solstice {
    int Distance(Vector2<float> pt1, Vector2<float> pt2) {
        return sqrt((Square(pt2.x - pt1.x) + Square(pt2.y - pt1.y)));
    }


    float AngleBeteewn(Vector2<float> pt1, Vector2<float> pt2) {
        return atan(Distance(pt1, pt2));
    }

    float AngleBetween2(Vector2<float> pt1, Vector2<float> pt2) {
        return atan2(pt2.y - pt1.y, pt2.x - pt1.x);
    }
};

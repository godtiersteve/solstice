#ifndef LINE_FUNCS_H_INCLUDED
#define LINE_FUNCS_H_INCLUDED

#include "graphics/vector2.h"
#include "utility/sol_math.h"

namespace solstice {
    int Distance(Vector2f pt1, Vector2f pt2);
    float AngleBeteewn(Vector2f pt1, Vector2f pt2);
    float AngleBetween2(Vector2f pt1, Vector2f pt2);
};
#endif // LINE_FUNCS_H_INCLUDED

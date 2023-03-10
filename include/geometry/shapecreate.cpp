#include "shapecreate.h"

namespace solstice {
    Polygonf CreateSlope(Vector2f origin, float height, float width, float angle, bool facingRight) {
        std::vector<Vector2f> verts;
        float top = width * 0.5 * sin(angle);
        if(!facingRight) {
            verts.push_back(origin);
            verts.push_back(origin + Vector2f(width, 0));
            verts.push_back(origin + Vector2f(width, height));
            verts.push_back(origin + Vector2f(0, height - top));
        }
        else {
            verts.push_back(origin);
            verts.push_back(origin + Vector2f(0, height));
            verts.push_back(origin + Vector2f(width, height - top));
            verts.push_back(origin + Vector2f(width, 0));
        }
        //origin.x = origin.x + width * 0.5f;
        return Polygonf(verts, origin);
    }
}

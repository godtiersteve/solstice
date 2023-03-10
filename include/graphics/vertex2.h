#ifndef VERTEX2_H_INCLUDED
#define VERTEX2_H_INCLUDED
#include "color.h"
#include "vector2.h"
namespace solstice {
template<class T>
struct Vertex2 : public Vector2<T> {
    Color color;
    Vector2f texCoord;
    int textureID;
    Vertex2<T>() {
        this->x = 0;
        this->y = 0;
        texCoord = 0;
        color = COL_WHITE;
        textureID = -1;
    }
    Vertex2<T>(const Vector2<T>& vt) {
        *this = vt;
        textureID = -1;
    }
    Vertex2<T>(const Vector2<T>& pos, const Vector2<T>& tex) {
        *this = pos;
        texCoord = tex;
        textureID = -1;
    }
    Vertex2<T>(const glm::vec2& pos, const glm::vec2& tex) {
        *this = pos;
        texCoord.x = tex.x;
        texCoord.y = tex.y;
        textureID = -1;
    }
    Vertex2<T>& operator=(const Vector2<T>& vt) {
        this->x = vt.x;
        this->y = vt.y;
        return *this;
    }
    Vertex2<T>& operator=(const glm::vec2& vt) {
        this->x = vt.x;
        this->y = vt.y;
        return *this;
    }
};
typedef Vertex2<float> Vertex2f;
typedef Vertex2<int> Vertex2i;
typedef Vertex2<double> Vertex2d;
typedef Vertex2<unsigned int> Vertex2ui;
typedef Vertex2<short> Vertex2s;
}

#endif // Vector2F_H_INCLUDED

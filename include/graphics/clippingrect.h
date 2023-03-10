#ifndef CLIPPINGRECT_H_INCLUDED
#define CLIPPINGRECT_H_INCLUDED

#include "vector2.h"

namespace solstice {
    struct ClippingRect {
        float height, width;
        int row, col;
        float clipWidth, clipHeight;
        ClippingRect() {
            height = 32;
            width = 32;
            row = 0;
            col = 0;
        }
        ClippingRect(int h, int w, float size) {
            height = h;
            width = w;
            clipWidth = size;
            clipHeight = size;
        }
        ClippingRect(int h, int w, float cwidth, float cheight, int r, int c) {
            height = h;
            width = w;
            clipWidth = cwidth;
            clipHeight = cheight;
            row = r;
            col = c;
        }
        ClippingRect(int h, int w, float size, int r, int c) {
            height = h;
            width = w;
            clipHeight = size;
            clipWidth = size;
            row = r;
            col = c;
        }
        Vector2f GetOrigin() {
            return Vector2f(col * clipWidth, row * clipHeight);
        }

    };

}

#endif // CLIPPINGRECT_H_INCLUDED

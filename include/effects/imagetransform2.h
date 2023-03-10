#ifndef IMAGETRANSFORM_2_H_INCLUDED
#define IMAGETRANSFORM_2_H_INCLUDED

#include "graphics/vector2.h"
#include "graphics/color.h"
#include "graphics/quaddata.h"
#include "graphics/multiquad.h"

namespace solstice {
struct ImageTransform2 {
    float xSkew, ySkew, angle, fade, scale;
    Vector2f velocity, acceleration;
    Vector2f destination;
    int duration;
    ImageTransform2();
    void TransformQuad(QuadData& q);
    void TransformMultiQuad(MultiImage& q);
};

struct ImageTransform2Step {
    float angleStep, xSkewStep, ySkewStep, fadeStep, scaleStep;
    Vector2f velocity, acceleration;
    ImageTransform2Step();
};

struct AfterImage {
    Color color;
    Vector2f position;
    int lifespan;
    ImageTransform2 transform;
    AfterImage();
    AfterImage(Vector2f pos, int life, Color col = COL_WHITE);
    AfterImage(Vector2f pos, int life, ImageTransform2 trans, Color col = COL_WHITE);
};
}


#endif // EFFECPOINT2_H_INCLUDED

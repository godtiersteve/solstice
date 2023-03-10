#include "ImageTransform2.h"

namespace solstice {
    ImageTransform2::ImageTransform2() {
        xSkew = 0;
        ySkew = 0;
        scale = 1.0f;
        angle = 0;
        duration = 10;
        fade = 1.0;
    }
    ImageTransform2Step::ImageTransform2Step() {
        angleStep = 0;
        xSkewStep = 0;
        ySkewStep = 0;
        scaleStep = 0;
        fadeStep = 0;
    }
    void ImageTransform2::TransformQuad(QuadData& q) {
        q.SetAngle(angle);
        q.SetScale(scale);
        q.AddXY(velocity.x, velocity.y);
        q.GetColor().alpha = fade;
    }
    void ImageTransform2::TransformMultiQuad(MultiImage& q) {
        q.SetAngle(angle);
        q.SetScale(scale);
        q.AddXY(velocity.x, velocity.y);
        q.SetAlpha(fade);
    }
    AfterImage::AfterImage() {
        color = COL_WHITE;
        color.alpha = 0.75f;
        lifespan = 10;
    }
    AfterImage::AfterImage(Vector2f pos, int life, Color col) {
        position = pos;
        lifespan = life;
        color = col;
        color.alpha = 0.75f;
    }
    AfterImage::AfterImage(Vector2f pos, int life, ImageTransform2 trans, Color col) {
        position = pos;
        lifespan = life;
        color = col;
        transform = trans;
    }
}

#include "rendertile.h"

namespace solstice {
RenderTile::RenderTile() {
    //mFlipsModel = false;
}

RenderTile::RenderTile(std::shared_ptr<IShapef> shape) {
    if(shape) {
        BackgroundTile2 tile;
        mTile.GetHitBox2() = HitBoxf(shape);
    }
}

RenderTile::RenderTile(BackgroundTile2 tile, int modelID, glm::mat4* transform) {
    Init(tile, modelID, transform);
}

void RenderTile::Init(BackgroundTile2 tile, int modelID, glm::mat4* transform) {
    mTile = tile;
}
Rectanglef RenderTile::GetHitRect() {
    if(mTile.mHitBox2.Get()->GetType() == SHAPE_RECT) {
        return mTile.mHitBox2.GetRect(0);
    }
    else {
        return mTile.mHitBox2.Get()->GetAABB();
    }
}
void RenderTile::AddAngle(float a) {
    a+= mTile.mHitBox2.GetAngle();
    mTile.mHitBox2.SetAngle(a);
}
void RenderTile::IncreaseSize(int height, int width) {
    if(mTile.HitBoxType() == SHAPE_RECT) {
        Rectanglef& rect = *dynamic_cast<Rectanglef*>(mTile.GetHBox().get());
        height = height ? height + rect.GetHeight() : rect.GetHeight();
        width = width ? width + rect.GetWidth() : rect.GetWidth();
        rect.SetHW(height, width);
    }
    else if(mTile.HitBoxType() == SHAPE_POLY) {
        if(height || width) {
            Polygonf& poly = *static_cast<Polygonf*>(mTile.GetHBox().get());
            Vector2f origin = poly.GetPosition();
            bool facingRight = true;
            Vector2f diff = poly.GetVertex(2) - poly.GetVertex(3);
            if(mTile.mStyle == TILE_SLOPE_30_R || mTile.mStyle == TILE_SLOPE_45_R || mTile.mStyle == TILE_SLOPE_60_R) {
                facingRight = false;
                width = -width;
                origin.x-= width;
            }
            height = height ? height + poly.GetHeight() : poly.GetHeight();
            width = width ? width + poly.GetWidth() : poly.GetWidth();
            poly = CreateSlope(origin, height, width, mTile.mAngle, facingRight);
        }

    }
}
void RenderTile::IncreaseWidth(int width) {
    if(mTile.HitBoxType() == SHAPE_RECT) {
        Rectanglef& rect = *dynamic_cast<Rectanglef*>(mTile.GetHBox().get());
        if(width && width+ rect.GetWidth() > 0) {
            rect.SetWidth(rect.GetWidth() + width);
        }
    }
    else if(mTile.HitBoxType() == SHAPE_POLY) {
        if(width) {
            Polygonf& poly = *static_cast<Polygonf*>(mTile.GetHBox().get());
            Vector2f origin = poly.GetPosition();
            bool facingRight = true;
            Vector2f diff = poly.GetVertex(2) - poly.GetVertex(3);
            if(mTile.mStyle == TILE_SLOPE_30_R || mTile.mStyle == TILE_SLOPE_45_R || mTile.mStyle == TILE_SLOPE_60_R) {
                facingRight = false;
                width = -width;
                origin.x-= width;
            }
            int height =  poly.GetHeight();
            width = width ? width + poly.GetWidth() : poly.GetWidth();
            poly = CreateSlope(origin, height, width, mTile.mAngle, facingRight);
        }

    }
}
void RenderTile::IncreaseHeight(int height) {
    if(mTile.HitBoxType() == SHAPE_RECT) {
        Rectanglef& rect = *dynamic_cast<Rectanglef*>(mTile.GetHBox().get());
        height = height ? height + rect.GetHeight() : rect.GetHeight();
        rect.SetHeight(height);
    }
    else if(mTile.HitBoxType() == SHAPE_POLY) {
        if(height) {
            Polygonf& poly = *static_cast<Polygonf*>(mTile.GetHBox().get());
            Vector2f origin = poly.GetPosition();
            bool facingRight = true;
            Vector2f diff = poly.GetVertex(2) - poly.GetVertex(3);
            int width = poly.GetWidth();
            if(mTile.mStyle == TILE_SLOPE_30_R || mTile.mStyle == TILE_SLOPE_45_R || mTile.mStyle == TILE_SLOPE_60_R) {
                facingRight = false;
                width = -width;
                origin.x-= width;
            }
            height =  height ? height + poly.GetHeight() : poly.GetHeight();
            poly = CreateSlope(origin, height, width, mTile.mAngle, facingRight);
        }

    }
}

}

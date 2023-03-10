#include "waterbody.h"

namespace solstice {
    WaterBody::WaterBody() {
        mWaterColor = COL_BLUE;
        mWaterColor.alpha = 1.0f;
        mZSplashPos = 0;
        mWaterType = WATER_FRESH;
        mSurfaceHeight = 25;
    }
    WaterBody::WaterBody(WaterSurface surface, Rectanglef hitbox) {
        Init(surface, hitbox);
        mWaterType = WATER_FRESH;
        mSurfaceHeight = 25;
    }
    WaterBody::WaterBody(Vector2f origin, float height, float width, float springHeight, float numSprings) {
        float springWidth = width / numSprings;
        Init(WaterSurface(springWidth, springHeight, width), Rectanglef(origin, height, width));
        mWaterType = WATER_FRESH;
        mSurfaceHeight = 25;
    }
    void WaterBody::Init(WaterSurface surface, Rectanglef hitbox) {
        mSurface = surface;
        mHitBox = hitbox;
        mWaterColor = COL_BLUE;
        mWaterColor.alpha = 1.0f;
        SetPosition(hitbox.GetOrigin());
        mSurface.SetPosition(mHitBox.GetTopLeft());
        mZSplashPos = 0;
    }
    void WaterBody::Init(Vector2f origin, float height, float width, float springHeight, float numSprings) {
        float springWidth = width / numSprings;
        Init(WaterSurface(springWidth, springHeight, width), Rectanglef(origin, height, width));
    }
    void WaterBody::SetPosition(Vector2f vt) {
        mHitBox.SetPosition(vt);
        mSurface.SetPosition(Vector2f(vt.x, vt.y + mHitBox.GetHeight()));
        //SetPosition(vt);
    }
    void WaterBody::Update(double dt) {
        UpdateSprings(dt);
    }
    Polygonf WaterBody::GetPolygon() {
        Polygonf p = mSurface.GetSurfacePoly();
        p.AddVertex(0, mHitBox.GetPosition());
        p.AddVertex(p.GetNumVert(), Vector2f(mHitBox.GetPosition().x + mHitBox.GetWidth(), mHitBox.GetPosition().y));
        return p;
    }
    void WaterBody::Ripple(float force) {
        mSurface.Ripple(force);
    }
    void WaterBody::Splash(Rectanglef rect, float force) {
        int size = mSplashMesh.GetSize();
        size_t i = (rect.GetLeft() - mHitBox.GetLeft()) / size;
        size_t j = (rect.GetRight() - mHitBox.GetLeft()) / size;
        if(j >= mSplashMesh.GetXSprings()) {
            return;
        }
        mSplashMesh.SplashRange(i, j, mZSplashPos, mZSplashPos, force);
    }
    std::vector<Rectanglef> WaterBody::GetRects() {
        std::vector<Rectanglef> rects(mSplashMesh.GetXSprings());
        for(size_t i = 0; i < mSplashMesh.GetXSprings(); i++) {
            //Rectanglef waterRect = mSplashMesh.GetHitBox(i, 1);
            Spring& spring = mSplashMesh.GetSpring(i, 1);
            Vector2f pos = mHitBox.GetPosition().x + i * mSplashMesh.GetSize();
            pos.y = mHitBox.GetTop();
            Rectanglef waterRect(pos, spring.height, spring.width);
            rects[i] = waterRect;
        }
        return rects;
    }
    float WaterBody::CheckCollision(Rectanglef rect) {
        float depth = 0;
        if(RectRectCollision(mHitBox, rect)) {
            depth = mHitBox.GetTop() - rect.GetBottom();
            return depth;
        }
        else {
            return 0;
        }
    }
    float WaterBody::CheckCollision(Circlef circ) {
        float depth = 0;
        if(solstice::CheckCollision(&mHitBox, &circ)) {
            depth = mHitBox.GetTop() - circ.GetAABB().GetBottom();
            return depth;
        }
        else {
            return 0;
        }
    }
    float WaterBody::CheckCollision(IShapef* shape) {
        if(shape->GetType() == SHAPE_RECT) {
            return CheckCollision(*static_cast<Rectanglef*>(shape));
        }
        else if(shape->GetType() == SHAPE_CIRCLE) {
            return CheckCollision(*static_cast<Circlef*>(shape));
        }
        return 0;
    }
    bool WaterBody::CheckSurfaceCollision(Rectanglef& rect) {
        Rectanglef surface = GetSurfaceRect();
        return RectRectCollision(rect, surface);
    }
    void WaterBody::IncreaseWidth(int w) {
        IncreaseSize(mHitBox.GetHeight(), w);
    }
    void WaterBody::IncreaseHeight(int h) {
        IncreaseSize(h, mHitBox.GetWidth());
    }
    void WaterBody::IncreaseSize(int height, int width) {
        height = height ? mHitBox.GetHeight() + height : mHitBox.GetHeight();
        width = width ? mHitBox.GetWidth() + width : mHitBox.GetWidth();
        mHitBox.SetHW(height, width);
    }
}

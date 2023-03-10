#include "watersurface.h"

namespace solstice {
WaterSurface::WaterSurface(int springWidth, float height, float width) {
    Init(springWidth, height, width);
}
void WaterSurface::Init(int springWidth, float height, float width) {
    mWaterColor = COL_BLUE;
    mWaterColor.alpha = 1.0f;
    mSpringHeight = height;
    mSpringWidth = springWidth;
    mTextureID = -1;
    mTextureRef = "";
    mWaterColor = COL_BLUE;
    mSpread = 0.225f;
    mDamping = 0.075f;
    mSpringConstant = 0.0025f;
    mRipples = false;
    for(int i = 0; i < width / springWidth; i++) {
        Vector2f position(i * springWidth, -height);
        mWaterSurface.push_back(WaterSpring(position, height, springWidth));
    }
    GeneratePolygon();
}
void WaterSurface::ReinitSurface(Vector2f position, float height, float width) {
    int amt = mWaterSurface.size();
    mWaterSurface.clear();
    for(int i = 0; i < amt; i++) {
        Vector2f position(mPosition.x + i * width, mPosition.y);
        mWaterSurface.push_back(WaterSpring(position, height, width));
    }
    mSpringHeight = height;
    mSpringWidth = width;
    mPosition = position;
}
void WaterSurface::SetSpringWidth(float width) {
    ReinitSurface(mPosition, mSpringHeight, width);
}
void WaterSurface::SetSpringHeight(float height) {
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        mWaterSurface[i].SetTargetHeight(height);
    }
}
void WaterSurface::SetPosition(Vector2f position) {
    mPosition = position;
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        mWaterSurface[i].position = Vector2f(position.x + i * mSpringWidth, position.y);
    }
}
void WaterSurface::Update() {
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        WaterSpring& surface = mWaterSurface[i];
        if(i > 0) {
            //surface.Pull(mWaterSurface[i - 1], mSpread);
            mWaterSurface[i - 1].Pull(surface, mSpread);
        }
        if(i < mWaterSurface.size() - 1) {
            mWaterSurface[i + 1].Pull(surface, mSpread);
            //surface.Pull(mWaterSurface[i + 1], mSpread);
        }
    }

    for(auto& surface : mWaterSurface) {
        surface.Update(mSpringConstant, mDamping);
    }
    UpdatePolygon();
}
void WaterSurface::Splash(Rectanglef hitbox, float force) {
    /*
    int leftMostCol = -1;
    int rightMostCol = mWaterSurface.size();
    for(int i = 0; i < mWaterSurface.size(); i++) {
        Vector2f position = mPosition;
        position.x+= i * mSpringWidth;
        Rectanglef hit(position, mWaterSurface[i].height, mSpringWidth);
        if(RectCollision(hitbox, hit)) {
            if(force < 0) {
                mWaterSurface[i].height+= force * 0.95f;
            }
            else {
                mWaterSurface[i].height+= force * 1.25;
            }
            if(mWaterSurface[i].height < 0) {
                mWaterSurface[i].height = 0;
            }
            if(i < rightMostCol) {
                rightMostCol = i;
            }
            if(i > leftMostCol) {
                leftMostCol = i;
            }
        }
    }
    if(leftMostCol >= 0) {
        mWaterSurface[leftMostCol].height-= force * 1.5f;
    }
    if(rightMostCol < mWaterSurface.size()) {
        mWaterSurface[rightMostCol].height-= force * 1.5;
    }
    */
    if(CheckCollision(&GetPolygon(), &hitbox)) {
        int i = (hitbox.GetCenter().x - mPosition.x) / mSpringWidth;
        if(i == 0) {
            i = 1;
        }
        else if(i >= mWaterSurface.size() -1) {
            i = mWaterSurface.size() - 1;
        }
        mWaterSurface[i].speed = force;
        mWaterSurface[i + 1].speed = force / 2;
        mWaterSurface[i - 1].speed = force / 2;
        UpdatePolygon();
    }
}
void WaterSurface::Ripple(float force, int i) {
    //int i = rand() % (mWaterSurface.size() - 1);
    if(i == 0) {
        i = 1;
    }
    if(i >= mWaterSurface.size() -1) {
        i = mWaterSurface.size() - 1;
    }

    mWaterSurface[i - 1].speed = force / 2;
    mWaterSurface[i].speed = force;
    mWaterSurface[i + 1].speed = force / 2;

    //mWaterSurface[i - 1].speed = force;
    //mWaterSurface[i + 1].speed = force;
    //i = mWaterSurface.size();
    //mWaterSurface[0].height-= force;
    //mWaterSurface[1].height-= force;
    //mWaterSurface[2].height-= force;

    //mWaterSurface[i + 1].height-= force;
    //mWaterSurface[i - 1].height-= force;
}
Polygonf WaterSurface::GetSurfacePoly() {
    Polygonf poly;
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        poly.AddVertex(Vector2f(mWaterSurface[i].position.x, mWaterSurface[i].position.y + mWaterSurface[i].height));
    }
    return poly;
}
vector<Rectanglef> WaterSurface::GetRects(Vector2f origin) {
    vector<Rectanglef> rects;
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        rects.push_back(Rectanglef(Vector2f(mWaterSurface[i].position.x + origin.x,
                        mWaterSurface[i].position.y + origin.y),
                        mWaterSurface[i].height,
                        mWaterSurface[i].width));
    }
    return rects;
}
void WaterSurface::GeneratePolygon() {
    mPolygon.Clear();
    float width = mSpringWidth * mWaterSurface.size();
    mPolygon.AddVertex(Vector2f(mPosition));
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        mPolygon.AddVertex(Vector2f(mWaterSurface[i].position.x, mWaterSurface[i].position.y + mWaterSurface[i].height));
    }
    mPolygon.AddVertex(Vector2f(mPosition.x + width, mPolygon.GetVertex(mPolygon.GetNumVert() - 1).y));
    mPolygon.AddVertex(Vector2f(mPosition.x + width, mPosition.y));
}
void WaterSurface::UpdatePolygon() {
    float width = mSpringWidth * mWaterSurface.size();
    mPolygon.GetVertex(0) = Vector2f(mPosition);
    for(size_t i = 0; i < mWaterSurface.size(); i++) {
        mPolygon.GetVertex(i + 1).y = mWaterSurface[i].position.y + mWaterSurface[i].height;
        mPolygon.GetVertex(i + 1).x = mWaterSurface[i].position.x;
    }
    mPolygon.GetVertex(mPolygon.GetNumVert() - 2) = Vector2f(mPosition.x + width, mPolygon.GetVertex(mPolygon.GetNumVert() - 3).y);
    mPolygon.GetVertex(mPolygon.GetNumVert() - 1) = Vector2f(mPosition.x + width, mPosition.y);
}
Polygonf& WaterSurface::GetPolygon() {
    /*
    Polygonf poly;
    float width = mSpringWidth * mWaterSurface.size();
    poly.AddVertex(Vector2f(mPosition.x + width / 2, mPosition.y));
    poly.AddVertex(Vector2f(mPosition));
    for(int i = 0; i < mWaterSurface.size(); i++) {
        poly.AddVertex(Vector2f(mWaterSurface[i].position.x, mWaterSurface[i].position.y + mWaterSurface[i].height));
    }
    poly.AddVertex(Vector2f(mPosition.x + width, poly.GetVertex(poly.GetNumVert() - 1).y));
    poly.AddVertex(Vector2f(mPosition.x + width, mPosition.y));
    //poly.SetCenter(Vector2f(mPosition.x + width / 2, mPosition.y));
    return poly;
    */
    return mPolygon;
}
}

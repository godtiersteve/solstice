#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "utility/util.h"
#include "globals.h"
#include "graphics/vector2.h"
#include "graphics/graphicglobals.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

namespace solstice {
enum VertexPositions{VT_BOTLEFT = 0, VT_TOPLEFT = 1, VT_TOPRIGHT = 2, VT_BOTRIGHT = 3};
template <class T> class Rectangle : public IShape<T>
{
protected:
    Vector2<T> mCenter;
    T mH, mW;
    DrawPoint mDrawFrom;
    void Rotate();
public:
    Rectangle();
    Rectangle(Vector2<T> origin, T h, T w);
    Rectangle(Vector2<T> origin, T h, T w, DrawPoint drawFrom);
    Rectangle(const Rectangle<T>& rect);
    Rectangle& Copy(const Rectangle<T>& rect);
    Rectangle& operator=(const Rectangle<T>& rect);
    void AddX(T x);
    void AddY(T y);
    void AddXY(T x, T y);
    void AddXY(Vector2<T> xy);

    void Init(Vector2<T> pos, T h, T w, DrawPoint drawFrom, float scale, float angle);
    void Init(Vector2<T> pos, T h, T w, DrawPoint drawFrom = DRAW_BOTLEFT);
    void Init(Vector2<T> topLeft, Vector2<T> topRight, Vector2<T> botLeft, Vector2<T> botRight);

    void SetHeight(T h);
    void SetWidth(T w);
    void SetHW(T h, T w);
    void SetX(T x);
    void SetY(T y);
    void SetXY(Vector2<T> point);
    void SetPosition(Vector2<T> pos)        {SetXY(pos);}
    void SetXYHW(T x, T y, T h, T w);
    void SetXYHW(Vector2<T> point, T h, T w);
    void SetDrawFrom(DrawPoint drawFrom);
    void SetAngle(float a);
    void SetScale(float scale);
    void SetSize(float size) {SetScale(size);}
    void SetBottom(T b);
    void SetTop(T t);
    void SetLeft(T l);
    void SetRight(T r);

    T GetLeft() const;
    T GetRight() const;
    T GetTop() const;
    T GetBottom() const;
    T GetHeight() const;
    T GetWidth() const;
    T GetArea() const;
    Vector2<T> GetHeightAxis() const;
    Vector2<T> GetWidthAxis() const;
    T GetX() const;
    T GetY() const;
    T GetMinLength() const {return (mH + mW) * 0.25f;}
    DrawPoint GetDrawFrom() const;
    Vector2<T> GetPosition() const {return GetOrigin();}
    Vector2<T> GetOrigin() const;
    Vector2<T> GetTopRight() const;
    Vector2<T> GetTopLeft() const;
    Vector2<T> GetBotRight() const;
    Vector2<T> GetBotLeft() const;
    Vector2<T> GetCenter() const;
    Vector2<T> GetTopCenter() const;
    Vector2<T> GetBotCenter() const;
    Vector2<T> GetLeftCenter() const;
    Vector2<T> GetRightCenter() const;
    Rectangle<T> Normalize(float xNorm, float yNorm);
    Rectangle<T> Normalize(Vector2<T> position, float xNorm, float yNorm);
    Rectangle<T> GetAABB() const ;

    Vector2<T> GetProjection(Vector2<T> axis);
    vector<Vector2<T> > GetAxes();
    vector<Vector2<T> > GetVertices() {return this->mVerts;}
    Vector2<T> GetClosestVertex(Vector2<T> point);
    void AdjustVertices();

    void RotateAboutOrigin(float angle);
    void RotateAboutPoint(float angle, Vector2f pt);

    void SaveToText(const char* file);
    void LoadFromText(const char* file);
    void SaveToText(ofstream& os);
    void LoadFromText(ifstream& is);
    void ToArray(T* arr);

    void Print();
    void Write(ofstream& file);
    void Read(ifstream& file);
};
typedef Rectangle<float> Rectanglef;
typedef Rectangle<int> Rectanglei;
typedef Rectangle<double> Rectangled;

template<class T>
Rectangle<T>::Rectangle() {
    this->mVerts = std::vector<Vector2<T> >(4);
    this->mSize = 1.0f;
    this->mAngle = 0.0f;
    this->mType = SHAPE_RECT;
    Init(0, 32, 32);
}
template<class T>
Rectangle<T>::Rectangle(Vector2<T> origin, T h, T w) {
    this->mSize = 1.0f;
    this->mAngle = 0.0f;
    this->mType = SHAPE_RECT;
    this->mVerts = std::vector<Vector2<T> >(4);
    Init(origin, h, w);
}
template<class T>
Rectangle<T>::Rectangle(Vector2<T> origin, T h, T w, DrawPoint drawFrom)
{
    this->mSize = 1.0f;
    this->mAngle = 0.0f;
    this->mType = SHAPE_RECT;
    this->mVerts = std::vector<Vector2<T> >(4);
    Init(origin, h, w, drawFrom);
}
template<class T>
void Rectangle<T>::Init(Vector2<T> pos, T h, T w, DrawPoint drawFrom) {
    Init(pos, h, w, drawFrom, 1, 0);
}
template<class T>
void Rectangle<T>::Init(Vector2<T> pos, T h, T w, DrawPoint drawFrom, float scale, float angle) {
    mH = h;
    mW = w;
    mDrawFrom = drawFrom;
    this->mAngle = angle;
    this->mPosition = pos;
    this->mSize = scale;
    AdjustVertices();
}
template<class T>
void Rectangle<T>::Init(Vector2<T> topLeft, Vector2<T> topRight, Vector2<T> botRight, Vector2<T> botLeft) {
    mH = topRight.y - botRight.y;
    mW = topRight.x - topLeft.x;
    mDrawFrom = DRAW_BOTLEFT;
    this->mAngle = 0;
    this->mPosition = botLeft;
    this->mSize = 1;
    this->mVerts[VT_TOPLEFT] = topLeft;
    this->mVerts[VT_TOPRIGHT] = topRight;
    this->mVerts[VT_BOTRIGHT] = botRight;
    this->mVerts[VT_BOTLEFT] = botLeft;
    mCenter.x = this->mVerts[VT_TOPLEFT].x + mW / 2;
    mCenter.y = this->mVerts[VT_TOPLEFT].y - mH / 2;
}
template<class T>
Rectangle<T>& Rectangle<T>::operator=(const Rectangle<T>& rect) {
    return Copy(rect);
}
template<class T>
Rectangle<T>::Rectangle(const Rectangle<T>& rect) {
    Copy(rect);
}
template<class T>
Rectangle<T>& Rectangle<T>::Copy(const Rectangle<T>& rect) {
    if(this->mVerts.empty()) {
        this->mVerts = std::vector<Vector2<T> >(4);
    }
    this->mW = rect.mW;
    this->mH = rect.mH;
    this->mCenter = rect.mCenter;
    this->mAngle = rect.mAngle;
    this->mType = rect.mType;
    this->mPosition = rect.mPosition;
    this->mSize = rect.mSize;
    this->mDrawFrom = rect.mDrawFrom;
    for(size_t i = 0; i < 4; i++) {
        this->mVerts[i] = rect.mVerts[i];
    }
    return *this;
}
template<class T>
void Rectangle<T>::AddX(T x) {
    this->mPosition.x+= x;
    this->mVerts[VT_TOPLEFT].x+= x;
    this->mVerts[VT_TOPRIGHT].x+= x;
    this->mVerts[VT_BOTLEFT].x+= x;
    this->mVerts[VT_BOTRIGHT].x+= x;
    mCenter.x+= x;
}
template<class T>
void Rectangle<T>::AddY(T y)   {
    this->mPosition.y+= y;
    this->mVerts[VT_TOPLEFT].y+= y;
    this->mVerts[VT_TOPRIGHT].y+= y;
    this->mVerts[VT_BOTLEFT].y+= y;
    this->mVerts[VT_BOTRIGHT].y+= y;
    mCenter.y+= y;
}
template<class T>
void Rectangle<T>::AddXY(T x, T y) 		{
    AddX(Trunc(x, 3));
    AddY(Trunc(y, 3));
}
template<class T>
void Rectangle<T>::AddXY(Vector2<T> xy) 			{
    AddX(xy.x);
    AddY(xy.y);
}

template<class T>
void Rectangle<T>::SetHeight(T h) {if(h != mH) {mH = h; AdjustVertices();}}
template<class T>
void Rectangle<T>::SetWidth(T w) {if(w != mW) {mW = w; AdjustVertices();}}
template<class T>
void Rectangle<T>::SetHW(T h, T w) {mH = h; mW = w; AdjustVertices();}
template<class T>
void Rectangle<T>::SetX(T x) {
    T diff = x - this->mPosition.x;
    AddX(diff);
}
template<class T>
void Rectangle<T>::SetY(T y) {
    T diff = y - this->mPosition.y;
    AddY(diff);
}
template<class T>
void Rectangle<T>::SetXY(Vector2<T> point) {
    SetX(point.x);
    SetY(point.y);
}
template<class T>
void Rectangle<T>::SetXYHW(T x, T y, T h, T w) {
    this->mPosition.x = x;
    this->mPosition.y = y;
    mH = h;
    mW = w;
    AdjustVertices();
}
template<class T>
void Rectangle<T>::SetXYHW(Vector2<T> point, T h, T w) {
    this->mPosition = point;
    mH = h;
    mW = w;
    AdjustVertices();
}
template<class T>
void Rectangle<T>::SetDrawFrom(DrawPoint drawFrom) {mDrawFrom = drawFrom; AdjustVertices();}
template<class T>
void Rectangle<T>::SetScale(float scale) {this->mSize = scale; AdjustVertices();}

template<class T>
T Rectangle<T>::GetLeft()  const  {
    if(!this->mAngle) {
        return this->mVerts[VT_TOPLEFT].x;
    }
    return std::min(this->mVerts[0].x, std::min(this->mVerts[1].x, std::min(this->mVerts[2].x, this->mVerts[3].x)));
}
template<class T>
T Rectangle<T>::GetRight() const  {
    if(!this->mAngle) {
        return this->mVerts[VT_TOPRIGHT].x;
    }
    return std::max(this->mVerts[0].x, std::max(this->mVerts[1].x, std::max(this->mVerts[2].x, this->mVerts[3].x)));
}
template<class T>
T Rectangle<T>::GetTop() const {
    if(!this->mAngle) {
        return this->mVerts[VT_TOPRIGHT].y;
    }
    return std::max(this->mVerts[0].y, std::max(this->mVerts[1].y, std::max(this->mVerts[2].y, this->mVerts[3].y)));
}
template<class T>
T Rectangle<T>::GetBottom() const              {
    if(!this->mAngle) {
        return this->mVerts[VT_BOTRIGHT].y;
    }
    return std::min(this->mVerts[0].y, std::min(this->mVerts[1].y, std::min(this->mVerts[2].y, this->mVerts[3].y)));
}
template<class T>
Rectangle<T> Rectangle<T>::GetAABB() const {
    if(!this->mAngle) {
        return *this;
    }
    else {
        return Rectangle<T>(GetCenter(), GetTop() - GetBottom(), GetRight() - GetLeft(), DRAW_CENTER);
    }
}
template<class T>
T Rectangle<T>::GetArea() const              {return mW * mH;}
template<class T>
T Rectangle<T>::GetHeight() const            {return mH;}
template<class T>
T Rectangle<T>::GetWidth() const            {return mW;}
template<class T>
T Rectangle<T>::GetX()     const               {return this->mPosition.x;}
template<class T>
T Rectangle<T>::GetY()  const                  {return this->mPosition.y;}
template<class T>
DrawPoint Rectangle<T>::GetDrawFrom() const        {return mDrawFrom;}
template<class T>
Vector2<T> Rectangle<T>::GetOrigin() const            {return this->mPosition;}
template<class T>
Vector2<T> Rectangle<T>::GetTopRight()  const         {return this->mVerts[VT_TOPRIGHT];}
template<class T>
Vector2<T> Rectangle<T>::GetTopLeft()  const         {return this->mVerts[VT_TOPLEFT];}
template<class T>
Vector2<T> Rectangle<T>::GetBotRight() const         {return this->mVerts[VT_BOTRIGHT];}
template<class T>
Vector2<T> Rectangle<T>::GetBotLeft() const          {return this->mVerts[VT_BOTLEFT];}
template<class T>
Vector2<T> Rectangle<T>::GetCenter()   const         {return mCenter;}
template<class T>
Vector2<T> Rectangle<T>::GetBotCenter()   const         {
    return Vector2f(GetLeft() + mW / 2, GetBottom());
    Vector2<T> ret = this->mVerts[VT_BOTRIGHT] - this->mVerts[VT_BOTLEFT];
    ret.x *= 0.5;
    ret.y *= 0.5;
    return this->mVerts[VT_BOTRIGHT] + ret;
}
template<class T>
Vector2<T> Rectangle<T>::GetTopCenter()   const   {
    Vector2<T> ret = this->mVerts[VT_TOPRIGHT] - this->mVerts[VT_TOPLEFT];
    ret.x *= 0.5;
    ret.y *= 0.5;
    return this->mVerts[VT_TOPRIGHT] + ret;
}
template<class T>
Vector2<T> Rectangle<T>::GetLeftCenter()   const {
    Vector2<T> ret = this->mVerts[VT_TOPLEFT] - this->mVerts[VT_BOTLEFT];
    ret.x *= 0.5;
    ret.y *= 0.5;
    return this->mVerts[VT_BOTLEFT] + ret;
}
template<class T>
Vector2<T> Rectangle<T>::GetRightCenter()   const         {
    return Vector2f(GetRight(), GetBottom() + mH / 2);
    Vector2<T> ret = this->mVerts[VT_TOPRIGHT] - this->mVerts[VT_BOTRIGHT];
    ret.x *= 0.5;
    ret.y *= 0.5;
    return this->mVerts[VT_BOTRIGHT] + ret;
}
template<class T>
Vector2<T> Rectangle<T>::GetHeightAxis() const {
    return this->mVerts[VT_TOPRIGHT] - this->mVerts[VT_BOTRIGHT];
}
template<class T>
Vector2<T> Rectangle<T>::GetWidthAxis() const {
    return this->mVerts[VT_TOPRIGHT] - this->mVerts[VT_TOPLEFT];
}
template<class T>
void Rectangle<T>::SetBottom(T b) {
    float move = GetBottom() - b;
    AddY(-move);
}
template<class T>
void Rectangle<T>::SetTop(T t) {
    float move = GetTop() - t;
    AddY(-move);
}
template<class T>
void Rectangle<T>::SetLeft(T l) {
    float move = l - GetLeft();
    AddY(move);
}
template<class T>
void Rectangle<T>::SetRight(T r) {
    float move = GetRight() - r;
    AddY(move);
}
template<class T>
void Rectangle<T>::AdjustVertices() {
    if(this->mVerts.size() != 0) {
        this->mVerts.clear();
        this->mVerts = std::vector<Vector2<T> >(4);
    }
    T h = mH * this->mSize;
    T w = mW * this->mSize;
    T l, r, t, b;
    T x = this->mPosition.x, y = this->mPosition.y, centerX = 0, centerY = 0;
    switch(mDrawFrom) {
        case DRAW_TOPLEFT:
            l = x;
            r = x + w;
            t = y ;
            b = y  - h;
            centerX = x + (w / 2);
            centerY = y - (h / 2);
            break;
        case DRAW_TOPCENTER:
            l = x - (w / 2);
            r = x + (w / 2);
            t = y ;
            b = y  - h;
            centerX = x;
            centerY = y - (h / 2);
            break;
        case DRAW_TOPRIGHT:
            l = x - w;
            r = x;
            t = y ;
            b = y  - h;
            centerX = x - (w / 2);
            centerY = y - (h / 2);
            break;
        case DRAW_RIGHTCENTER:
            l = x - w;
            r = x;
            t = y  + (h / 2);
            b = y  - (h / 2);
            centerX = x - (w / 2);
            centerY = y;
            break;
        case DRAW_BOTRIGHT:
            l = x - w;
            r = x;
            t = y  + h;
            b = y;
            centerX = x - (w / 2);
            centerY = y + (h / 2);
            break;
        case DRAW_BOTCENTER:
            l = x - (w / 2);
            r = x + (w / 2);
            t = y  + h;
            b = y;
            centerX = x;
            centerY = y + (h / 2);
            break;
        case DRAW_LEFTCENTER:
            l = x;
            r = x + w;
            t = y  + (h / 2);
            b = y  - (h / 2);
            centerX = x + (w / 2);
            centerY = y;
            break;
        case DRAW_CENTER:
            l = x - (w / 2);
            r = x + (w / 2);
            t = y  + (h / 2);
            b = y  - (h / 2);
            centerX = x;
            centerY = y;
            break;
        default:
            l = x;
            r = x + w;
            t = y  + h;
            b = y;
            centerX = x + (w / 2);
            centerY = y + (h / 2);
            break;
    }

    this->mVerts[VT_TOPLEFT]        = Vector2<T>(l, t);
    this->mVerts[VT_TOPRIGHT]       = Vector2<T>(r, t);
    this->mVerts[VT_BOTRIGHT]       = Vector2<T>(r, b);
    this->mVerts[VT_BOTLEFT]        = Vector2<T>(l, b);
    mCenter                         = Vector2<T>(centerX, centerY);
    //Rotate();
    float angle = this->mAngle;
    this->mAngle = 0;
    RotateAboutOrigin(angle);
}
template<class T>
void Rectangle<T>::SetAngle(float a) {
    if(a || this->mAngle) {
        float amt = a - this->mAngle;
        this->mAngle = a;
        Vector2f origin = GetCenter();
        float cosa = cos(amt);
        float sina = sin(amt);
        for(auto& vt : this->mVerts) {
            float xd = vt.x - origin.x;
            float yd = vt.y - origin.y;
            vt.x = xd * cosa - yd * sina + origin.x;
            vt.y = xd * sina + yd * cosa + origin.y;
        }
        float xd = this->mCenter.x - origin.x;
        float yd = this->mCenter.y - origin.y;
        this->mCenter.x = xd * cosa - yd * sina + origin.x;
        this->mCenter.y = xd * sina + yd * cosa + origin.y;
    }
}
template<class T>
void Rectangle<T>::RotateAboutOrigin(float a) {
    RotateAboutPoint(a, GetPosition());
}
template<class T>
void Rectangle<T>::RotateAboutPoint(float a, Vector2f pt) {
    if(a || this->mAngle) {
        float amt = a - this->mAngle;
        this->mAngle = a;
        float cosa = cos(amt);
        float sina = sin(amt);
        for(auto& vt : this->mVerts) {
            float xd = vt.x - pt.x;
            float yd = vt.y - pt.y;
            vt.x = xd * cosa - yd * sina + pt.x;
            vt.y = xd * sina + yd * cosa + pt.y;
        }
        float xd = this->mCenter.x - pt.x;
        float yd = this->mCenter.y - pt.y;
        this->mCenter.x = xd * cosa - yd * sina + pt.x;
        this->mCenter.y = xd * sina + yd * cosa + pt.y;
    }
}
template<class T>
void Rectangle<T>::Rotate() {
    if(this->mAngle) {
        this->mVerts[VT_TOPLEFT].Rotate(this->mAngle, mCenter);
        this->mVerts[VT_TOPRIGHT].Rotate(this->mAngle, mCenter);
        this->mVerts[VT_BOTRIGHT].Rotate(this->mAngle, mCenter);
        this->mVerts[VT_BOTLEFT].Rotate(this->mAngle, mCenter);
    }
}
template<class T>
void Rectangle<T>::SaveToText(const char* file) {
    ofstream os(file);
    SaveToText(os);
    os.close();
}

template<class T>
void Rectangle<T>::LoadFromText(const char* file) {
    ifstream is(file);
    LoadFromText(is);
    is.close();
}

template<class T>
void Rectangle<T>::SaveToText(ofstream& os) {
    os << mH << ' ' << mW << ' ';
    os << ' ';
    os << this->mSize << ' ' << this->mAngle << ' ';
    int drawFrom = mDrawFrom;
    os << drawFrom << ' ';
    this->mPosition.SaveToFile(os);
}
template<class T>
void Rectangle<T>::LoadFromText(ifstream& is) {
    T h, w, scale, angle;
    int drawPt;
    Vector2<T> xy;
    is >> h >> w;
    is >> scale >> angle;
    is >> drawPt;
    xy.LoadFromFile(is);
    SetXYHW(xy, h, w);
    SetDrawFrom((DrawPoint)drawPt);
    SetScale(scale);
    SetAngle(angle);
}
template<class T>
void Rectangle<T>::ToArray(T* arr) {
    arr[0] = this->mVerts[VT_TOPLEFT].x;
    arr[1] = this->mVerts[VT_TOPLEFT].y;

    arr[2] = this->mVerts[VT_TOPRIGHT].x;
    arr[3] = this->mVerts[VT_TOPRIGHT].y;

    arr[4] = this->mVerts[VT_BOTRIGHT].x;
    arr[5] = this->mVerts[VT_BOTRIGHT].y;

    arr[6] = this->mVerts[VT_BOTLEFT].x;
    arr[7] = this->mVerts[VT_BOTLEFT].y;
}
template<class T>
Vector2<T> Rectangle<T>::GetProjection(Vector2<T> axis) {
    T min = this->mVerts[0].Dot(axis);
    T max = min;
    for(size_t i = 1; i < this->mVerts.size(); i++) {
        Vector2<T>& vt = this->mVerts[i];
        T proj = axis.Dot(vt);
        if(proj > max) {
            max = proj;
        }
        if(proj < min) {
            min = proj;
        }
    }
    return Vector2<T>(min, max);
}
template<class T>
vector<Vector2<T> > Rectangle<T>::GetAxes() {
    vector<Vector2<T> > ret(2, 0);
    if(this->mAngle) {
        ret[0] = this->mVerts[VT_TOPRIGHT] - this->mVerts[VT_TOPLEFT];
        ret[0] = ret[0].GetUnit().GetNormal();
        ret[1] = this->mVerts[VT_TOPRIGHT] - this->mVerts[VT_BOTRIGHT];
        ret[1] = ret[1].GetUnit().GetNormal();
    }
    else {
        ret[0] = Vector2<T>(0, 1);
        ret[1] = Vector2<T>(1, 0);
    }
    return ret;
}
template<class T>
Vector2<T> Rectangle<T>::GetClosestVertex(Vector2<T> point) {
    float dist = FLT_MAX;
    Vector2<T>* closest = NULL;
    for(auto& vt : this->mVerts) {
        float distance = vt.Distance(point);
        if(dist > distance) {
            dist = distance;
            closest = &vt;
        }
    }
    if(closest) {
        return *closest;
    }
    else {
        return Vector2<T>();
    }
}
template<class T>
Rectangle<T> Rectangle<T>::Normalize(float xNorm, float yNorm) {
    Rectangle<T> rect = *this;
    rect.SetXYHW(Vector2<T>(this->mPosition.x * xNorm,
                          this->mPosition.y * yNorm),
                          mH * yNorm,
                          mW * xNorm);
    return rect;
}
template<class T>
Rectangle<T> Rectangle<T>::Normalize(Vector2<T> position, float xNorm, float yNorm) {
    Rectangle<T> rect = *this;
    rect.SetXYHW(Vector2f((position.x + this->mPosition.x) * xNorm, (position.y + this->mPosition.y) * yNorm),
                            mH * yNorm,
                            mW * xNorm);
    return rect;
}
template<class T>
void Rectangle<T>::Write(ofstream& file) {
    BinaryWrite(file, this->mPosition.x);
    BinaryWrite(file, this->mPosition.y);
    BinaryWrite(file, mH);
    BinaryWrite(file, mW);
    BinaryWrite(file, this->mAngle);
    BinaryWrite(file, this->mSize);
    int drawFrom = mDrawFrom;
    BinaryWrite(file, drawFrom);
}
template<class T>
void Rectangle<T>::Read(ifstream& file) {
    BinaryRead(file, this->mPosition.x);
    BinaryRead(file, this->mPosition.y);
    BinaryRead(file, mH);
    BinaryRead(file, mW);
    BinaryRead(file, this->mAngle);
    BinaryRead(file, this->mSize);
    int drawFrom;
    BinaryRead(file, drawFrom);
    mDrawFrom = static_cast<DrawPoint>(drawFrom);
    AdjustVertices();
}
template<class T>
void Rectangle<T>::Print() {
    std::cout << "Top right (X, Y): " << this->mVerts[VT_TOPRIGHT].x << ", " << this->mVerts[VT_TOPRIGHT].y << std::endl;
    std::cout << "Top left (X, Y): " << this->mVerts[VT_TOPLEFT].x << ", " << this->mVerts[VT_TOPLEFT].y << std::endl;
    std::cout << "Bottom left (X, Y): " << this->mVerts[VT_BOTLEFT].x << ", " << this->mVerts[VT_BOTLEFT].y << std::endl;
    std::cout << "Bottom Right (X, Y): " << this->mVerts[VT_BOTRIGHT].x << ", " << this->mVerts[VT_BOTRIGHT].y << "\n\n";
    std::cout << "Angle: " << this->mAngle << std::endl;
    std::cout << "Height, Width: " << mH << ", " << mW << std::endl;
}
}

#endif // RECTANGLE_H

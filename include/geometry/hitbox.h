#ifndef HITBOX_H_INCLUDED
#define HITBOX_H_INCLUDED

#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "polygon.h"
#include "graphics/color.h"
#include <memory>
#include <vector>

using std::vector;

namespace solstice {
template<class T>
class HitBox {
private:
    vector<std::shared_ptr<IShape<T> > > mHitBoxes;
    Vector2<T> mPosition;
    float mAngle;
    void Init(const HitBox<T>& hbox);
public:
    Color mColor;
    HitBox();
    HitBox(Rectangle<T> rect, Vector2<T> pos = 0);
    HitBox(Polygon<T> poly, Vector2<T> pos = 0);
    HitBox(Circle<T> circ, Vector2<T> pos = 0);
    HitBox(std::shared_ptr<IShape<T> > shape, Vector2<T> pos = 0);
    HitBox(const HitBox<T>& hbox);
    HitBox<T>& operator=(const HitBox<T>& hbox);
    ~HitBox();
    void Clear();
    HitBox<T>& operator=(Rectangle<T> r);
    HitBox<T>& operator=(Polygon<T> p);
    HitBox<T>& operator=(Circle<T> c);
    IShape<T>* operator()(size_t i = 0)           {return mHitBoxes[i].get();}
    std::shared_ptr<IShape<T>> GetHitBox(size_t i = 0)            {return mHitBoxes[i];}
    std::shared_ptr<IShape<T>> GetShape(size_t i = 0)      {return mHitBoxes[i];}
    std::shared_ptr<IShape<T>> Get()    {return mHitBoxes[0];}
    Rectangle<T>& GetRect(size_t i = 0)          {return *static_cast<Rectangle<T>* >(mHitBoxes[i].get());}
    Polygon<T>* GetPoly(size_t i = 0)            {return (Polygon<T>*)mHitBoxes[i];}
    Circle<T>* GetCircle(size_t i = 0)         {return (Circle<T>*)mHitBoxes[i];}
    IShape<T>* GetFront()                       {return mHitBoxes.front().get();}
    const vector<std::shared_ptr<IShape<T> > > GetHitBoxes() const     {return mHitBoxes;}
    void AddHitBox(std::shared_ptr<IShape<T> > s);
    void AddRect(Rectangle<T> rect)             {mHitBoxes.push_back(std::make_shared<Rectangle<T> >(rect));}
    void AddCircle(Circle<T> circ)              {mHitBoxes.push_back(std::make_shared<Circle<T> >(circ));}
    void AddPoly(Polygon<T> poly)               {mHitBoxes.push_back(std::make_shared<Polygon<T> >(poly));}
    void SetAngle(float a);
    void SetPosition(Vector2f pos);
    void AddX(T x);
    void AddY(T y);
    void AddXY(T x, T y);
    void AddXY(Vector2<T> xy);
    void SetX(T x);
    void SetY(T y);
    void SetXY(T x, T y);
    void SetXY(Vector2<T> xy);
    T GetX() {return mPosition.x;}
    T GetY() {return mPosition.y;}
    size_t Size() const {return mHitBoxes.size();}

    float GetAngle() {return mAngle;}
    Vector2f GetPosition() {return mPosition;}
    void Read(ifstream& file);
    void Write(ofstream& file);
};

typedef HitBox<float> HitBoxf;
typedef HitBox<int> HitBoxi;
typedef HitBox<double> HitBoxd;

template<class T>
HitBox<T>::HitBox() {
    mAngle = 0;
}
template<class T>
HitBox<T>::HitBox(Rectangle<T> rect, Vector2<T> pos) {
    mAngle = 0;
    *this = rect;
    SetPosition(pos);
}
template<class T>
HitBox<T>::HitBox(Polygon<T> poly, Vector2<T> pos) {
    mAngle = 0;
    *this = poly;
    SetPosition(pos);
}
template<class T>
HitBox<T>::HitBox(Circle<T> circ, Vector2<T> pos) {
    mAngle = 0;
    *this = circ;
    SetPosition(pos);
}
template<class T>
HitBox<T>::HitBox(std::shared_ptr<IShape<T> > shape, Vector2<T> pos) {
    mHitBoxes.push_back(shape);
    mPosition = pos;
    mAngle = 0;
}
template<class T>
HitBox<T>& HitBox<T>::operator=(const HitBox<T>& hbox) {
    Init(hbox);
    mColor = hbox.mColor;
    return *this;
}

template<class T>
HitBox<T>::HitBox(const HitBox<T>& hbox) {
    Init(hbox);
}
template<class T>
void HitBox<T>::Init(const HitBox<T>& hbox) {
    Clear();
    for(size_t i = 0; i < hbox.Size(); i++) {
        std::shared_ptr<IShapef> hb = hbox.mHitBoxes[i];
        switch(hb->GetType()) {
            case SHAPE_RECT:
                mHitBoxes.push_back(std::make_shared<Rectangle<T> >(*(Rectangle<T>*)hb.get()));
                break;
            case SHAPE_CIRCLE:
                mHitBoxes.push_back(std::make_shared<Circle<T> >(*(Circle<T>*)hb.get()));
                break;
            case SHAPE_POLY:
            default:
                mHitBoxes.push_back(std::make_shared<Polygon<T> >(*(Polygon<T>*)hb.get()));
                break;
        }

    }
    mAngle = hbox.mAngle;
    mPosition = hbox.mPosition;
}

template<class T>
void HitBox<T>::AddHitBox(std::shared_ptr<IShape<T> > s) {
    mHitBoxes.push_back(s);
}
template<class T>
HitBox<T>::~HitBox() {
    Clear();
}
template<class T>
void HitBox<T>::Clear() {
    mHitBoxes.clear();
}
template<class T>
HitBox<T>& HitBox<T>::operator=(Rectangle<T> r) {
    mHitBoxes.clear();
    AddRect(r);
    return *this;
}

template<class T>
HitBox<T>& HitBox<T>::operator=(Polygon<T> p) {
    mHitBoxes.clear();
    AddPoly(p);
    return *this;
}

template<class T>
HitBox<T>& HitBox<T>::operator=(Circle<T> c) {
    mHitBoxes.clear();
    AddCircle(c);
    return *this;
}
template<class T>
void HitBox<T>::SetAngle(float a) {
    for(auto& vt : mHitBoxes) {
        vt->SetAngle(a);
    }
}
template<class T>
void HitBox<T>::SetPosition(Vector2f pos) {
    Vector2<T> diff = pos -  mPosition;
    mPosition = pos;
    for(auto& vt : mHitBoxes) {
        //vt->AddXY(diff);
        vt->SetPosition(pos);
    }
}
template<class T>
void HitBox<T>::AddX(T x) {
    for(auto& vt : mHitBoxes) {
        vt->AddX(x);
    }
}
template<class T>
void HitBox<T>::AddY(T y) {
    for(auto& vt : mHitBoxes) {
        vt->AddY(y);
    }
}
template<class T>
void HitBox<T>::AddXY(T x, T y) {
    for(auto& vt : mHitBoxes) {
        vt->AddXY(x, y);
    }
}
template<class T>
void HitBox<T>::AddXY(Vector2<T> xy) {
    for(auto& vt : mHitBoxes) {
        vt->AddXY(xy);
    }
}

template<class T>
void HitBox<T>::SetX(T x) {
    for(auto& hb : mHitBoxes) {
        hb->SetX(x);
    }
}
template<class T>
void HitBox<T>::SetY(T y) {
    for(auto& hb : mHitBoxes) {
        hb->SetY(y);
    }
}
template<class T>
void HitBox<T>::SetXY(T x, T y) {
    SetPosition(x, y);
}
template<class T>
void HitBox<T>::SetXY(Vector2<T> xy) {
    SetPosition(xy);
}
template<class T>
void HitBox<T>::Read(ifstream& file) {
    mHitBoxes.clear();
    size_t num;
    BinaryRead(file, num);
    for(size_t i = 0; i < num; i++) {
        int type;
        BinaryRead(file, type);
        std::shared_ptr<IShape<T> > shape;
        switch(type) {
            case SHAPE_RECT:
                shape.reset(new Rectanglef());
                break;
            default:
                shape.reset(new Polygonf());
                break;
        }
        shape->Read(file);
        mHitBoxes.push_back(shape);
    }
}

template<class T>
void HitBox<T>::Write(ofstream& file) {
    BinaryWrite(file, mHitBoxes.size());
    for(auto h : mHitBoxes) {
        int type = h->GetType();
        BinaryWrite(file, type);
        h->Write(file);
    }
}
}

#endif // HITBOX_H_INCLUDED

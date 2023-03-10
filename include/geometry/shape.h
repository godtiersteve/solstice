#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include "graphics/vector2.h"
#include "edge.h"
#include "utility/util.h"
#include "engine/gameobject.h"
#include <vector>
#include <cfloat>
#include <fstream>

using std::vector;

namespace solstice {
template<class T> class Rectangle;
enum IShapeType{SHAPE_POLY, SHAPE_RECT, SHAPE_CIRCLE, SHAPE_POINT, SHAPE_QUAD};
template<class T> class IShape {
protected:
    vector<Vector2<T> > mVerts;
    float mAngle, mSize;
    Vector2<T> mPosition;
    IShapeType mType;
public:
    IShape();
    virtual void SetPosition(Vector2<T> pos) = 0;
    virtual void SetAngle(float angle) = 0;
    virtual void SetScale(float scale) = 0;
    virtual vector<Vector2<T> >  GetAxes() = 0;
    virtual Vector2<T> GetProjection(Vector2<T> axis) = 0;
    virtual void Read(std::ifstream& f);
    virtual void Write(std::ofstream& f);
    Edge<T> GetSeparatingEdge(Vector2f n);
    //Vector2<T> GetEdges(size_t i);
    //Vertex<T> GetAxes(size_t i);
    //virtual Vector2<T> GetClosestVertex(Vector2<T> point) = 0;
    Vector2<T> GetVertex(size_t i)      {return mVerts[i % mVerts.size()];}
    size_t GetNumVert() const           {return mVerts.size();}
    float GetSize() const               {return mSize;}
    float GetScale() const              {return mSize;}
    float GetAngle() const              {return mAngle;}
    Vector2<T> GetPosition() const      {return mPosition;}
    IShapeType GetType() const          {return mType;}
    virtual Rectangle<T> GetAABB() const       {return Rectangle<T>();}
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
};
typedef IShape<float> IShapef;
typedef IShape<int> IShapei;
typedef IShape<double> IShaped;
template<class T>
IShape<T>::IShape() {
    mAngle = 0;
    mSize = 0;
    mType = SHAPE_POLY;
}
template<class T>
void IShape<T>::AddX(T x) {
    if(!mVerts.empty()) {
        for(auto& vt : mVerts) {
            vt.x+= x;
        }
    }
    mPosition.x+=x;
}
template<class T>
void IShape<T>::AddY(T y) {
    if(!mVerts.empty()) {
        for(auto& vt : mVerts) {
            vt.y+= y;
        }
    }
    mPosition.y+=y;
}
template<class T>
void IShape<T>::AddXY(T x, T y) {
    if(!mVerts.empty()) {
        for(auto& vt : mVerts) {
            vt.x+= x;
            vt.y+= y;
        }
    }
    mPosition.x+=x;
    mPosition.y+=y;
}
template<class T>
void IShape<T>::AddXY(Vector2<T> xy) {
    if(!mVerts.empty()) {
        for(auto& vt : mVerts) {
            vt+= xy;
        }
    }
    mPosition+=xy;
}

template<class T>
void IShape<T>::SetX(T x) {
    T diff = mPosition.x - x;
    AddX(diff);
    mPosition.x = x;
}
template<class T>
void IShape<T>::SetY(T y) {
    T diff = mPosition.y - y;
    AddY(diff);
    mPosition.y = y;
}
template<class T>
void IShape<T>::SetXY(T x, T y) {
    T diff = mPosition.x - Vector2f(x, y);
    AddXY(diff);
    mPosition+= Vector2f(x, y);
}
template<class T>
void IShape<T>::SetXY(Vector2<T> xy) {
    Vector2<T> diff = mPosition - xy;
    AddXY(diff);
    mPosition+= xy;
}
template<class T>
void IShape<T>::Read(std::ifstream& f) {
    size_t numVerts;
    BinaryRead(f, numVerts);
    for(size_t i = 0; i < numVerts; i++) {
        Vector2<T> vt;
        vt.Read(f);
        mVerts.push_back(vt);
    }
    mPosition.Read(f);
    mType = SHAPE_POLY;
}
template<class T>
void IShape<T>::Write(std::ofstream& f) {
    BinaryWrite(f, mVerts.size());
    for(auto& vt : mVerts) {
        vt.Write(f);
    }
    mPosition.Write(f);

}
template<class T>
Edge<T> IShape<T>::GetSeparatingEdge(Vector2f n) {
    double max = FLT_MIN;
    int index = 0;
    for(int i = 0; i < mVerts.size(); i++) {
        double proj = n.Dot(mVerts[i]);
        if(proj > max) {
            max = proj;
            index = i;
        }
    }

    Vector2<T> v = mVerts[index];
    Vector2<T> v1 = GetVertex(index + 1);
    Vector2<T> v0 = index ? GetVertex(index - 1) : GetVertex(mVerts.size() - 1);
    Vector2<T> l = v - v1;
    Vector2<T> r = v - v0;
    l.Normalize();
    r.Normalize();
    if(r.Dot(n) <= l.Dot(n)) {
        return Edge<T>(v, v0, v);
    }
    else {
        return Edge<T>(v, v, v1);
    }
}
}


#endif // SHAPE_H_INCLUDED

#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include "shape.h"
#include "polygon.h"
namespace solstice {
template<class T>
class Circle : public IShape<T> {
public:
    Circle(Vector2<T> pos = 0, float size = 1);
    float GetRadius() {return this->mSize;}
    float GetDiameter() {return this->mSize * 2;}

    void SetPosition(Vector2<T> pos);
    void SetAngle(float angle);
    void SetScale(float scale);
    void SetSize(float size) {SetScale(size);}
    vector<Vector2<T> >  GetAxes();
    Vector2<T> GetProjection(Vector2<T> axis);
    Vector2<T> GetClosestPoint(Vector2<T> pt);
    Vector2<T> GetCenter() {return this->mPosition;}
    Polygon<T> ToPolygon(int vertices);
    void Read(std::ifstream& f);
    void Write(std::ofstream& f);
};
typedef Circle<float> Circlef;
typedef Circle<int> Circlei;
typedef Circle<double> Circled;
template<class T>
Circle<T>::Circle(Vector2<T> pos, float size) {
    this->mSize = size;
    this->mPosition = pos;
    this->mType = SHAPE_CIRCLE;
    this->mAngle = 0;
}
template<class T>
void Circle<T>::SetPosition(Vector2<T> pos) {
    this->mPosition = pos;
}
template<class T>
void Circle<T>::SetAngle(float angle) {
    this->mAngle = angle;
}
template<class T>
void Circle<T>::SetScale(float scale) {
    this->mSize = scale;
}
template<class T>
vector<Vector2<T> >  Circle<T>::GetAxes() {
    return vector<Vector2<T> >();
}
template<class T>
Vector2<T> Circle<T>::GetProjection(Vector2<T> axis) {
    Vector2f proj(-this->mSize, this->mSize);
    proj+= axis.Dot(this->mPosition);
    return proj;
}
template<class T>
Polygon<T> Circle<T>::ToPolygon(int vertices) {
    return Polygon<T>(this->mPosition, vertices, this->mSize);
}
template<class T>
Vector2<T> Circle<T>::GetClosestPoint(Vector2<T> pt) {
    Vector2f dist = pt - this->mPosition;
    Vector2f ret = this->mPosition + dist / dist.Magnitude() * this->mSize;
    return ret;
}
template<class T>
void Circle<T>::Read(std::ifstream& f) {
    //BinaryRead(f)
}

template<class T>
void Circle<T>::Write(std::ofstream& f) {
}
}

#endif // CIRCLE_H_INCLUDED

#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

#include <fstream>
#include <cmath>
#include "utility/sol_math.h"
#include "utility/util.h"

using std::ifstream;
using std::ofstream;

template<class T> class Vector2;
template<class T> Vector2<T> operator+(const Vector2<T> first, const Vector2<T> second);
template<class T> Vector2<T> operator-(const Vector2<T> f, const Vector2<T> s);
template<class T> Vector2<T> operator-(const Vector2<T> v, const T f) {
    return Vector2<T>(v.x - f, v.y - f);
}
template<class T> Vector2<T> operator+(const Vector2<T> v, const T f) {
    return Vector2<T>(v.x + f, v.y + f);
}
template<class T> Vector2<T> operator-(const T f, const Vector2<T> v) {
    return Vector2<T>(f - v.x, f - v.y);
}
template<class T> Vector2<T> operator+(const T f, const Vector2<T> v) {
    return Vector2<T>(v.x + f, v.y + f);
}
template<class T> bool operator==(Vector2<T> f, Vector2<T> s);
template<class T> bool operator!=(Vector2<T> first, Vector2<T> second);
template<class T> bool operator>(Vector2<T> vt, int num);
template<class T> Vector2<T> operator*(Vector2<T> v, T f);
template<class T> Vector2<T> operator*(T f, Vector2<T> v) {return v * f;}
template<class T> Vector2<T> operator*=(Vector2<T>& v, T f) {
    v.x*= f;
    v.y*= f;
    return v;
}
template<class T> T operator*(Vector2<T> first, Vector2<T> second);
template<class T> bool operator<(Vector2<T> first, Vector2<T> second);
template<class T> bool operator>(Vector2<T> first, Vector2<T> second);
template<class T> bool operator>=(Vector2<T> first, Vector2<T> second);
template<class T> bool operator<=(Vector2<T> first, Vector2<T> second);

template<class T>
class Vector2 {
public:
    T x, y;
    Vector2();
    Vector2(T pos):x(pos), y(pos) {}
    Vector2(T x, T y);
    void Rotate(float angle, Vector2<T> origin = 0);
    void Rotate(float angle, T radius, Vector2<T> origin);
    Vector2<T>& operator+=(Vector2<T> v);
    Vector2<T>& operator-=(Vector2<T> v);
    Vector2<T>& operator=(T f);
    Vector2<T>& operator=(Vector2<T> v);
    Vector2<T> operator-()  {return Vector2<T>(-x, -y);}
    operator Vector2<int>() const {return Vector2<int>(x, y);}
    operator Vector2<float>() const {return Vector2<float>(x, y);}
    operator glm::vec2() const {return glm::vec2(x, y);}
    Vector2<T> GetNormal();
    Vector2<T> GetNormal2();
    Vector2<T> GetUnit();
    Vector2<T> GetAbs();
    Vector2<T> Normalize() {*this = GetUnit(); return *this;}
    T Magnitude();
    T Magnitude2();
    T Length();
    T Length2();
    T Distance(Vector2<T> pt);
    T DistanceSqrt(Vector2<T> pt);
    T Dot(Vector2<T> pt);
    T Cross(Vector2<T> pt);
    Vector2<T> CrossScalar(T s);
    Vector2<T> Truncate(int digits);
    Vector2<T> Max(float xx, float yy);
    Vector2<T> Reflect() {*this = -(*this); return *this;}
    void Invert() {x = x != 0 ? 1.0 / x : x; y =  y != 0.0 ? 1.0 / y : y;}
    void SaveToFile(const char* filename);
    void LoadFromFile(const char* filename);
    void SaveToFile(ofstream& os);
    void Write(ofstream& os);
    void Read(ifstream& is);
    void LoadFromFile(ifstream& is);
    void Print();
    bool Empty() {return x != 0 && y != 0;}
    float Atan2()   {return atan2(y, x);}
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector2<double> Vector2d;
typedef Vector2<unsigned int> Vector2ui;

template<class T>
T Length(Vector2<T> v) {return v.x * v.x + v.y * v.y;}
template<class T>
T LengthSqrt(Vector2<T> v) {return sqrt(v.x * v.x + v.y * v.y);}


template<class T>
Vector2<T> VMax(Vector2<T> v1, Vector2<T> v2) {
    return Vector2<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
}
template<class T>
Vector2<T> VMin(Vector2<T> v1, Vector2<T> v2) {
    return Vector2<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
}

template<class T>
Vector2<T>::Vector2() {
    x = 0;
    y = 0;
}
template<class T>
Vector2<T>::Vector2(T x, T y)
{
    this->x = x;
    this->y = y;
}
template<class T>
Vector2<T> Vector2<T>::Max(float xx, float yy) {
    if(x > xx) {
        x = xx;
    }
    else if(x < -xx) {
        x = -xx;
    }
    if(y > yy) {
        y = yy;
    }
    else if(y < -yy) {
        y = yy;
    }
    return *this;
}
template<class T>
Vector2<T>& Vector2<T>::operator=(T f) {
    x = f;
    y = f;
    return *this;
}
template<class T>
Vector2<T>& Vector2<T>::operator=(Vector2<T> v) {
    x = v.x;
    y = v.y;
    return *this;
}

template<class T>
Vector2<T> operator+(const Vector2<T> first, const Vector2<T> second) {
    Vector2<T> foo;
    foo.x = first.x + second.x;
    foo.y = first.y + second.y;
    return foo;
}
template<class T>
Vector2<T> operator-(const Vector2<T> f, const Vector2<T> s) {
    return Vector2<T>(f.x - s.x, f.y - s.y);
}
template<class T>
Vector2<T>& Vector2<T>::operator+=(Vector2<T> v) {
    x+= v.x;
    y+= v.y;
    return *this;
}
template<class T>
Vector2<T>& Vector2<T>::operator-=(Vector2<T> v) {
    *this = *this - v;
    return *this;
}

template<class T>
bool operator<(Vector2<T> first, Vector2<T> second) {
    return (first.x * first.x + first.y * first.y) < (second.x * second.x + second.y * second.y);
}
template<class T>
bool operator>(Vector2<T> first, Vector2<T> second) {
    return (first.x * first.x + first.y * first.y) > (second.x * second.x + second.y * second.y);
}
template<class T>
bool operator>=(Vector2<T> first, Vector2<T> second) {
    return (first.x * first.x + first.y * first.y) >= (second.x * second.x + second.y * second.y);
}
template<class T>
bool operator<=(Vector2<T> first, Vector2<T> second) {
    return (first.x * first.x + first.y * first.y) <= (second.x * second.x + second.y * second.y);
}

template<class T>
Vector2<T> operator*(Vector2<T> v, T f) {
    v.x*=f;
    v.y*=f;
    return v;
}
template<class T>
Vector2<T> operator/(Vector2<T> vt, T f) {
    vt.x/=f;
    vt.y/=f;
    return vt;
}
template<class T>
bool operator!=(Vector2<T> first, Vector2<T> second) {
    int x1 = first.x;
    int x2 = second.x;
    int y1 = first.y;
    int y2 = second.y;
    return (x1 != x2 || y1 != y2);
}
template<class T>
bool operator==(Vector2<T> f, Vector2<T> s) {
    int x1 = f.x;
    int x2 = s.x;
    int y1 = f.y;
    int y2 = s.y;
    return (x1 == x2 && y1 == y2);
}

template<class T>
T operator*(Vector2<T> first, Vector2<T> second) {
    return first.x * second.x + first.y * second.y;
}
template<class T>
bool operator==(Vector2<T> vt, T f) {
    return ((int)vt.x == (int)f && (int)vt.y == (int)f);
}
template<class T>
bool operator>(Vector2<T> vt, int num) {
    return ((int)vt.x > num && (int)vt.y > num);
}
template<class T>
T Vector2<T>::Dot(Vector2<T> pt) {
    return *this * pt;
}
template<class T>
T Vector2<T>::Cross(Vector2<T> pt) {
    return this->x * pt.y - this->y * pt.x;
}
template<class T>
Vector2<T> Vector2<T>::CrossScalar(T s) {
    return Vector2<T>(s * y, -s * x);
}
template<class T>
void Vector2<T>::Rotate(float angle, Vector2<T> origin) {
    if(angle == 0) {
        return;
    }
    T x2, y2;
    T xd = x - origin.x;
    T yd = y - origin.y;
    x2 = xd * cos(angle) - yd * sin(angle);
    y2 = xd * sin(angle) + yd * cos(angle);
    x = x2 + origin.x;
    y = y2 + origin.y;
}
template<class T>
void Vector2<T>::Rotate(float angle, T radius, Vector2<T> origin) {
    T x2, y2;
    T xd = x - origin.x;
    T yd = y - origin.y;
    x2 = xd * cos(angle) - yd * sin(angle);
    y2 = xd * sin(angle) + yd * cos(angle);
    x = (radius * x2) + origin.x;
    y = (radius * y2) + origin.y;
    Truncate(3);
}
template<class T>
Vector2<T> Vector2<T>::GetNormal() {
    return Vector2<T>(-y, x);
}
template<class T>
Vector2<T> Vector2<T>::GetNormal2() {
    return Vector2<T>(y, -x);
}
template<class T>
Vector2<T> Vector2<T>::GetAbs() {
    return Vector2f(fabs(x), fabs(y));
}
template<class T>
Vector2<T> Vector2<T>::GetUnit() {
    double length = sqrt(x * x + y * y);
    if(length == 0) {
        return 0;
    }
    double xx = x / length;
    double yy = y / length;
    Vector2<T> ret(xx, yy);
    if(xx == 1.0 || xx == -1.0) {
        yy = 0.0;
    }
    else if(yy == 1.0 || yy == -1.0) {
        xx = 0.0f;
    }
    return Vector2<T>(xx, yy);
}
template<class T>
T Vector2<T>::Magnitude() {
    return sqrt(x * x + y * y);
}
template<class T>
T Vector2<T>::Magnitude2() {
    return x * x + y * y;
}
template<class T>
T Vector2<T>::Length() {
    return Magnitude();
}
template<class T>
T Vector2<T>::Length2() {
    return Magnitude2();
}
template<class T>
T Vector2<T>::Distance(Vector2<T> pt) {
    return (x - pt.x) * (x - pt.x) + (y - pt.y) * (y - pt.y);
}
template<class T>
T Vector2<T>::DistanceSqrt(Vector2<T> pt) {
    return sqrt(Distance(pt));
}
template<class T>
Vector2<T> Vector2<T>::Truncate(int digits) {
    x = solstice::Trunc(x, digits);
    y = solstice::Trunc(y, digits);
    return *this;
}
template<class T>
void Vector2<T>::SaveToFile(ofstream& os) {
    os << x << ' ' << y << ' ';
}
template<class T>
void Vector2<T>::LoadFromFile(ifstream& is) {
    is >> x >> y;
}
template<class T>
void Vector2<T>::Print() {
    std::cout << "x: " << x << ", y: " << y << std::endl;
}
template<class T>
void Vector2<T>::SaveToFile(const char* filename) {
    ofstream out(filename);
    SaveToFile(out);
    out.close();
}
template<class T>
void Vector2<T>::LoadFromFile(const char* filename) {
    ifstream is(filename);
    LoadFromFile(is);
    is.close();
}
template<class T>
void Vector2<T>::Write(ofstream& os) {
    solstice::BinaryWrite(os, x);
    solstice::BinaryWrite(os, y);
}
template<class T>
void Vector2<T>::Read(ifstream& is) {
    solstice::BinaryRead(is, x);
    solstice::BinaryRead(is, y);
}


#endif // VERTEX_H_INCLUDED

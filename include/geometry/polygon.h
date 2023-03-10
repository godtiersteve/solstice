#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <vector>
#include "rectangle.h"
#include "globals.h"
#include "graphics/vertexbuffer.h"
#include "shape.h"
// TODO (Stephen#1#): Ensure that vertices are always clockwise/counterclockwise in orientation (perhaps rename left/right to fit this). This ensures that deletions and insertions always happen in the same "place"

using std::vector;

namespace solstice {
template<class T> class Polygon : public IShape<T> {
    protected:
        void GetVertexArray(float* vertices);
        T mHeight, mWidth;
    public:
        Polygon();
        Polygon(Vector2<T> position, int numSides, int size);
        Polygon(vector<Vector2<T>>& vertices, Vector2<T> origin = 0);
        Polygon(Rectanglef rect);
        ~Polygon();
        Polygon(const Polygon& poly);
        Polygon& operator=(Rectanglef rect);
        Polygon& operator=(Polygon poly);

        void Init(Vector2<T> position, int numSides, int size);
        void Init(const vector<Vector2<T>>& vertices);
        void Clear()        {this->mVerts.clear();}
        void AddVertex(unsigned int i, Vector2<T> vertex);
        void AddVertex(Vector2<T> vt);
        void InsertAfterClosest(Vector2<T> vt);
        void InsertBeforeClosest(Vector2<T> vt);
        void InsertClosest(Vector2<T> vt);
        void RemoveVertex(unsigned int i);
        Vector2<T>& GetVertex(unsigned int i);
        int GetClosest(Vector2<T> pt);
        int GetNextClosest(Vector2<T> pt);

        vector<Vector2<T>>& GetVertices();

        void SetPosition(Vector2<T> center);
        void SetScale(float scale) {SetSize(scale);}
        void SetSize(float size);
        void SetAngle(float angle);
        Vector2<T>& GetLeft(size_t i);
        Vector2<T>& GetRight(size_t i);
        T GetHeight() {return mHeight;}
        T GetWidth()    {return mWidth;}

        void SubDivide();

        Vector2<T> GetProjection(Vector2<T> axis);
        vector<Vector2<T> > GetProjections(vector<Vector2<T> > axes);
        vector<Vector2<T> > GetAxes();
        std::vector<glm::vec2> GenerateTextureCoords(glm::vec2 origin, float height, float width);
        virtual Rectangle<T> GetAABB() const ;
        void Print();
};
typedef Polygon<float> Polygonf;
typedef Polygon<int> Polygoni;
typedef Polygon<double> Polygond;
template<class T>
Polygon<T>::Polygon() {
    this->mAngle = 0;
    Init(0, 0, 0);
    this->mType = SHAPE_POLY;
}
template<class T>
Polygon<T>::Polygon(Vector2<T> position, int numSides, int size) {
    this->mAngle = 0;
    this->mType = SHAPE_POLY;
    Init(position, numSides, size);
}
template<class T>
Polygon<T>::Polygon(vector<Vector2<T>>& vertices, Vector2<T> origin) {
    this->mAngle = 0;
    this->mType = SHAPE_POLY;
    this->mPosition = origin;
    Init(vertices);
}
template<class T>
Polygon<T>::Polygon(Rectanglef rect) {
    //*this = rect;
    this->mVerts = rect.GetVertices();
    this->mHeight = rect.GetHeight();
    this->mWidth = rect.GetWidth();
    this->mType = SHAPE_RECT;
    this->mPosition = rect.GetCenter();
}
template<class T>
Polygon<T>::Polygon(const Polygon& poly) {
    this->mPosition = poly.mPosition;
    this->mAngle = poly.mAngle;
    mHeight = poly.mHeight;
    mWidth = poly.mWidth;
    this->mVerts = poly.mVerts;
}
template<class T>
Polygon<T>::~Polygon() {
    this->mVerts.clear();
}
template<class T>
Polygon<T>& Polygon<T>::operator=(Polygon poly) {
    this->mPosition = poly.GetPosition();
    this->mVerts = poly.mVerts;
    this->mAngle = poly.GetAngle();
    this->mSize = poly.GetScale();
    mHeight = poly.mHeight;
    mWidth = poly.mWidth;
    return *this;
}
template<class T>
Polygon<T>& Polygon<T>::operator=(Rectanglef rect) {
    this->mVerts.clear();
    this->mSize = (rect.GetHeight() + rect.GetWidth()) / 2;
    this->mVerts.push_back(Vector2<T>(rect.GetWidth() / 2, rect.GetHeight() / 2));
    this->mVerts.push_back(Vector2<T>(-rect.GetWidth() / 2, rect.GetHeight() / 2));
    this->mVerts.push_back(Vector2<T>(-rect.GetWidth() / 2, -rect.GetHeight() / 2));
    this->mVerts.push_back(Vector2<T>(rect.GetWidth() / 2, -rect.GetHeight() / 2));

    SetPosition(rect.GetCenter());
    this->mAngle = rect.GetAngle();
    return *this;
}
template<class T>
void Polygon<T>::Init(Vector2<T> position, int numSides, int size) {
    this->mSize = size;
    this->mVerts.clear();
    this->mPosition = position;
    T left, right, top, bottom;
    left = position.x;
    right = position.x;
    top = position.y;
    bottom  = position.y;
    this->mVerts = std::vector<Vector2<T> >(numSides);
    double angleStep = TWO_PI / numSides;
    //angleStep = 0;
    T radius = size / 2.0;
    double angle = 0;
    for(int i = 0; i < numSides; i++) {
        double angleSin = sin(angle);
        double angleCos = cos(angle);
        angle+= angleStep;
        T x = radius * angleCos + position.x;
        T y = radius * angleSin + position.y;
        if(x < left) {
            left = x;
        }
        if(x > right) {
            right = x;
        }
        if(y < bottom) {
            bottom = y;
        }
        if(y > top) {
            top = y;
        }
        this->mVerts[i].x = x;
        this->mVerts[i].y = y;
    }
    mHeight = top - bottom;
    mWidth = right - left;
    //SetPosition(position);
}
template<class T>
void Polygon<T>::Init(const vector<Vector2<T>>& vertices) {
    if(vertices.size() <= 2) {
        return;
    }
    this->mVerts = vertices;
    T left, right, top, bottom;
    left = vertices[0].x;
    right = vertices[0].x;
    top = vertices[0].y;
    bottom = vertices[0].y;
    for(size_t i = 0; i < this->mVerts.size(); i++) {
        T x = this->mVerts[i].x;
        T y = this->mVerts[i].y;
        if(x < left) {
            left = x;
        }
        if(x > right) {
            right = x;
        }
        if(y < bottom) {
            bottom = y;
        }
        if(y > top) {
            top= y;
        }
    }
    mHeight = top - bottom;
    mWidth = right - left;
}
template<class T>
int Polygon<T>::GetClosest(Vector2<T> pt) {
    float dist = FLT_MAX;
    int j = 0;
    for(size_t i = 0; i < this->mVerts.size(); i++) {
        Vector2<T>& vt = this->mVerts[i];
        float d = vt.Distance(pt);
        if(d < dist) {
            dist = d;
            j = i;
        }
    }
    return j;
}
template<class T>
int Polygon<T>::GetNextClosest(Vector2<T> pt) {
    float dist = FLT_MAX;
    int j = 0;
    int k = 0;
    for(size_t i = 0; i < this->mVerts.size(); i++) {
        Vector2<T>& vt = this->mVerts[i];
        float d = vt.Distance(pt);
        if(d < dist) {
            dist = d;
            k = j;
            j = i;
        }
    }
    return k;
}
template<class T>
void Polygon<T>::AddVertex(unsigned int i, Vector2<T> vertex) {
    if(i >= this->mVerts.size()) {
        i = 0;
    }
    this->mVerts.insert(this->mVerts.begin() + i, vertex);
}
template<class T>
void Polygon<T>::AddVertex(Vector2<T> vt) {
    this->mVerts.push_back(vt);
}
template<class T>
void Polygon<T>::InsertAfterClosest(Vector2<T> vt) {
}
template<class T>
void Polygon<T>::InsertBeforeClosest(Vector2<T> vt) {
    int i = GetClosest(vt);
    if(i == this->mVerts.size() - 1) {
        i = 0;
    }
    else if(!i) {
        i = this->mVerts.size() - 1;
    }
    else {
        i--;
    }
    std::cout << i << std::endl;
    AddVertex(i, vt);
}
template<class T>
void Polygon<T>::InsertClosest(Vector2<T> vt) {
    int i = GetClosest(vt);
    int j = GetNextClosest(vt);
    std::cout << i << ", " << j << std::endl;
    if(j > i) {
        AddVertex(j, vt);
    }
    else {
        AddVertex(i, vt);
    }

}
template<class T>
void Polygon<T>::RemoveVertex(unsigned int i) {
    if(this->mVerts.size() - 1 < 3) {
        return;
    }
    this->mVerts.erase(this->mVerts.begin() + i);
}
template<class T>
Vector2<T>& Polygon<T>::GetVertex(unsigned int i)       {return this->mVerts[i];}

template<class T>
void Polygon<T>::SetPosition(Vector2<T> center) {
    Vector2<T> diff = center - this->mPosition;
    for(size_t i = 0; i < this->mVerts.size(); i++) {
        this->mVerts[i]+= diff;
    }
    this->mPosition = center;
}
template<class T>
void Polygon<T>::SetSize(float size)               {
    this->mSize = size;
    Init(this->mPosition, this->mVerts.size(), this->mSize);
}
template<class T>
vector<Vector2<T>>& Polygon<T>::GetVertices()   {return this->mVerts;}
template<class T>
void Polygon<T>::SetAngle(float angle) {
    if(angle > TWO_PI) {
        angle = angle - TWO_PI;
    }
    if(angle < -TWO_PI) {
        angle = angle + TWO_PI;
    }

    for(auto& vt : this->mVerts) {
        vt.Rotate(-this->mAngle + angle);
    }
    this->mAngle = angle;
}
template<class T>
void Polygon<T>::GetVertexArray(GLfloat* vertices) {
    if(this->mVerts.empty()) {
        vertices = NULL;
        return;
    }
    int i = 0;
    vertices[i++] = 0;
    vertices[i++] = 0;
    for(auto& vt : this->mVerts) {
        vertices[i++] = vt.x;
        vertices[i++] = vt.y;
    }
}
template<class T>
Vector2<T>& Polygon<T>::GetLeft(size_t i) {
    if(i == 0) {
        return this->mVerts.back();
    }
    else {
        return this->mVerts[i - 1];
    }
}
template<class T>
Vector2<T>& Polygon<T>::GetRight(size_t i) {
    if(i >= this->mVerts.size() -1) {
        return this->mVerts[0];
    }
    else {
        return this->mVerts[i + 1];
    }
}
template<class T>
std::vector<glm::vec2> Polygon<T>::GenerateTextureCoords(glm::vec2 origin, float height, float width) {
    std::vector<glm::vec2> texCoords(this->mVerts.size());
    glm::vec2 pos = this->mPosition;
    float angle = PI * 0.5;
    float angleStep = TWO_PI / (this->mVerts.size() - 2);
    float x = origin.x + (cos(angle) + 1) * 0.5f * width;
    float y = origin.y + (sin(angle) + 1) * 0.5f * height;
    texCoords.front().x = x;
    texCoords.front().y = y;
    for(size_t i = 1; i < texCoords.size(); i++) {
        texCoords[i].x = origin.x + (cos(i * angleStep + PI * 0.5) + 1.0) * 0.5f * width;
        texCoords[i].y = origin.y + (sin(i * angleStep + PI * 0.5) + 1.0) * 0.5f * height;
    }

    return texCoords;
}
template<class T>
void Polygon<T>::SubDivide() {
    vector<Vector2<T> > divisionVertices;
    for(int i = 0; i < this->mVerts.size(); i++) {
        float x, y, x2, y2;
        float leftAvg = 1.0f / 3.0, rightAvg = 2.0f / 3.0f;
        Vector2<T>& right = GetRight(i);
        Vector2<T>& left = GetLeft(i);
        Vector2<T>& vertex = this->mVerts[i];
        x = left.x * leftAvg + vertex.x * rightAvg;
        y = left.y * leftAvg + vertex.y * rightAvg;
        x2 = right.x * leftAvg + vertex.x * rightAvg;
        y2 = right.y * leftAvg + vertex.y * rightAvg;
        divisionVertices.push_back(Vector2<T>(x, y));
        divisionVertices.push_back(Vector2<T>(x2, y2));
    }
    this->mVerts.clear();
    Init(divisionVertices);
}
template<class T>
Vector2<T> Polygon<T>::GetProjection(Vector2<T> axis) {
    if(this->mVerts.empty()) {
        return 0;
    }
    T min = this->mVerts[0].Dot(axis);
    T max = min;
    for(size_t i = 0; i < this->mVerts.size(); i++) {
        T projection = axis.Dot(this->mVerts[i]);
        if(projection < min) {
            min = projection;
        }
        else if(projection > max) {
            max = projection;
        }
    }
    return Vector2<T>(min, max);
}
template<class T>
Rectangle<T> Polygon<T>::GetAABB() const {
    return Rectanglef(this->mPosition, mHeight, mWidth, DRAW_CENTER);
}
template<class T>
vector<Vector2<T> > Polygon<T>::GetProjections(vector<Vector2<T> > axes) {
    vector<Vector2<T> > projections;
    for(int i = 0; i < axes.size(); i++) {
        projections.push_back(GetProjection(axes[i]));
    }
    return projections;
}
template<class T>
vector<Vector2<T> > Polygon<T>::GetAxes() {
    vector<Vector2<T> > axes;
    size_t verts;
    if(this->mType == SHAPE_RECT) {
        verts = 2;
    }
    else {
        verts = this->mVerts.size();
    }
    axes.resize(verts);
    for(size_t i = 0; i < verts; i++) {
        Vector2<T> left = this->mVerts[i];
        Vector2<T> right = this->GetRight(i);
        Vector2<T> axis = left - right;
        axes[i] = axis.GetUnit().GetNormal();
    }
    return axes;
}
template<class T>
void Polygon<T>::Print() {
    for(size_t i = 0; i < this->mVerts.size(); i++) {
        std::cout << "Vertex " << i << " | x: " << this->mVerts[i].x << ", y: " << this->mVerts[i].y << std::endl;
        std::cout << "Left: " << GetLeft(i).x << ", " << GetLeft(i).y << std::endl;
        std::cout << "Right: " << GetRight(i).x << ", " << GetRight(i).y << std::endl;
    }
}
};

#endif // POLYGON_H_INCLUDED

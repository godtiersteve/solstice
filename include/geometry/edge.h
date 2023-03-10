#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

#include "graphics/vector2.h"

namespace solstice {
template<class T> struct Edge {
    Vector2<T> v1, v2, best;
    Edge() {}
    Edge(Vector2<T> b, Vector2<T> start, Vector2<T> end) {
        v1 = start;
        v2 = end;
        best = b;
    }
    Edge(Vector2<T> start, Vector2<T> end) {
        v1 = start;
        v2 = end;
    }
    operator Vector2<T>()       {return GetEdge();}
    Vector2<T> GetEdge()        {return v2 - v1;}
    T Dot(Vector2<T> v)         {return GetEdge().Dot(v);}
    T Dot(Edge<T> e)            {return GetEdge().Dot(e.GetEdge());}
    T Cross(Vector2<T> v)       {return GetEdge().Cross(v);}
    T Cross(Edge<T> e)          {return GetEdge().Cross(e.GetEdge());}
    T Length()                  {return GetEdge().Length();}
    T Length2()                 {return GetEdge().Length2();}
};
typedef Edge<float> Edgef;
typedef Edge<int> Edgei;
typedef Edge<double> Edged;

}

#endif // EDGE_H_INCLUDED

#include "collision.h"

namespace solstice {
Collision::Collision(float mintrans) {
    hasCollision = false;
    mtv = mintrans;
    normal = 0;
}
Collision::Collision(bool col) {
    hasCollision = col;
    normal = 0;
    mtv = 0;
}
Vector2f Collision::GetContactPoint(Rectanglef& rect1, Rectanglef& rect2) {
    GetEdges(rect1, rect2);
    Edgef ref, inc;
    bool flip = false;
    float e1Dot = e1.Dot(normal);
    float e2Dot = e2.Dot(normal);
    if((fabs(e1Dot) > 0.001f) && (fabs(e1Dot) <= fabs(e2Dot))) {
        ref = e1;
        inc = e2;
        minEdge = e1;
        flip = false;
    }
    else {
        ref = e2;
        inc = e1;
        minEdge = e1;
        flip = true;
    }
    Vector2f refv = ref.GetEdge().GetUnit();
    float o1 = refv.Dot(ref.v1);
    std::vector<Vector2f> clipped = Clip(inc.v1, inc.v2, refv, o1);
    if(clipped.size() < 2) {
        if(clipped.size() == 1) {
            contactPoint = clipped[0];
            clipEdge = Edgef(contactPoint, contactPoint);
        }
        else {
            contactPoint = ref.best;
            clipEdge = Edgef(contactPoint, contactPoint);
        }
    }
    else {
        float o2 = refv.Dot(ref.v2);
        clipped = Clip(clipped[0], clipped[1], -refv, -o2);
        if(clipped.size() >= 2) {
            Vector2f refNorm = refv.GetNormal();
            if(flip) {
                refNorm.Reflect();
            }
            float max = refNorm.Dot(ref.best);
            for(auto vt = clipped.begin(); vt != clipped.end(); ) {
                float dot = refNorm.Dot(*vt);
                if(dot - max < -0.1f) {
                    vt = clipped.erase(vt);
                }
                else {
                    vt++;
                }
            }
            if(clipped.empty()) {
                contactPoint = inc.best;
                clipEdge = Edgef(contactPoint, contactPoint);
            }
            else if(clipped.size() == 1) {
                contactPoint = clipped[0];
                clipEdge = Edgef(contactPoint, contactPoint);
            }
            else {
                contactPoint = (clipped[0] + clipped[1]) * 0.5f;
                clipEdge = Edgef(clipped[0], clipped[1]);
            }

        }
        else {
            contactPoint = ref.best;
            clipEdge = Edgef(contactPoint, contactPoint);
        }
    }
    return contactPoint;
}
std::vector<Vector2f> Clip(Vector2f v1, Vector2f v2, Vector2f n, float o) {
    std::vector<Vector2f> cp;
    float d1 = n.Dot(v1) - o;
    float d2 = n.Dot(v2) - o;
    if(d1 > 0.0f) {
        cp.push_back(v1);
    }
    if(d2 > 0.0f) {
        cp.push_back(v2);
    }
    if(d1 * d2 < 0.0) {
        Vector2f e = v2 - v1;
        float u = d1 / (d1 - d2);
        e = e * u + v1;
        cp.push_back(e);
    }
    return cp;
}
void Collision::GetEdges(Rectanglef& rect1, Rectanglef& rect2) {
    float min = normal.Dot(rect1.GetVertex(0));
    int index = 0;
    for(unsigned int i = 1; i < rect1.GetNumVert(); i++) {
        float proj = normal.Dot(rect1.GetVertex(i));
        if(proj < min) {
            min = proj;
            index = i;
        }
    }
    Vector2f v = rect1.GetVertex(index);
    Vector2f v1 = rect1.GetVertex(index + 1);
    Vector2f v0 = rect1.GetVertex(index - 1);
    Vector2f l = v - v1;
    Vector2f r = v - v0;
    l.Normalize();
    r.Normalize();
    float rDot = r.Dot(normal), lDot = l.Dot(normal);
    if(rDot < lDot) {
        e1 = Edgef(v, v, v1);
    }
    else {
        e1 = Edgef(v, v0, v);
    }

    min = normal.Dot(-rect2.GetVertex(0));
    index = 0;
    for(unsigned int i = 1; i < rect2.GetNumVert(); i++) {
        Vector2f vt = rect2.GetVertex(i);
        float proj = normal.Dot(-vt);
        if(proj < min) {
            min = proj;
            index = i;
        }
    }

    v = rect2.GetVertex(index);
    v1 = rect2.GetVertex(index + 1);
    v0 = index ? rect2.GetVertex(index - 1) : rect2.GetVertex(rect2.GetNumVert() - 1);
    l = v - v1;
    r = v - v0;
    l.Normalize();
    r.Normalize();
    if(r.Dot(-normal) <= l.Dot(-normal)) {
        e2 = Edgef(v, v, v1);
    }
    else {
        e2 = Edgef(v, v0, v);
    }
}
}

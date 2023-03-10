#include "hitdetection.h"
namespace solstice {
Collision CheckCollision(IShapef* shape1, IShapef* shape2) {
    if(!shape1 || !shape2) {
        return false;
    }
    if(shape1->GetType() == SHAPE_RECT && shape2->GetType() == SHAPE_RECT) {
        return RectCollision(*(Rectanglef*)shape1, *(Rectanglef*)shape2);
    }
    else {
        if(shape1->GetType() == SHAPE_RECT && shape2->GetType() == SHAPE_POLY) {
            Polygonf p = *(Rectanglef*)shape1;
            return PolyCollision(p, *(Polygonf*)shape2);
        }
        else if(shape2->GetType() == SHAPE_RECT && shape1->GetType() == SHAPE_POLY) {
            Polygonf p = *(Rectanglef*)shape2;
            return PolyCollision(p, *(Polygonf*)shape1);
        }
        else if(shape2->GetType() == SHAPE_RECT && shape1->GetType() == SHAPE_CIRCLE) {
            return CircleRectCollision(*(Circlef*)shape1, *(Rectanglef*)shape2);
        }
        else if(shape1->GetType() == SHAPE_RECT && shape2->GetType() == SHAPE_CIRCLE) {
            return CircleRectCollision(*(Circlef*)shape2, *(Rectanglef*)shape1);
        }
        else {
            return PolyCollision(*(Polygonf*)shape1, *(Polygonf*)shape2);
        }
    }
}
Collision RectCollision(Rectanglef& rect1, Rectanglef& rect2) {
    /*
    if(rect1.GetAngle() == 0.0f && rect2.GetAngle() == 0.0f) {
        return AABBCollision(rect1, rect2);
    }
    */
    return SeparatingAxis(rect1, rect2);

}
std::vector<Collision> CheckCollisionv(const HitBoxf& hbox1, const HitBoxf& hbox2) {
    std::vector<Collision> collisions;
    for(auto& hb : hbox1.GetHitBoxes()) {
        for(auto& hb2 : hbox1.GetHitBoxes()) {
            Collision c = CheckCollision(hb.get(), hb2.get());
            if(!c) {
                collisions.push_back(c);
            }
        }
    }
    return collisions;
}
Collision CheckCollision(const HitBoxf& hbox1, const HitBoxf& hbox2) {
    for(auto& hb : hbox1.GetHitBoxes()) {
        for(auto& hb2 : hbox2.GetHitBoxes()) {
            Collision col = CheckCollision(hb.get(), hb2.get());
            if(col) {
                return col;
            }
        }
    }
    return Collision();
}
Collision CheckCollision(IShapef* shape, const HitBoxf& hbox) {
    for(auto& hb : hbox.GetHitBoxes()) {
        Collision col = CheckCollision(shape, hb.get());
        if(col) {
            return col;
        }
    }
    return Collision();
}
Collision SeparatingAxis(Rectanglef& rect1, Rectanglef& rect2) {
    Collision c(FLT_MAX);
    float distance = FLT_MAX;
    float minDistance = FLT_MAX;
    std::vector<Vector2f> axes = rect1.GetAxes();
    std::vector<Vector2f> axes2 = rect2.GetAxes();

    for(size_t i = 0; i < axes.size(); i++) {
        Vector2f& axis = axes[i];
        if(!CheckProjection(rect1, rect2, axis, distance)) {
            c.mtv = 0;
            c.normal = 0;
            c.hasCollision = false;
            return c;
        }
        else if(fabs(distance) < fabs(minDistance)) {
            minDistance = distance;
            c.mtv = axis * distance;
            c.normal = axis;
            if(distance < 0) {
                c.normal = -c.normal;
            }
            c.hasCollision = true;
        }
    }
    for(size_t i = 0; i < axes2.size(); i++) {
        Vector2f& axis = axes2[i];
        if(!CheckProjection(rect1, rect2, axis, distance)) {
            c.mtv = 0;
            c.normal = 0;
            c.hasCollision = false;
            return c;
        }
        else if(fabs(distance) < fabs(minDistance)) {
            minDistance = distance;
            c.mtv = axis * distance;
            c.normal = axis;
            if(distance < 0) {
                c.normal = -c.normal;
            }
            c.hasCollision = true;
        }
    }
    return c;
}
void GetContactPoint(Collision& c, Rectanglef& rect1, Rectanglef& rect2) {
    c.GetEdges(rect1, rect2);
    Edgef ref, inc;
    bool flip = false;
    if(fabs(c.e1.Dot(c.normal)) <= fabs(c.e2.Dot(c.normal))) {
        ref = c.e1;
        inc = c.e2;
    }
    else {
        ref = c.e2;
        inc = c.e1;
        flip = true;
    }
    Vector2f refv = ref.GetEdge().GetUnit();
    float o1 = refv.Dot(ref.v1);
    std::vector<Vector2f> clipped = Clip(inc.v1, inc.v2, refv, o1);
    if(clipped.size() < 2) {
        c.contactPoint = ref.best;
    }
    else {
        float o2 = refv.Dot(ref.v2);
        c.clipEdge = Edgef(clipped[0], clipped[1]);
        clipped = Clip(clipped[0], clipped[1], -refv, -o2);
        if(clipped.size() >= 2) {
            Vector2f refNorm = ref.Cross(-1.0f);
            if(flip) {
                refNorm = -refNorm;
            }
            float max = refNorm.Dot(ref.best);
            if(refNorm.Dot(clipped[0]) - max < 0.0) {
                clipped.erase(clipped.begin());
            }
            if(refNorm.Dot(clipped[1]) - max  < 0.0) {
                clipped.erase(clipped.begin() + 1);
            }
            if(clipped.empty()) {
                c.contactPoint = ref.best;
            }
            else if(clipped.size() == 1) {
                c.contactPoint = clipped[0];
            }
            else {
                c.contactPoint = (clipped[0] + clipped[1]) * 0.5f;
                c.clipEdge = Edgef(clipped[0], clipped[1]);
            }

        }
        else {
            c.contactPoint = ref.best;
        }
    }
}

bool PolyCollision(Polygonf& poly1, Polygonf& poly2, Vector2f& mtv) {
    Collision ret = PolyCollision(poly1, poly2);
    mtv = ret.mtv;
    return ret;
}
Collision PolyCollision(Polygonf& poly1, Polygonf& poly2) {
    Collision c(FLT_MAX);
    float distance = FLT_MAX;
    float minDistance = FLT_MAX;
    vector<Vector2f> axes = poly1.GetAxes();
    vector<Vector2f> axes2 = poly2.GetAxes();
    for(size_t i = 0; i < axes.size(); i++) {
        Vector2f& axis = axes[i];
        if(!CheckProjectionPoly(poly1, poly2, axes[i], distance)) {
            c.mtv = 0;
            c.normal = 0;
            c.hasCollision = false;
            return c;
        }
        else if(fabs(distance) < fabs(minDistance)) {
            minDistance = distance;
            c.mtv = axis * distance;
            c.normal = axis;
            if(distance < 0) {
                c.normal = -axis;
            }
            c.hasCollision = true;
        }
    }
    for(size_t i = 0; i < axes2.size(); i++) {
        Vector2f& axis = axes2[i];
        if(!CheckProjectionPoly(poly1, poly2, axes2[i], distance)) {
            c.mtv = 0;
            c.normal = 0;
            c.hasCollision = false;
            return c;
        }
        else if(fabs(distance) < fabs(minDistance)) {
            minDistance = distance;
            c.mtv = axis * distance;
            c.normal = axis;
            if(distance < 0) {
                c.normal = -axis;
            }
            c.hasCollision = true;
        }
    }
    //c.mtv = Vector2i(c.mtv);
    return c;
}
bool RectRectCollision2(Rectanglef rect1, Rectanglef rect2, Vector2f& mtv) {
    mtv = FLT_MAX;
    float distance;
    Vector2f axes[4];
    axes[0] = (rect1.GetTopRight() - rect1.GetBotRight());
    axes[1] = (rect2.GetTopRight() - rect2.GetBotRight());
    axes[2] = (rect1.GetTopRight() - rect1.GetTopLeft());
    axes[3] = (rect2.GetTopRight() - rect2.GetTopLeft());
    for(int i = 0; i < 4; i++) {
        if(!CheckProjection(rect1, rect2, axes[i], distance)) {
            mtv = 0;
            return false;
        }
        else if(fabs(distance) < fabs(mtv.Magnitude())) {
            mtv = axes[i].GetNormal().GetUnit() * distance;
        }
    }
    return true;
}
bool RectRectCollision2(Rectanglef rect1, Rectanglef rect2) {
    Vector2f axis1, axis2, axis3, axis4;
    axis1 = (rect1.GetTopRight() - rect1.GetBotRight());
    axis2 = (rect2.GetTopRight() - rect2.GetBotRight());
    axis3 = (rect1.GetTopRight() - rect1.GetTopLeft());
    axis4 = (rect2.GetTopRight() - rect2.GetTopLeft());

    if(!CheckProjection(rect1, rect2, axis1)) {
        return false;
    }
    if(!CheckProjection(rect1, rect2, axis2)) {
        return false;
    }
    if(!CheckProjection(rect1, rect2, axis3)) {
        return false;
    }
    if(!CheckProjection(rect1, rect2, axis4)) {
        return false;
    }
    return true;
}
Collision CircleCollision(Circlef& circ1, Circlef& circ2) {
    float dist = circ1.GetPosition().DistanceSqrt(circ2.GetPosition());\
    return dist <= circ1.GetRadius() + circ2.GetRadius();
}
Collision CirclePolyCollision(Circlef& circ, Polygonf& poly) {
    std::vector<Vector2f> axes = poly.GetAxes();
    return false;
}
Collision CircleRectCollision(Circlef& circ, Rectanglef& rect) {
    Collision c;
    c.hasCollision = true;
    Vector2f center = circ.GetCenter();
    Vector2f closestPoint = rect.GetClosestVertex(center);
    vector<Vector2f> axes = rect.GetAxes();
    Vector2f circleAxis = (closestPoint - center).GetUnit();
    axes.push_back(circleAxis);
    Vector2f proj, proj2;
    float minDist = FLT_MAX;
    for(auto axis : axes) {
        proj = rect.GetProjection(axis);
        proj2 = circ.GetProjection(axis);
        if(!CheckRange(proj, proj2)) {
            c.hasCollision = false;
            c.mtv = 0;
            c.normal = 0;
            return c;
        }
        else {
            float min = std::max(proj.x, proj2.x);
            float max = std::min(proj.y, proj2.y);
            float distance = max - min;
            if(fabs(distance) < fabs(minDist)) {
                minDist = distance;
                if(distance < 0) {
                    c.normal = -axis;
                }
                else {
                    c.normal = axis;
                }
                c.mtv = axis * distance;
            }
        }
    }
    if(!c) {
        c.mtv = 0;
    }
    return c;
}
bool CheckProjection(Rectanglef& rect1, Rectanglef& rect2, Vector2f axis) {
    Vector2f projection1 = rect1.GetProjection(axis);
    Vector2f projection2 = rect2.GetProjection(axis);
    return(CheckRange(projection1, projection2));
}
bool CheckProjection(Rectanglef& rect1, Rectanglef& rect2, Vector2f axis, float& distance) {
    Vector2f projection1 = rect1.GetProjection(axis);
    Vector2f projection2 = rect2.GetProjection(axis);
    if(CheckRange(projection1, projection2)) {
        float min = std::max(projection1.x, projection2.x);
        float max = std::min(projection1.y, projection2.y);
        distance = max - min;
        if(projection1.x < projection2.x) {
            distance = -distance;
        }
        return true;
    }
    else {
        distance = 0;
        return false;
    }
}
bool CheckProjectionPoly(Polygonf& p1, Polygonf& p2, Vector2f axis, float& distance) {
    Vector2f projection1 = p1.GetProjection(axis);
    Vector2f projection2 = p2.GetProjection(axis);
    if(CheckRange(projection1, projection2)) {
        float min = std::max(projection1.x, projection2.x);
        float max = std::min(projection1.y, projection2.y);
        distance = max - min;
        if(projection1.x < projection2.x) {
            distance = -distance;
        }
        return true;
    }
    else {
        distance = 0;
        return false;
    }
}
bool CheckRange(float min, float max, float value) {
    return (value >= min) && (value <= max);
}
bool CheckRange(Vector2f first, Vector2f second) {
    if(first.y <= second.x || second.y <= first.x) {
        return false;
    }
    return true;
}

bool PointRectCollision(Vector2f point, Rectanglef rect) {
    return RectRectCollision2(rect, Rectanglef(point, 1, 1));
}
Collision AABBCollision(Rectanglef& rect1, Rectanglef& rect2) {
    Collision col;
    if(RectRectCollision(rect1, rect2)) {
        col.hasCollision = true;
        float left, right, top, bottom;
        left = rect2.GetLeft() - rect1.GetRight();
        right = rect2.GetRight() - rect1.GetLeft();
        top = rect2.GetTop() - rect1.GetBottom();
        bottom = rect2.GetBottom() - rect1.GetTop();

        if(fabs(left) > right) {
            col.mtv.x = right;
            col.normal.x = 1;
        }
        else {
            col.mtv.x = left;
            col.normal.x = -1;
        }
        if(fabs(bottom) > top) {
            col.mtv.y = top;
            col.normal.y = 1;
        }
        else {
            col.mtv.y = bottom;
            col.normal.y = -1;
        }

        if(fabs(col.mtv.x) <= fabs(col.mtv.y)) {
            col.mtv.y = 0;
            col.normal.y = 0;
        }
        else {
            col.mtv.x = 0;
            col.normal.x = 0;
        }
    }
    return col;
}

bool RectRectCollision(Rectanglef& rect1, Rectanglef& rect2) {
    if(rect1.GetLeft() > rect2.GetRight()
       || rect1.GetRight() < rect2.GetLeft()
       || rect1.GetTop() < rect2.GetBottom()
       || rect1.GetBottom() > rect2.GetTop())
        return false;
    return true;
}
bool BoundingCircleCollision(Rectanglef rect1, Rectanglef rect2) {
    float rect1Radius = rect1.GetBotRight().Distance(rect1.GetTopLeft()) / 2;
    float rect2Radius = rect2.GetBotRight().Distance(rect2.GetTopLeft()) / 2;
    float distance = rect1.GetCenter().Distance(rect2.GetCenter());
    return distance <= rect1Radius + rect2Radius;
}


bool Raycast(Vector2f pos, Vector2f dir, Rectanglef& rect, float length) {
    Vector2f t1, t2;
    float tmin = -FLT_MAX;
    float tmax = FLT_MAX;
    if(dir.x != 0.0) {
        float tx1 = (rect.GetLeft() - pos.x) / dir.x;
        float tx2 = (rect.GetRight() - pos.x) / dir.x;
        tmin = std::max(tmin, std::min(tx1, tx2));
        tmax = std::min(tmax, std::max(tx1, tx2));
    }
    if(dir.y != 0.0) {
        float ty1 = (rect.GetBottom() - pos.y) / dir.y;
        float ty2 = (rect.GetTop() - pos.y) / dir.y;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
    }
    return tmax >= tmin;
}

bool RayIntersectInv(Vector2f pos, Vector2f dir, Rectanglef& rect, float maxLength) {
    if(maxLength) {
        float dist = pos.Distance(rect.GetCenter());
        if(dist > maxLength * maxLength) {
            return false;
        }
    }
    float tx1 = (rect.GetLeft() - pos.x) * dir.x;
    float tx2 = (rect.GetRight() - pos.x) * dir.x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = (rect.GetBottom() - pos.y) * dir.y;
    float ty2 = (rect.GetTop() - pos.y) * dir.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));
    return tmax >= tmin && tmax > 0;
}
}

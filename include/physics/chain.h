#ifndef CHAIN_H_INCLUDED
#define CHAIN_H_INCLUDED

#include "graphics/vector2.h"
#include "physics/rigidbody.h"
#include "geometry/rectangle.h"
#include "physics/physicsrect.h"
#include "constraint.h"

#include <vector>
#include <cmath>
#include <memory>

namespace solstice {
class Chain {
    std::vector<PhysicsBox2f> mLinks;
    Constraint mLinkConst;
    Constraint mEndConst;
    std::vector<Constraint> mLinkConstraints;
    Vector2f mAnchorPoint;
    RigidBody* mAttachedObject;
    size_t mNumIter;
    void GenDefaultLinkQuad();
public:
    QuadData mLinkQuad;
    Chain();
    Chain(Constraint c);
    Chain(Constraint linkConst, Constraint endConst);

    void Init(int numLinks, float linkSize);
    void Init(int numLinks, float linkSize, Constraint linkConst);
    void Init(int numLinks, PhysicsBox2f link, Constraint linkConst);
    void SetAnchorPoint(Vector2f pt);
    void AddXY(Vector2f mv);

    void Update(double t);
    void SetEndConst(Constraint c)          {mEndConst = c;}
    void SetLinkConst(Constraint c)         {mLinkConst = c;}
    Constraint& GetLinkConstraint()         {return mLinkConst;}
    Constraint& GetEndConstraint()          {return mEndConst;}
    std::vector<PhysicsBox2f> GetLinks()    {return mLinks;}
    size_t GetNumLinks()                    {return mLinks.size();}
    Vector2f GetAnchorPoint()               {return mAnchorPoint;}
    size_t GetNumIter()                     {return mNumIter;}
    void SetNumIter(size_t i)               {mNumIter = i;}
    PhysicsBox2f& GetBack()                 {return mLinks.back();}
    PhysicsBox2f& GetFront()                {return mLinks.front();}
    PhysicsBox2f& GetLink(size_t i)         {return mLinks[i];}
    float Length()                          {return mLinkConst.GetMinLength() * mLinks.size() + mEndConst.GetMinLength();}
    float Length2()                         {return Length() * Length();}
    void Attach(RigidBody* obj);
    RigidBody* GetAttachedObject()          {return mAttachedObject;}
    bool CheckInbounds(RigidBody* body);
    void Cut(size_t i);
    void Mend() {Init(mLinks.size(), mLinks.front(), mLinkConst);}
    void DrawHitBoxes(SceneRenderer& rend, Color col = COL_BLUE);
    void Draw(SceneRenderer& rend);
};
}

#endif // CHAIN_H_INCLUDED

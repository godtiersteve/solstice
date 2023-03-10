#ifndef HOVERTROLLEY_H_INCLUDED
#define HOVERTROLLEY_H_INCLUDED

#include "interactables/interactable.h"
#include "physics/physicsrect.h"
#include "physics/constraint.h"
#include "physics/rigidbodycontainer.h"

namespace solstice {
    class HoverTrolley : public Interactable, public PhysicsBox2f {
        protected:
            float mMaxWeight, mCurrentWeight;
            std::vector<Constraint> mConstraints;
            RigidBodyContainer mBodyContainer;
            PhysicsBox2f mBaseBox, mHoverBox;
            void CalcWeight();
            void CalcHeight();
        public:
            float mLevitateHeight, mMinHeight;
            HoverTrolley(float levitateHeight = 150, float maxWeight = 10);
            float GetMaxWeight() {return mMaxWeight;}
            float GetCurrentWeight() {return mCurrentWeight;}
            void AddWeight(float w);
            void SetMaxWeight(float w);
            void RemoveWeight(float w);
            void SetWeight(float w);
            void Update(double dt);
            void SetNoTorque(bool b) {mNoTorque = true;}
            void InitFloatBox(float h, float w) {mHoverBox.GetHitBox().SetHW(h, w);}
            Rectanglef& GetHitBox() {return PhysicsBox2f::mHitBox;}
            Collision CheckBodyCollision(RigidBody* body);
            void CheckAndResolveCollision(RigidBody* body) {}
            void CheckAndResolveCollision(BackgroundTile2& tile) {PhysicsBox2f::CheckAndResolveCollision(tile);}
            PhysicsBox2f& GetHoverBox() {return mHoverBox;}
            Rectanglef GetHoverHitBox() {return mHoverBox.GetHitBox();}
    };
}


#endif // HOVERTROLLEY_H_INCLUDED

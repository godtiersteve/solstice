#ifndef INTERACTABLE_H_INCLUDED
#define INTERACTABLE_H_INCLUDED

#include "engine/gameobject.h"
#include "geometry/hitbox.h"
#include "graphics/3d/modelanimset.h"
#include "engine/resizableobject.h"
#include "graphics/renderableobject.h"
#include "physics/physicsrect.h"

namespace solstice {
    enum InteractableType {
        INTERACT_CHECKPOINT, INTERACT_TROLLEY, INTERACT_DOOR, INTERACT_SWITCH,
        INTERACT_TELEPORTER, INTERACT_CONVEYER_BELT, INTERACT_TRACTOR_BEAM,
        INTERACT_ICEBLOCK, INTERACT_DAMAGETILE
    };
    class Interactable : public GameObject, public ResizableObject, public RenderableObject {
        protected:
            int mInteractableType;
        public:
            Rectanglef mHitBox;
            Color mHitBoxColor;
            ModelAnimationset mAnimationSet;
            Interactable() {
                mModelID = -1;
                mInteractableType = -1;
            }
            void SetX(float x) {mHitBox.SetX(x);}
            void SetY(float y) {mHitBox.SetY(y);}
            void SetXY(float x, float y) {mHitBox.SetPosition(Vector2f(x, y));}
            void SetXY(Vector2f xy) {mHitBox.SetPosition(xy);}
            float GetX() {return mHitBox.GetX();}
            float GetY() {return mHitBox.GetY();}
            void IncreaseWidth(int w) {}
            void IncreaseHeight(int h) {}
            void IncreaseSize(int height, int width) {}
            void AddX(float x) {mHitBox.AddX(x);}
            void AddY(float y) {mHitBox.AddY(y);}
            void AddXY(float x, float y) {mHitBox.AddXY(Vector2f(x, y));}
            void AddXY(Vector2f xy) {mHitBox.AddXY(xy);}
            void SetAngle(float a) {mHitBox.SetAngle(a);}
            void AddAngle(float a) {mHitBox.SetAngle(mHitBox.GetAngle() + a);}
            float GetAngle() {return 0;}
            IShapef* GetShape() {return &mHitBox;}
            Rectanglef GetHitRect() {return mHitBox;}
            Vector2f GetPosition() {return mHitBox.GetPosition();}
            virtual Collision CheckBodyCollision(RigidBody* b) {return false;}
            virtual Collision CheckMovableCollision(MovableObject* o) {return false;}
            virtual void Update(double dt) {}
            int GetInteractableType() {return mInteractableType;}
    };
}

#endif // INTERACTABLE_H_INCLUDED

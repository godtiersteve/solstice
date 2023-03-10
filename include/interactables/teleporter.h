#ifndef TELEPORTER_H_INCLUDED
#define TELEPORTER_H_INCLUDED

#include "interactables/interactable.h"
#include "physics/rigidbody.h"

namespace solstice {
    enum TeleporterType {TELE_TOUCH, TELE_SWITCH, TELE_STEP};
    class Teleporter : public Interactable {
    private:
        void TouchTeleport(RigidBody* body);
        std::vector<RigidBody*> mTeleportedBodies;
    public:
        Rectanglef mEndPoint;
        bool mStopsXMomentum, mStopsYMomentum;
        TeleporterType mTeleportType;
        Teleporter(TeleporterType type = TELE_TOUCH);
        Teleporter(TeleporterType type, Vector2f endPoint);
        void SetEndPointPos(Vector2f p) {mEndPoint.SetPosition(p);}
        bool CheckCanUse(RigidBody* body);
        void TeleportBody(RigidBody* body);
        Collision CheckBodyCollision(RigidBody* b) {TeleportBody(b); return false;}
        void SetPosition(Vector2f pos);
        void SetPosition(Vector2f start, Vector2f end);
        void Update(double dt);
    };
}

#endif // TELEPORTER_H_INCLUDED

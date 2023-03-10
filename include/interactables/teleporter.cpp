#include "teleporter.h"

namespace solstice {
    Teleporter::Teleporter(TeleporterType type) {
        mTeleportType = type;
        mEndPoint = mHitBox;
        mEndPoint.SetPosition(mHitBox.GetPosition() + Vector2f(200, 0));
        mStopsXMomentum = true;
        mStopsYMomentum = true;
        mInteractableType = INTERACT_TELEPORTER;
    }
    Teleporter::Teleporter(TeleporterType type, Vector2f endPoint) {
        mTeleportType = type;
        mEndPoint = mHitBox;
        mEndPoint.SetPosition(mHitBox.GetPosition() + endPoint);
        mStopsXMomentum = true;
        mStopsYMomentum = true;
        mInteractableType = INTERACT_TELEPORTER;
    }
    bool TeleporterCheckCanUse(RigidBody* body) {
    }
    void Teleporter::TeleportBody(RigidBody* body) {
        switch(mTeleportType) {
            case TELE_TOUCH:
                TouchTeleport(body);
                break;
            case TELE_SWITCH:
                break;
            case TELE_STEP:
                break;
            default:
                break;
        }
    }
    void Teleporter::Update(double dt) {
        for(auto iter = mTeleportedBodies.begin(); iter != mTeleportedBodies.end(); ) {
            if(!solstice::CheckCollision(&(*iter)->GetHitBox(), &mHitBox) && !solstice::CheckCollision(&(*iter)->GetHitBox(), &mEndPoint)) {
                iter = mTeleportedBodies.erase(iter);
            }
            else {
                iter++;
            }
        }
    }
    void Teleporter::TouchTeleport(RigidBody* body) {
        for(size_t i = 0; i < mTeleportedBodies.size(); i++) {
            if(body == mTeleportedBodies[i]) {
                return;
            }
        }
        bool teleported = false;
        if(CheckCollision(&body->GetHitBox(), &mHitBox)) {
            body->SetPosition(mEndPoint.GetPosition());
            mTeleportedBodies.push_back(body);
            teleported = true;
        }
        else if(CheckCollision(&body->GetHitBox(), &mEndPoint)) {
            body->SetPosition(mHitBox.GetPosition());
            mTeleportedBodies.push_back(body);
            teleported = true;
        }
        if(teleported) {
            if(mStopsXMomentum) {
                body->StopX();
            }
            if(mStopsYMomentum) {
                body->StopY();
            }
        }
    }
    void Teleporter::SetPosition(Vector2f pos) {
        Vector2f diff = mEndPoint.GetPosition() - mHitBox.GetPosition();
        mHitBox.SetPosition(pos);
        mEndPoint.SetPosition(pos + diff);
    }
    void Teleporter::SetPosition(Vector2f start, Vector2f end) {
        mHitBox.SetPosition(start);
        mEndPoint.SetPosition(end);
    }
}

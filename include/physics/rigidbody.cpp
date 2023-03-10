#include "rigidbody.h"
namespace solstice {
size_t RigidBody::sIDs = 0;
RigidBody::RigidBody() {
    SetDefaults();
}
RigidBody::RigidBody(float invMass, float fric, float elast, Vector2f vel) {
    SetDefaults();
    mInvMass = invMass;
    mInvMass2 = invMass;
    mFriction = fric;
    mElasticity = elast;
    mVelocity = vel;
}
RigidBody::RigidBody(BackgroundTile2 bg) {
    mInvMass = 0.0f;
    mInvMass2 = 0.0f;
    mFriction = bg.GetFriction();
    mElasticity = bg.GetElasticity();
    mVelocity = 0.0f;
    mStep = 0.0f;
    mRotVelocity = 0;
    mLastAngle = 0.0f;
    mNoTorque = false;
    mUnderwater = false;
    mInertia = 0.0f;
    mAngleDamping = 0.0f;
    mID = sIDs++;
    char num[20];
    sprintf(num, "Body_%i", mID);
    mName = num;
    mMaxRotVel = 0.0;
    mIsPlatform = false;
    mNoPlatform = false;
    mSubmergeDepth = 0.0f;
}
void RigidBody::RigidBody::SetDefaults() {
    mInvMass = 1.0f;
    mInvMass2 = 1.0f;
    mFriction = 0.0f;
    mElasticity = 0.0f;
    mVelocity = 0.0f;
    mStep = 0.0f;
    mRotVelocity = 0;
    mLastAngle = 0.0f;
    mNoTorque = false;
    mUnderwater = false;
    mInertia = 0.1f;
    mAngleDamping = 0.98f;
    mID = sIDs++;
    char num[20];
    sprintf(num, "Body_%i", mID);
    mName = num;
    mMaxRotVel = PI / 8.0;
    mIsPlatform = false;
    mNoPlatform = false;
    mZOffset = 0.0f;
    mCollisionFound = false;
    mSubmergeDepth = 0.0f;
    ResetCollisionLocs();
}
void RigidBody::ResetCollisionLocs() {
    for(size_t i = 0; i < 4; i++) {
        mCollisionLocations[i] = false;
    }
}
void RigidBody::CheckCollisions(Vector2f mtv) {
    if(mtv.x > 0.1 && mVelocity.x <= 0) {
        mCollisionLocations[COL_LEFT] = true;
    }
    else if(mtv.x < -0.1 && mVelocity.x >= 0) {
        mCollisionLocations[COL_RIGHT] = true;
    }
    if(mtv.y > 0.1 && mVelocity.y <= 0) {
        mCollisionLocations[COL_BOTTOM] = true;
    }
    else if(mtv.y < -0.1 && mVelocity.y >= 0) {
        mCollisionLocations[COL_TOP] = true;
    }
}
void RigidBody::ResetFlags() {
    mUnderwater = false;
    mGrounded = false;
    ResetCollisionLocs();
}
void RigidBody::SetPosition(Vector2f pos) {
    mHitBox.SetPosition(pos);
    mLastPos = pos;
	mPosition = pos;
}
void RigidBody::AddXY(Vector2f mv) {
    mHitBox.AddXY(mv);
    mLastPos+= mv;
	mPosition+=mv;
}
void RigidBody::AddX(float x) {
    mHitBox.AddX(x);
    mLastPos.x+= x;
	mPosition.x+=x;
}
void RigidBody::AddY(float y) {
    mHitBox.AddY(y);
    mLastPos.y+= y;
	mPosition.y+=y;
}
void RigidBody::Move(Vector2f mv) {
    mHitBox.AddXY(mv);
	mPosition+=mv;
	mLastPos+= mv;
}
void RigidBody::MoveTo(Vector2f mv) {
    mHitBox.SetPosition(mv);
	mPosition = mv;
	mLastPos = mv;
}
void RigidBody::SetAttributes(float invMass, float buoyancy, float elast) {
    mInvMass = invMass;
    mBuoyancy = buoyancy;
    mElasticity = elast;
}
void RigidBody::ApplyForces(float tstep) {
    mVelocity.x+= Trunc(mAppliedForce.x, 4) * tstep;
    mVelocity.y+= Trunc(mAppliedForce.y, 4) * tstep;
    mAppliedForce = 0.0f;
}
void RigidBody::AddRotVelocity(float v) {
    mRotVelocity+= v;
}
void RigidBody::VerletNoVelocity(float dt) {
    mStep = dt;
    Vector2f pos = GetPosition();
    mPosition = GetPosition();
    Vector2f vel = pos - mLastPos + mAppliedImpulse;
    mVelocity = vel;
    Vector2f newPos = vel + mAppliedForce * mStep * mStep;
    AdjustVelocity(newPos);
    mHitBox.SetPosition(pos + newPos);
	mPosition = pos + newPos;
    mLastPos = pos;
    mAppliedForce = 0.0f;
    if(mGrounded) {
        mState = MOVE_GROUNDED;
    }
    else if(mUnderwater) {
        mState = MOVE_SWIMMING_UNDERWATER;
    }

    float t = 1.0;
    if(mUnderwater) {
        t = 0.01;
    }
    if(fabs(mLastRotVelocity + mRotVelocity) < 0.01) {
        mRotVelocity = 0.0;
    }
    float angle = mHitBox.GetAngle();
    float angleVel = (angle - mLastAngle) * mAngleDamping * 0.98;
    if(fabs(angleVel) < RADIAN) {
        angleVel = 0.0f;
    }
    if(fabs(mRotVelocity) < RADIAN * 0.5) {
        mRotVelocity = 0.0f;
    }
    float angleAccel = mRotVelocity * mStep * mStep;
    angleVel += angleAccel;
    if(angleVel < -mMaxRotVel) {
        angleVel = -mMaxRotVel;
    }
    else if(angleVel > mMaxRotVel) {
        angleVel = mMaxRotVel;
    }
    // || (mGrounded && fabs(angleVel) < RADIAN * 2 && mVelocity.Magnitude2() <= 0.1)
    if((!mNoTorque && mUnderwater && GetSubmergeRatio() >= 0.3)
       //|| (mGrounded && fabs(angleVel) < RADIAN * 2 && mVelocity.Magnitude2() <= 0.1 && !mCollisionLocations[COL_RIGHT] && !mCollisionLocations[COL_LEFT])
       ) {
        //SetAngle(RotateToZero(mHitBox.GetAngle(), RADIAN * 2));
        angle = RotateToZero(mHitBox.GetAngle(), RADIAN * 2);
        angleVel = 0.0f;
        mRotVelocity = 0.0f;
    }
    float newAngle = angle + angleVel * t;
    mHitBox.SetAngle(newAngle);
    if(fabs(newAngle - angle) <= 0.0002f) {
        angle = newAngle;
    }
    mLastAngle = angle;
    mLastRotVelocity = mRotVelocity;
    mRotVelocity = 0.0f;
    mAppliedImpulse = 0.0;
    mCollisionFound = false;
    ResetCollisionLocs();
}

Vector2f RigidBody::GetInvMomentum() {
    Vector2f m = GetMomentum();
    Vector2f ret;
    ret.x = m.x ? 1.0f / m.x : 0.0f;
    ret.y = m.y ? 1.0f / m.y : 0.0f;
    return ret;
}
void RigidBody::InitHitBox(Vector2f pos, float h, float w, int drawFrom) {
    mHitBox.Init(pos, h, w, (DrawPoint)drawFrom);
    mLastPos = pos;
}
void RigidBody::CheckAndResolveCollision(RigidBody* b) {
    if(this == b) {
        return;
    }
    //Collision c = solstice::CheckCollision(GetHitBoxShape(), b->GetHitBoxShape());
    Collision c = solstice::CheckCollision(GetHitShapef(), b->GetHitShapef());
    if(!c || c.mtv.Length2() < 1.0) {
        if(c.normal.y > 0.25) {
            mGrounded = true;
        }
        if(fabs(c.normal.y) > 0.25) {
            mLastPos.y = GetPosition().y;
        }
        if(fabs(c.normal.x) > 0.25) {
            mLastPos.x = GetPosition().x;
        }
        return;
    }
    if(c && fabs(GetInvMass()) + fabs(b->GetInvMass()) > 0.001) {
        mCollisionFound = true;
        CheckCollisions(c.mtv);
        b->CheckCollisions(-c.mtv);
        Vector2f vel = CalcVelocity(b, c);
        mVelocity = GetPosition() - mLastPos;
        Vector2f bodyVel = b->GetPosition() - b->GetLastPosition();
        if(!GetNoTorque() || !b->GetNoTorque()) {
            c.GetContactPoint(GetHitBox(), b->GetHitBox());
        }
        if(!mUnderwater && !b->GetUnderWater()) {
            if(mInvMass2 > 0.0 && b->GetInvMass() > 0.0) {
                float massA = GetInvMass() / (GetInvMass() + b->GetInvMass());
                float massB = b->GetInvMass() / (GetInvMass() + b->GetInvMass());
                if(!GetNoTorque()) {

                    Vector2f center = GetHitBox().GetCenter();
                    Vector2f rA = c.contactPoint - center;
                    Vector2f force = rA * (vel.Dot(rA) / rA.Dot(rA));
                    force = vel - force;
                    float rotVel = rA.Cross(-force);
                    AddRotVelocity(-rotVel * mInertia * massA);
                }
                if(!b->GetNoTorque()) {
                    Vector2f center = b->GetHitBox().GetCenter();
                    Vector2f rA = c.contactPoint - center;
                    Vector2f force = rA * (vel.Dot(rA) / rA.Dot(rA));
                    force = vel - force;
                    float rotVel = rA.Cross(-force);
                    b->AddRotVelocity(rotVel * b->GetInertia() * massB);
                }
            }
        }
        ResolveCollision(b, c, vel);
        if(mIsPlatform && c.normal.y < 0.1f) {
            if(fabs(vel.x) < 1.0f) {
                b->AddX(GetVelocity2().x);
            }
        }
        else if(b->GetIsPlatform() && c.normal.y > -0.1f) {
            if(fabs(mVelocity.x) < 1.0f) {
                AddX(b->GetVelocity2().x);
            }
        }
    }
}
void RigidBody::ResolveCollision(RigidBody* b, Collision& c, Vector2f vel) {
    float massA = GetInvMass() / (GetInvMass() + b->GetInvMass());
    float massB = b->GetInvMass() / (GetInvMass() + b->GetInvMass());
    float pushA = 0.5, pushB = 0.5;
    if(b->GetInvMass() < 0.0) {
        massA = 0.0;
        massB = 1.0;
        pushA = 0.0f;
        pushB = 1.0f;
    }
    else if(mInvMass2 < 0.0) {
        massA = 1.0;
        massB = 0.0;
        pushA = 1.0f;
        pushB = 0.0f;
    }
    if(fabs(c.mtv.x) < 3.0f) {
        vel.x = 0.0f;
    }
    if(mGrounded && c.mtv.y < 0) {
        AddX(c.mtv.x * pushA);
        b->AddY(-c.mtv.y);
        b->AddX(-c.mtv.x * pushB);
    }
    else {
        AddXY(c.mtv * pushA);
        b->AddXY(-c.mtv * pushB);
    }
    GetLastPosition()-= vel * massA;
    b->GetLastPosition()+= vel * massB;
    if(c.normal.y >= 0.25) {
        mGrounded = true;
    }
    else if(c.normal.y < -0.25f) {
        b->SetGrounded(true);
    }
}
void RigidBody::ResolveCollision(Collision& c, float mass, Vector2f vel) {
    AddXY(c.mtv * mass);
    vel = vel * mass;
    if(fabs(vel.y) < 0.5) {
        vel.y = 0.0;
    }
    GetLastPosition()-= vel * mass;
    if(c.normal.y >= 0.5f) {
        mGrounded = true;
    }
}
void RigidBody::CheckAndResolveCollision(BackgroundTile2& ground) {
    if(ground.GetType() == TILE_PLATFORM && mNoPlatform) {
        return;
    }
    Collision c = solstice::CheckCollision(GetHitShapef(), &ground.GetHitBox());
    //Collision c = solstice::CheckCollision(&mHitBox, &ground.GetHitBox());
    if(!c || (c && c.mtv.Length2() < 1.0)) {
        if(c && c.normal.y > 0.25) {
            mGrounded = true;
        }
        if(fabs(c.normal.y) > 0.25) {
            mLastPos.y = GetPosition().y;
        }
        if(fabs(c.normal.x) > 0.25) {
            mLastPos.x = GetPosition().x;
        }
        //CalcTorque(GetPosition() - GetLastPosition(), c, ground);
        return;
    }
    else if(c && GetInvMass()) {
        mCollisionFound = true;
        CheckCollisions(c.normal);
        if(ground.GetType() == TILE_GROUND) {
            Vector2f velocity = GetPosition() - GetLastPosition();
            Vector2f normalVel = c.normal.Dot(velocity) * c.normal;
            Vector2f tangentVel = velocity - normalVel;
            float groundFriction = ground.GetFriction() * fabs(c.normal.y);
            if(c.normal.x && c.normal.y > 0.25f) {
                groundFriction = 1.0f;
            }
            float friction = std::max(mFriction, groundFriction);
            float elasticity = std::max(mElasticity, ground.GetElasticity());
            //Vector2f vel = tangentVel * -friction + normalVel * -(1 + elasticity);
            Vector2f vel = CalcVelocity(velocity, c, friction, elasticity);
            CalcTorque(velocity, c, ground);
            ResolveCollision(c, 1.0f, vel);
        }
        else if(ground.GetType() == TILE_PLATFORM) {
            CheckPlatform(c, ground);
        }
        else if(ground.GetType() == TILE_WATER) {
            SetUnderwater(true);
        }
        return;
    }
}
void RigidBody::CalcTorque(Vector2f vel, Vector2f contactPoint) {
        Vector2f dist = GetHitBox().GetCenter() - contactPoint;
        float rotVel = dist.Cross(mVelocity) * mInertia;
        if(fabs(rotVel) < RADIAN * 2) {
            rotVel = 0.0;
        }
        AddRotVelocity(rotVel);
}
void RigidBody::CalcTorque(Vector2f vel, Collision& c, BackgroundTile2& ground) {
    if(!GetNoTorque()) {
        c.GetContactPoint(GetHitBox(), ground.GetHitBox());
        //vel.y-= 10.0;
        Vector2f momentArm = c.contactPoint - GetHitBox().GetCenter();
        Vector2f para = momentArm * (vel.Dot(momentArm) / momentArm.Dot(momentArm));
        Vector2f force = vel - para;
        float rotVel = momentArm.Cross(force);
        AddRotVelocity(-rotVel);
    }
}
void RigidBody::DampVelocity(float damp) {
    Vector2f vel = GetPosition() - mLastPos;
    mLastPos = GetPosition() - vel * damp;
}
void RigidBody::DampX(float damp) {
    float vel = GetPosition().x - mLastPos.x;
    mLastPos.x = GetPosition().x - vel * damp;
}
void RigidBody::DampY(float damp) {
    float vel = GetPosition().y - mLastPos.y;
    mLastPos.y = GetPosition().y - vel * damp;
}
void RigidBody::CheckPlatform(Collision& c, BackgroundTile2& bg) {
    Vector2f velocity = GetPosition() - GetLastPosition();
    Rectanglef rect = bg.GetHitBox2().GetRect(0);
    if(bg.GetSprings()) {
        if(velocity.y <= 0) {
            if(!bg.Springing()) {
                if(GetHitBox().GetBottom() >= rect.GetBottom()) {
                    SetPosition(Vector2f(GetPosition().x, rect.GetTop()));
                    GetLastPosition().y = GetPosition().y;
                    SetGrounded(true);
                }
            }
            else if(GetHitBox().GetBottom() - velocity.y >= rect.GetBottom()) {
                GetLastPosition().y = GetPosition().y;
                SetGrounded(true);
                SetPosition(Vector2f(GetPosition().x, rect.GetTop()));
                AddY(c.mtv.y);
            }
        }
    }
    else if(GetHitBox().GetBottom() >= bg.GetHitBox().GetBottom() && velocity.y < 0.0f) {
        GetLastPosition().y = GetPosition().y;
        SetGrounded(true);
        SetPosition(Vector2f(GetPosition().x, bg.GetHitBox().GetTop()));
    }
    if(mGrounded && mAppliedForce.y < 0) {
        mAppliedForce.y = 0;
    }
}
Vector2f RigidBody::CalcVelocity(RigidBody* b, Collision& c) {
    mVelocity = GetPosition() - mLastPos;
    Vector2f bVel = b->GetPosition() - b->GetLastPosition();
    Vector2f velocity = mVelocity - bVel;
    Vector2f normalVel = c.normal.Dot(velocity) * c.normal;
    Vector2f tangentVel = velocity - normalVel;
    float friction = std::max(mFriction, b->GetFriction());
    float elasticity = std::min(mElasticity, b->GetElasticity());
    return tangentVel * -friction + normalVel * -(1 + elasticity);
}
Vector2f RigidBody::CalcVelocity(BackgroundTile2& ground, Collision& c) {
    mVelocity = GetPosition() - mLastPos;
    Vector2f normalVel = c.normal.Dot(mVelocity) * c.normal;
    Vector2f tangentVel = mVelocity - normalVel;
    float friction = std::max(mFriction, ground.GetFriction());
    float elasticity = std::max(mElasticity, ground.GetElasticity());
    float mag = tangentVel.Magnitude();
    if(mVelocity.y < 0 && !mVelocity.x) {
        if((mag / -mVelocity.y) < 0.5f) {
            friction = 1.001f;
        }
    }
    Vector2f vel = -(tangentVel * -friction + normalVel * -(1 + elasticity));
    return vel;
}
Vector2f RigidBody::CalcVelocity(Vector2f vel, Collision& col, float friction, float elasticity) {
    Vector2f normalVel = col.normal.Dot(vel) * col.normal;
    Vector2f tangentVel = vel - normalVel;
    return tangentVel * -friction + normalVel * -(1 + elasticity);
}
void RigidBody::Stop() {
    mVelocity.x = 0;
    mVelocity.y = 0;
    mAppliedForce.x = 0;
    mAppliedForce.y = 0;
    mLastPos = GetPosition();
}
void RigidBody::StopX() {
    mVelocity.x = 0;
    mAppliedForce.x = 0;
    mLastPos.x = GetPosition().x;
}
void RigidBody::StopY() {
    mVelocity.y = 0;
    mAppliedForce.y = 0;
    mLastPos.y = GetPosition().y;
}
}

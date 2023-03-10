#include "collisionresolution.h"

namespace solstice {
    void ResolveCollision(RigidBody* b1, RigidBody* b2, Collision& c) {
        if(!c.hasCollision) {
            return;
        }
        if(b1->GetInvMass() + b2->GetInvMass() > 0) {
            float massA = b1->GetInvMass() / (b1->GetInvMass() + b2->GetInvMass());
            float massB = b2->GetInvMass() / (b1->GetInvMass() + b2->GetInvMass());
            Vector2f b1Vel = b1->GetPosition() - b1->GetLastPosition();
            Vector2f b2Vel = b2->GetPosition() - b2->GetLastPosition();
            Vector2f vel = b1->CalcVelocity(b2, c);
            if(!b1->GetNoTorque() || !b2->GetNoTorque()) {
                c.GetContactPoint(b1->GetHitBox(), b2->GetHitBox());
            }
            if(!b1->GetNoTorque()) {
                Vector2f rA = (b1->GetHitBox().GetCenter() - c.contactPoint).Normalize();
                float rotVelA = -rA.Cross(c.normal.Dot(b2Vel) * c.normal);
                if(fabs(rotVelA) > RADIAN * 10) {
                    b1->AddRotVelocity(rotVelA);
                }
            }
            if(!b2->GetNoTorque()) {
                Vector2f rB = (b2->GetHitBox().GetCenter() - c.contactPoint).Normalize();
                float rotVelB = -rB.Cross(-c.normal.Dot(b1Vel) * -c.normal);
                if(fabs(rotVelB) > RADIAN * 10) {
                    b2->AddRotVelocity(rotVelB);
                }
            }
            if(fabs(c.normal.y) > 0.5f && fabs(c.mtv.y) > 1.0f && c.mtv.x) {
                float yMove = (c.mtv.x * c.mtv.x) / c.mtv.y + c.mtv.y;
                b1->AddY(yMove * massA);
                b2->AddY(yMove * -massB);
            }
            else {
                b1->AddXY(c.mtv * massA);
                b2->AddXY(c.mtv * -massB);
            }
            if(fabs(c.normal.y) > 0.5f) {
                if(b1Vel.y < 0.0f) {
                    b1->GetLastPosition().y-= vel.y * massA;
                }
                if(b2Vel.y < 0.0f) {
                    b2->GetLastPosition().y+= vel.y * massB;
                }
            }
            if(fabs(c.normal.x) > 0.5f) {
                b1->GetLastPosition().x-= vel.x * massA;
                b2->GetLastPosition().x+= vel.x * massB;
            }
            if(c.normal.y > 0.75f && b1Vel.y < 0.0f) {
                b1->SetGrounded(true);
            }
            if(c.normal.y < -0.75f && b1Vel.y < 0.0f) {
                b2->SetGrounded(true);
            }
            else {
                b2->ApplyForceY(-1000);
            }
            //b1->AddXY(mtv * massA);
            //b2->AddXY(-mtv * massB);
            //b1->GetLastPosition()-= massA * vel;
            //b2->GetLastPosition()+= massB * vel;
        }
        else {
            b1->AddXY(c.mtv * 0.5f);
            b2->AddXY(c.mtv * -0.5f);
        }
    }
    void ResolveCollision(RigidBody* b1, BackgroundTile2& ground, Collision& c) {
        if(!c.hasCollision) {
            return;
        }
        if(c.hasCollision && b1->GetInvMass()) {
            if(ground.GetType() == TILE_GROUND) {
                Vector2f vel = b1->CalcVelocity(ground, c);
                if(c.mtv.y && c.mtv.x) {
                    b1->AddY((c.mtv.x * c.mtv.x) / c.mtv.y + c.mtv.y);
                }
                else {
                    b1->AddXY(c.mtv);
                }
                if(c.normal.y) {
                    b1->GetLastPosition().y+= vel.y;
                }
                if(fabs(c.normal.x) > 0.5f) {
                    b1->GetLastPosition().x+= vel.x;
                }

                if(c.normal.y > 0.0f) {
                    b1->SetGrounded(true);
                }
                if(!b1->GetNoTorque()) {
                    c.GetContactPoint(b1->GetHitBox(), ground.GetHitBox());
                    Vector2f rA = b1->GetHitBox().GetCenter() - c.contactPoint;
                    float rotVel = rA.Cross(c.normal);
                    b1->AddRotVelocity(-rotVel);
                }
            }
            else if(ground.GetType() == TILE_PLATFORM) {
                Vector2f velocity = b1->GetPosition() - b1->GetLastPosition();
                if(ground.GetSprings()) {
                    if(velocity.y <= 0) {
                        if(!ground.Springing()) {
                            if(b1->GetHitBox().GetBottom() >= ground.GetHitBox().GetBottom()) {
                                b1->SetPosition(Vector2f(b1->GetPosition().x, ground.GetHitBox().GetTop()));
                                b1->GetLastPosition().y = b1->GetPosition().y;
                                b1->SetGrounded(true);
                            }
                        }
                        else if(b1->GetHitBox().GetBottom() - velocity.y >= ground.GetHitBox().GetBottom()) {
                            b1->GetLastPosition().y = b1->GetPosition().y;
                            b1->SetGrounded(true);
                            b1->AddY(c.mtv.y);
                        }
                    }
                }
                else if(b1->GetHitBox().GetBottom() >= ground.GetHitBox().GetBottom() && velocity.y < 0.0f) {
                    b1->GetLastPosition().y = b1->GetPosition().y;
                    b1->SetGrounded(true);
                    b1->SetPosition(Vector2f(b1->GetPosition().x, ground.GetHitBox().GetTop()));
                }
            }
            else if(ground.GetType() == TILE_WATER) {
                b1->SetUnderwater(true);
            }
        }
    }
}

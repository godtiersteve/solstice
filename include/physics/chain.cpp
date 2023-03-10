#include "chain.h"

namespace solstice {
    Chain::Chain() {
        mNumIter = 1;
        mAttachedObject = nullptr;
        GenDefaultLinkQuad();
    }
    Chain::Chain(Constraint c) {
        mNumIter = 1;
        mEndConst = c;
        Init(5, 30, c);
        mAttachedObject = nullptr;
        GenDefaultLinkQuad();
    }
    Chain::Chain(Constraint linkConst, Constraint endConst) {
        mNumIter = 1;
        mEndConst = endConst;
        Init(5, 30, linkConst);
        mAttachedObject = nullptr;
        GenDefaultLinkQuad();
    }
    void Chain::GenDefaultLinkQuad() {
        mLinkQuad.SetTextureCoords(Vector2f(256, 128), 128, 128, 2048, 2048);
        mLinkQuad.SetTextureID(7);
        mLinkQuad.SetHW(60, 60);
        mLinkQuad.SetDrawFrom(DRAW_CENTER);
    }
    void Chain::Cut(size_t i) {
        mLinkConstraints[i].mFirst = nullptr;
    }
    void Chain::Init(int numLinks, float linkSize) {
        Init(numLinks, linkSize, mLinkConst);
    }
    void Chain::Init(int numLinks, float linkSize, Constraint linkConst) {
        mLinks.clear();
        PhysicsBox2f link;
        link.GetHitBox().Init(linkSize, linkSize, DRAW_CENTER);
        link.SetPosition(mAnchorPoint);
        Init(numLinks, link, mLinkConst);
    }
    void Chain::Init(int numLinks, PhysicsBox2f link, Constraint linkConst) {
        mLinks.clear();
        mLinkConst = linkConst;
        mLinks = std::vector<PhysicsBox2f>(numLinks, link);
        mLinks.front().SetInvMass(0.0f);
        SetAnchorPoint(mAnchorPoint);
        mLinkConstraints = std::vector<Constraint>(numLinks, mLinkConst);
        for(size_t i = 0; i < mLinkConstraints.size(); i++) {
            if(i < mLinks.size() - 1) {
                mLinkConstraints[i].SetBodies(&mLinks[i], &mLinks[i + 1]);
            }
        }
        mLinkConstraints.back() = mEndConst;
        if(mAttachedObject) {
            mLinkConstraints.back().SetBodies(&mLinks.back(), mAttachedObject);
        }
        else {
            mLinkConstraints.back().SetBodies(nullptr, nullptr);
        }
    }
    void Chain::SetAnchorPoint(Vector2f pt) {
        mAnchorPoint = pt;
        if(!mLinks.empty()) {
            mLinks.front().SetPosition(pt);
        }
    }
    void Chain::AddXY(Vector2f mv) {
        for(auto& link : mLinks) {
            link.AddXY(mv);
        }
    }
    bool Chain::CheckInbounds(RigidBody* body) {
        if(!body) {
            return false;
        }
        float range = (mLinks.size() * mLinkConst.GetMinLength()) + mEndConst.GetMinLength();
        range*= range;
        float dist = body->GetHitBox().GetCenter().Distance(mAnchorPoint);
        return dist <= range;
    }
    void Chain::Attach(RigidBody* obj) {
        mAttachedObject = obj;
        mLinkConstraints.back().SetBodies(&mLinks.back(), mAttachedObject);
    }
    void Chain::Update(double t) {
        /*
        for(size_t i = 0; i < mLinks.size(); i++) {
            mLinks[i].Update(t);
        }
        for(size_t i = 0; i < mNumIter; i++) {
            for(size_t j = 0; j < mLinkConstraints.size(); j++) {
                mLinkConstraints[j].Solve();
            }
        }
        */
        for(size_t i = 0; i < mLinks.size(); i++) {
            mLinks[i].Update(t);
        }
        for(size_t j = 0; j < mLinks.size(); j++) {
            if(j > 0) {
                float r = j * mLinkConst.GetMinLength();
                solstice::Circlef c(mLinks.front().GetPosition(), r);
                if(mLinks[j].GetPosition().Distance(mLinks.front().GetPosition()) > r * r) {
                    mLinks[j].GetHitBox().SetPosition(c.GetClosestPoint(mLinks[j].GetPosition()));
                }
                Vector2f angle = mLinks[j - 1].GetPosition() - mLinks[j].GetPosition();
                mLinks[j].SetAngle(atan2(angle.y, angle.x) + PI * 0.5);
            }
            if(j < mLinks.size() - 1) {
                mLinkConst.Solve(&mLinks[j], &mLinks[j + 1]);
                if(mAttachedObject) {
                    Vector2f angle = mLinks.back().GetPosition() - mAttachedObject->GetPosition();
                    mAttachedObject->SetAngle(atan2(angle.y, angle.x) + PI * 0.5);
                }
            }
        }
        if(mAttachedObject) {
            mEndConst.Solve(&mLinks.back(), mAttachedObject);
        }
        /*
        for(size_t i = 0; i < mLinks.size(); i++) {
            //mLinks[i].Update(t);
            if(i > 0) {
                //Vector2f angle = mLinks[i - 1].GetPosition() - mLinks[i].GetPosition();
                //mLinks[i].SetAngle(atan2(angle.y, angle.x));
            }
            if(mAttachedObject) {
                //Vector2f angle = mLinks.back().GetPosition() - mAttachedObject->GetPosition();
                //mAttachedObject->SetAngle(atan2(angle.y, angle.x));
            }
        }
        */
        /*
        if(endBody) {
            Vector2f angle = endBody->GetPosition() - mLinks.back().GetPosition();
            endBody->SetAngle(atan2(angle.y, angle.x));
        }
        */
    }
    void Chain::DrawHitBoxes(SceneRenderer& rend, Color col) {
        for(size_t i = 0; i < mLinks.size(); i++) {
            rend.DrawRect(mLinks[i].GetHitBox(), col);
        }
    }
    void Chain::Draw(SceneRenderer& rend) {
        QuadData quad = mLinkQuad;
        for(size_t i = 0; i < mLinks.size(); i++) {
            //quad = mLinks[i].GetHitBox();
            //quad.SetPosition(mLinks[i].GetPosition());
            quad.SetXYHW(mLinks[i].GetHitBox().GetPosition(),
                         mLinks[i].GetHitBox().GetHeight() * 2,
                         mLinks[i].GetHitBox().GetWidth() * 2);
            quad.SetAngle(mLinks[i].GetHitBox().GetAngle());
            rend.DrawQuad(quad);
        }
    }
}

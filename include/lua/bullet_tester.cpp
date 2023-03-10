#include "bullet_tester.h"

namespace solstice {
BulletTester::BulletTester() {
    hitBoxColor = Color(0.0, 0.0, 1.0, 0.75);
    enemyColor = Color(1.0, 0.0, 0.0, 1.0);
    targetColor = Color(1.0, 1.0, 0.3, 0.75);
    blastColor = Color(1.0, 0.0, 0.5, 0.25);
    mActiveBullet = 0;
}
void BulletTester::SetupBinds(kaguya::State& state) {
    state["BulletTester"].setClass(kaguya::UserdataMetatable<BulletTester>()
                         .setConstructors<BulletTester()>()
                        .addFunction("AddBullet", &BulletTester::AddBullet)
                        .addFunction("AddSubBullet", &BulletTester::AddSubBullet)
                        .addFunction("GetBullet", &BulletTester::GetBullet)
                        .addFunction("GetSubBullet", &BulletTester::GetSubBullet)
                        .addFunction("GetLastBullet", &BulletTester::GetLastBullet)
                        .addFunction("GetLastSubBullet", &BulletTester::GetLastSubBullet)
                        .addFunction("AddBody", &BulletTester::AddBody)
                        .addFunction("GetTexture", &BulletTester::GetTexture)
                        .addProperty("hitBoxColor", &BulletTester::hitBoxColor)
                        .addProperty("enemyColor", &BulletTester::enemyColor)
                        .addProperty("targetColor", &BulletTester::targetColor)
                        .addProperty("blastColor", &BulletTester::blastColor));
}
void BulletTester::Update(double dt) {
    mAddRenderer.Reset();
    mOpaqueRenderer.Reset();
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i]->Update(dt);
    }
    for(size_t i = 0; i < mSubBullets.size(); i++) {
        mSubBullets[i]->Update(dt);
    }
    for(size_t i = 0; i < mBodies.size(); i++) {
        mBodies[i].Update(dt);
    }
}
void BulletTester::Draw(SceneRenderer& rend) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i]->Draw(rend);
    }
    for(size_t i = 0; i < mSubBullets.size(); i++) {
        mSubBullets[i]->Draw(rend);
    }
    for(size_t i = 0; i < mBodies.size(); i++) {
        rend.DrawRect(mBodies[i].GetHitBox(), enemyColor);
    }
}
void BulletTester::DrawHitboxes(SceneRenderer& rend) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i]->DrawHitBoxes(rend, hitBoxColor);
    }
    for(size_t i = 0; i < mSubBullets.size(); i++) {
        mSubBullets[i]->DrawHitBoxes(rend, hitBoxColor);
    }
}
void BulletTester::DrawRadii(SceneRenderer& rend) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        if(mBullets[i]->GetBaseBullet().GetLocksOn()) {
            mBullets[i]->DrawTargetRanges(rend, 32, targetColor);
        }
        mBullets[i]->DrawBlastRadiuses(rend, 32, blastColor);
    }
    for(size_t i = 0; i < mSubBullets.size(); i++) {
        if(mSubBullets[i]->GetBaseBullet().GetLocksOn()) {
            mSubBullets[i]->DrawTargetRanges(rend, 32, targetColor);
        }
        mSubBullets[i]->DrawBlastRadiuses(rend, 32, blastColor);
    }
}
void BulletTester::DrawParticles(ShaderProgram& sh) {
    mOpaqueRenderer.Render(sh);
    mAddRenderer.Render(sh);
}
void BulletTester::ResetBodies() {
    for(size_t i = 0; i < mBodies.size(); i++) {
        float x = -300 + i * 30;
        mBodies[i].SetPosition(Vector2f(0, 0));
        mBodies[i].Stop();
    }
}
void BulletTester::Reset() {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i]->Reset();
    }
    for(size_t i = 0; i < mSubBullets.size(); i++) {
        mSubBullets[i]->Reset();
    }
}
void BulletTester::ReleaseShots() {
    if(!mBullets.empty()) {
        mBullets[mActiveBullet]->QueueShot();
    }
}
void BulletTester::CheckCollisions(BackgroundTile2& ground) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i]->CheckCollisions(ground);
    }
    for(size_t i = 0; i < mSubBullets.size(); i++) {
        mSubBullets[i]->CheckCollisions(ground);
    }
    for(size_t i = 0; i < mBodies.size(); i++) {
        mBodies[i].CheckAndResolveCollision(ground);
    }
}
void BulletTester::CheckCollisions() {
    for(size_t j = 0; j < mBodies.size(); j++) {
        for(size_t i = 0; i < mBullets.size(); i++) {
            mBullets[i]->CheckCollisions(&mBodies[j]);
        }
        for(size_t i = 0; i < mSubBullets.size(); i++) {
            mSubBullets[i]->CheckCollisions(&mBodies[j]);
        }
    }
}
void BulletTester::InitBullets(kaguya::State& state) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i]->InitBullets(&state);
        mBullets[i]->SetRenderer(&mAddRenderer);
        int subID = mBullets[i]->GetSubBulletID();
        if(subID > -1) {
            mBullets[i]->SetSubBullet(mSubBullets[subID].get());
        }
    }
    if(!mBullets.empty()) {
        std::cout << mSubBullets.size() << std::endl;
        for(size_t i = 0; i < mSubBullets.size(); i++) {
            mSubBullets[i]->InitBullets(&state);
            mSubBullets[i]->SetRenderer(&mAddRenderer);
        }
    }
}
void BulletTester::NextActive() {
    mActiveBullet++;
    if(mActiveBullet >= mBullets.size()) {
        mActiveBullet = 0;
    }
}
void BulletTester::LastActive() {
    mActiveBullet--;
    if(mActiveBullet < 0) {
        mActiveBullet = mBullets.size() - 1;
    }
}
void BulletTester::CreateShot(Vector2f dir, Vector2f pos, Vector2f speed) {
    if(!mBullets.empty()) {
        mBullets[mActiveBullet]->CreateShot(dir, pos, speed);
    }
}
}

#include "bulletcombo.h"

namespace solstice {
BulletCombo::BulletCombo() {
    mCurrentComboStep = -1;
    mCurrentTime = 0;
}
BulletCombo::BulletCombo(std::vector<BulletComboStep>& steps) {
    mComboSteps = steps;
    mCurrentComboStep = -1;
    mCurrentTime = 0;
}
void BulletCombo::Update(double dt) {
    if(mCurrentComboStep > -1 && mCurrentTime < mComboSteps[mCurrentComboStep].mCancelWindow.max) {
        mCurrentTime++;
    }
    for(size_t i = 0; i < mComboSteps.size(); i++) {
        //mComboSteps[i].Update(dt);
    }
    if(mCurrentTime >= mComboSteps[mCurrentComboStep].mCancelWindow.max) {
        mCurrentTime = 0;
        mCurrentComboStep = -1;
    }
}
void BulletCombo::Shoot(Vector2f dir, Vector2f pos, Vector2f vel) {
    int size = mComboSteps.size();
    if(mCurrentComboStep == -1) {
        mCurrentComboStep = 0;
        mComboSteps[mCurrentComboStep++].mBullet->CreateShot(Vector2f(1.0, 0.0));
        mCurrentTime = 0;
    }
    else if(mCurrentComboStep >= 0 && mCurrentComboStep < mComboSteps.size()) {
        if(mCurrentTime >= mComboSteps[mCurrentComboStep].mCancelWindow.min) {
            mComboSteps[mCurrentComboStep++].mBullet->CreateShot(Vector2f(1.0, 0.0));
            mCurrentTime = 0;
        }
    }
}
}

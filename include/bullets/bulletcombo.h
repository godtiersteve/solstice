#ifndef BULLETCOMBO_H_INCLUDED
#define BULLETCOMBO_H_INCLUDED

#include "bullets/bullet.h"
#include "bullets/bullettype.h"
///**TODO : Finish implementing bullet combos***///
namespace solstice {
    class BulletComboStep {
    public:
        std::shared_ptr<BulletType> mBullet;
        ValueRangef mCancelWindow;
        void Update(double dt) {if(mBullet) mBullet->Update(dt);}
        BulletComboStep() {
            mCancelWindow.min = 10;
            mCancelWindow.max = 40;
        }
        BulletComboStep(std::shared_ptr<BulletType> bullet, int comboTime) {
            mBullet = bullet;
            mCancelWindow.min = 15;
            mCancelWindow.max = 40;
        }
    };
    class BulletCombo {
    public:
        std::vector<BulletComboStep> mComboSteps;
        int GetCurrentComboStep() {return mCurrentComboStep;}
        void Update(double dt);
        void Shoot(Vector2f dir, Vector2f pos, Vector2f vel);
        BulletCombo();
        BulletCombo(std::vector<BulletComboStep>& steps);
    protected:
        int mCurrentComboStep;
        float mCurrentTime;

    };
}


#endif // BULLETCOMBO_H_INCLUDED

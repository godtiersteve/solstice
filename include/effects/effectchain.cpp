#include "effectchain.h"

namespace solstice {
    EffectChain::EffectChain() {

    }
    EffectChain::EffectChain(const EffectChain& eff) {
        *this = eff;
    }
    EffectChain& EffectChain::operator=(const EffectChain& eff) {
        mAnimation.clear();
        mCurFrame = eff.mCurFrame;
        mLoops = eff.mLoops;
        mReverses = eff.mReverses;
        for(size_t i = 0; i < eff.mAnimation.size(); i++) {
            mAnimation.push_back(eff.mAnimation[i]);
        }
        return *this;

    }
    void EffectChain::Reverse() {
        std::reverse(mAnimation.begin(), mAnimation.end());
    }
    void EffectChain::Start() {
    }
    void EffectChain::Stop() {
    }
    void EffectChain::Pause(short dur) {
    }
    void EffectChain::Reset() {
    }
    void EffectChain::Update(float dur) {
        if(!mAnimation.empty() && (mLoops > mLoopsPerformed || mLoops < 0)) {
            mUpdatesPerformed++;
            if(mUpdatesPerformed >= mUpdateInterval) {
                mUpdatesPerformed = 0;
                mCurFrame.Update();
                if(mCurFrame.Finished() && mCurFrameNum < mAnimation.size() - 1) {
                    mCurFrameNum++;
                    mCurFrame = mAnimation[mCurFrameNum];
                    mCurFrame.Start();
                }
                else if(mCurFrame.Finished() && mCurFrameNum == mAnimation.size()) {
                    mLoopsPerformed++;
                    if(mReverses) {
                        Reverse();
                    }
                }
            }
        }
    }
    void EffectChain::Update(QuadData q) {
        if(!mAnimation.empty() && (mLoops > mLoopsPerformed || mLoops < 0)) {
            mUpdatesPerformed++;
            if(mUpdatesPerformed >= mUpdateInterval) {
                mUpdatesPerformed = 0;
                mCurFrame.Update(q);
                if(mCurFrame.Finished() && mCurFrameNum < mAnimation.size() - 1) {
                    mCurFrameNum++;
                    mCurFrame = mAnimation[mCurFrameNum];
                    mCurFrame.Start();
                }
                else if(mCurFrame.Finished() && mCurFrameNum == mAnimation.size()) {
                    mLoopsPerformed++;
                    if(mReverses) {
                        Reverse();
                    }
                }
            }
        }
    }
}

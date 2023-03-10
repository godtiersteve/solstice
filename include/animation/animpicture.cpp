#include "animpicture.h"

namespace solstice {
    PictureFrame::PictureFrame() {
        mDuration = 0;
        mElapsed = 0;
    }
    PictureFrame::PictureFrame(unsigned int duration) {
        mDuration = duration;
        mElapsed = 0;
    }
    bool PictureFrame::Update() {
        if(!Finished()) {
            mElapsed++;
            return false;
        }
        else {
            return true;
        }
    }
    void PictureFrame::Reset() {
        mElapsed = 0;
    }
    AnimatedPicture::AnimatedPicture() {
        mHoriFlip =  false;
        mVertFlip = false;
        mHidden = false;
        mColor = COL_EMPTY;
    }
    AnimatedPicture::AnimatedPicture(string ref) {
        mHoriFlip =  false;
        mVertFlip = false;
        mHidden = false;
        mColor = COL_EMPTY;
        mRef = ref;
    }

    void AnimatedPicture::Update() {
        if(mStatus != ANIM_STOPPED && !mAnimation.empty()) {

        }
        else if(mStatus == ANIM_PAUSED) {
            if(mPauseDur > 0) {
                mPauseDur--;
            }
            else if(mPauseDur == 0) {
                Play();
            }
        }
        else {
            if(mCurFrame.Update()) {
                mCurFrameNum++;
                mLoopsPerformed++;
                if(mCurFrameNum >= mAnimation.size()) {
                    if(mLoopsPerformed < mLoops || mLoops < 0) {
                        mCurFrameNum = mLoopFrame;
                        if(mReverses) {
                            Reverse();
                        }
                    }
                    else {
                        mCurFrameNum = mAnimation.size() - 1;
                    }
                }
                mCurFrame = mAnimation[mCurFrameNum];
            }
        }
    }
}

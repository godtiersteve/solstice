#include "particleanim.h"

namespace solstice {
void ParticleAnimation::Update(float dt) {
	if(mAnimation.empty()) {
		return;
	}
	ParticleAnimFrame f = mAnimation[0];
	if(mStatus == ANIM_PAUSED) {
		if(mPauseDur > 0)
			mPauseDur--;
		else if(mPauseDur == 0)
			Play();
	}
	else if(mStatus != ANIM_STOPPED && !Finished()) {
		if(mCurFrame.Finished()) {
			if(mCurFrameNum >= mAnimation.size() - 1) {
				if(mLoops == -1) {
					mCurFrameNum = mLoopFrame;
				}
				else if(mLoops == -2) {
					mCurFrameNum = mLoopFrame;
					Reverse();
				}
				else if(mLoops >= 0) {
					mLoops--;
					if(mLoops < 0) {
						mLoops = 0;
					}
					else {
						mCurFrameNum = mLoopFrame;
					}
					mLoopsPerformed++;
				}
			}
			else if(mCurFrameNum < mAnimation.size() - 1) {
				mCurFrameNum++;
			}
			if(mCurFrameNum >= mAnimation.size()) {
				mCurFrameNum = 0;
			}
			mCurFrame = mAnimation[mCurFrameNum];
		}
		else {
			mCurFrame.Update();
		}
	}
}

}

#include "imageeffect.h"

namespace solstice {
    ImageEffect::ImageEffect() {
        Init();
    }
    ImageEffect::ImageEffect(ImageTransform2 start, ImageTransform2 end) {
        Init();
        mAnimation.push_back(start);
        mAnimation.push_back(end);
        mCurFrame = mAnimation[0];
        mDuration = mCurFrame.duration;
    }
    ImageEffect::ImageEffect(const ImageEffect& eff) {
        *this = eff;
    }
    ImageEffect& ImageEffect::operator=(const ImageEffect& eff) {
        mCurFrame = eff.mCurFrame;
        mDuration = eff.mDuration;
        mUpdatesPerformed = eff.mUpdatesPerformed;
        mLoopsPerformed = eff.mLoopsPerformed;
        mStepPerFrame = eff.mStepPerFrame;
        mHasShadow = eff.mHasShadow;
        mHasAfterImage = eff.mHasAfterImage;
        mReverses = eff.mReverses;
        mLoops = eff.mLoops;
        mShadowAlpha = eff.mShadowAlpha;
        mShadowOffset = eff.mShadowOffset;
        mShadowColor = eff.mShadowColor;
        mAfterImgLife = eff.mAfterImgLife;
        mAfterImageTimer = eff.mAfterImageTimer;
        mAfterImgTimerCur = eff.mAfterImgTimerCur;
        mStatus = eff.mStatus;
        mQuad = eff.mQuad;
        mAnimation.clear();
        for(size_t i = 0; i < eff.mAnimation.size(); i++) {
            mAnimation.push_back(eff.mAnimation[i]);
        }
        return *this;
    }
    void ImageEffect::Init() {

        mDuration = 0;
        mUpdatesPerformed = 0;
        mLoopsPerformed = -1;
        mStepPerFrame = 0;
        mHasShadow = false;
        mHasAfterImage = false;
        mReverses = false;
        mLoops = 1;
        mShadowAlpha = 0.75f;
        mShadowOffset = Vector2f(5, -5);
        mShadowColor = COL_BLACK;
        mAfterImgLife = 5;
        mAfterImageTimer = 5;
        mAfterImgTimerCur = 0;
        mStatus = ANIM_STOPPED;
        mQuad = NULL;
    }
    void ImageEffect::AddEffectPoint(ImageTransform2 effect)  {
        mAnimation.push_back(effect);
        Reset();
    }
    void ImageEffect::AddEffectPoint(ImageTransform2 effect, size_t i) {
        mAnimation.insert(mAnimation.begin() + i, effect);
        Reset();
    }
    ImageTransform2& ImageEffect::GetEffect(size_t i)   {return mAnimation[i];}
    void ImageEffect::Reverse() {
        if(mAnimation.size() == 2) {
            mAnimation[1].duration = mAnimation[0].duration;
            mAnimation[0].duration = 0;
        }
        std::reverse(mAnimation.begin(), mAnimation.end());
    }
    void ImageEffect::Start() {
        Play();
        Update();
    }
    void ImageEffect::Start(QuadData& q) {
        Play();
        Update(q);
    }
    void ImageEffect::Start(Picture& q) {
        Play();
        Update(q);
    }
    void ImageEffect::Play() {
        if(mStatus == ANIM_STOPPED) {
            Reset();
        }
        mStatus = ANIM_PLAYING;
    }
    void ImageEffect::Stop() {
        Reset();
        mStatus = ANIM_STOPPED;
    }
    void ImageEffect::Pause(short duration) {
        mStatus = ANIM_PAUSED;
    }
    void ImageEffect::Update(float dur) {
        if(mQuad != NULL) {
            Update(*mQuad);
        }
    }
    void ImageEffect::Update(QuadData& q) {
        if(mStatus == ANIM_STOPPED || mAnimation.empty()) {
            return;
        }
        else if(mStatus == ANIM_PAUSED) {
            if(mPauseDur > 0) {
                mPauseDur--;
                return;
            }
            else if(mPauseDur == 0) {
                mStatus = ANIM_PLAYING;
            }
        }
        if(mAnimation.size() == 1) {
            mCurFrame = mAnimation[0];
            TransformQuad(q);
            return;
        }
        if(mLoops > mLoopsPerformed || mLoops < 0) {
            if(mUpdatesPerformed == 0) {
                mCurFrame = mAnimation[mCurFrameNum];
                mDuration = mCurFrame.duration;
                TransformQuad(q);
                CalculateSteps();
                Step();
                mUpdatesPerformed++;
            }
            else if(mUpdatesPerformed < mDuration) {
                Step();
                TransformQuad(q);
                mUpdatesPerformed++;
            }
            else if(mUpdatesPerformed >= mDuration) {
                mCurFrameNum++;
                mUpdatesPerformed = 0;
                if(mCurFrameNum >= mAnimation.size()) {
                    mLoopsPerformed++;
                    mCurFrameNum = 0;
                    if(mReverses && (mLoops > mLoopsPerformed || mLoops < 0)) {
                        Reverse();
                    }
                }
            }
        }
        if(mHasAfterImage) {
            for(size_t i = 0; i < mAfterImages.size(); i++) {
                if(mAfterImages[i].color.alpha <= 0 || mAfterImages[i].lifespan <= 0) {
                    mAfterImages.pop_front();
                }
                mAfterImages[i].color.alpha-= 1.0f / mAfterImgLife;
            }
        }
    }
    void ImageEffect::Update(MultiImage& q) {
        if(mStatus == ANIM_STOPPED || mAnimation.empty()) {
            return;
        }
        else if(mStatus == ANIM_PAUSED) {
            if(mPauseDur > 0) {
                mPauseDur--;
                return;
            }
            else if(mPauseDur == 0) {
                mStatus = ANIM_PLAYING;
            }
        }
        if(mAnimation.size() == 1) {
            mCurFrame = mAnimation[0];
            TransformMultiQuad(q);
            return;
        }
        if(mLoops > mLoopsPerformed || mLoops < 0) {
            if(mUpdatesPerformed == 0) {
                mCurFrame = mAnimation[mCurFrameNum];
                mDuration = mCurFrame.duration;
                TransformMultiQuad(q);
                CalculateSteps();
                Step();
                mUpdatesPerformed++;
            }
            else if(mUpdatesPerformed < mDuration) {
                Step();
                TransformMultiQuad(q);
                mUpdatesPerformed++;
            }
            else if(mUpdatesPerformed >= mDuration) {
                mCurFrameNum++;
                mUpdatesPerformed = 0;
                if(mCurFrameNum >= mAnimation.size()) {
                    mLoopsPerformed++;
                    mCurFrameNum = 0;
                    if(mReverses && (mLoops > mLoopsPerformed || mLoops < 0)) {
                        Reverse();
                    }
                }
            }
        }
        if(mHasAfterImage) {
            for(size_t i = 0; i < mAfterImages.size(); i++) {
                if(mAfterImages[i].color.alpha <= 0 || mAfterImages[i].lifespan <= 0) {
                    mAfterImages.pop_front();
                }
                mAfterImages[i].color.alpha-= 1.0f / mAfterImgLife;
            }
        }
    }
    bool ImageEffect::Finished() {
        return mLoopsPerformed == mLoops;
    }
    void ImageEffect::AddFade(float initial, float end, int steps) {
        ImageTransform2 transform;
        transform.fade = initial;
        transform.duration = steps;
        mAnimation.push_back(transform);
        transform.fade = end;
        transform.duration = 0;
        mAnimation.push_back(transform);
    }
    void ImageEffect::AddPulse(float startScale, float endScale, int steps) {
        AddScale(startScale, endScale, steps);
        mLoops = -1;
        mReverses = true;
    }
    void ImageEffect::AddSlide(Vector2f start, Vector2f end, Vector2f velocity, int steps) {
        ImageTransform2 transform;
        transform.velocity = velocity;
        transform.destination = start;
        transform.duration = steps;
        mAnimation.push_back(transform);
        transform.velocity = 0;
        transform.destination = end;
        transform.duration = 0;
        mAnimation.push_back(transform);
    }
    void ImageEffect::AddSpin(float startAngle, float endAngle, int steps) {
        ImageTransform2 transform;
        transform.angle = startAngle;
        transform.duration = steps;
        mAnimation.push_back(transform);
        transform.angle = endAngle;
        transform.duration = 0;
        mAnimation.push_back(transform);
    }
    void ImageEffect::AddScale(float startScale, float endScale, int steps) {
        ImageTransform2 transform;
        transform.scale = startScale;
        transform.duration = steps;
        mAnimation.push_back(transform);
        transform.scale = endScale;
        transform.duration = 0;
        mAnimation.push_back(transform);
    }
    void ImageEffect::AddAfterImage(unsigned int afterImgTimer, unsigned int afterImgLife) {
        mAfterImageTimer = afterImgTimer;
        mAfterImgLife = afterImgLife;
        mHasAfterImage = true;
    }
    void ImageEffect::AddTransform(ImageTransform2 start, ImageTransform2 end) {
        mAnimation.push_back(start);
        mAnimation.push_back(end);
    }
    void ImageEffect::Step() {
        if(mCurFrameNum >= mAnimation.size() - 1) {
            return;
        }
        mCurFrame.angle+= mCurrentStep.angleStep;
        mCurFrame.xSkew+= mCurrentStep.xSkewStep;
        mCurFrame.ySkew+= mCurrentStep.ySkewStep;
        mCurFrame.fade+= mCurrentStep.fadeStep;
        mCurFrame.scale+= mCurrentStep.scaleStep;
        mCurFrame.velocity+= mCurrentStep.acceleration;
    }
    void ImageEffect::Reset() {
        if(mAnimation.empty()) {
           return;
        }
        mCurFrame = mAnimation[0];
        mCurFrameNum = 0;
        mLoopsPerformed = 0;
        mUpdatesPerformed = 0;
    }
    void ImageEffect::CalculateSteps() {
        if(mAnimation.size() <= 1 || mCurFrameNum >= mAnimation.size() - 1) {
            return;
        }
        ImageTransform2& start = mAnimation[mCurFrameNum];
        ImageTransform2& end = mAnimation[mCurFrameNum + 1];
        int dur = start.duration;

        mCurrentStep.angleStep = (end.angle - start.angle) / dur;
        mCurrentStep.xSkewStep = (end.xSkew - start.xSkew) / dur;
        mCurrentStep.ySkewStep = (end.ySkew - start.ySkew) / dur;
        mCurrentStep.fadeStep = (end.fade - start.fade) / dur;
        mCurrentStep.scaleStep = (end.scale - start.scale) / dur;
        mCurrentStep.acceleration.x = (end.velocity.x - start.velocity.x) / dur;
        mCurrentStep.acceleration.y = (end.velocity.y - start.velocity.y) / dur;
        mCurrentStep.velocity.x = (end.destination.x - start.destination.x) / dur;
        mCurrentStep.velocity.y = (end.destination.y - start.destination.y) / dur;
        mCurFrame.velocity = mCurrentStep.velocity;
    }
    void ImageEffect::TransformQuad(QuadData& q) {
        mCurFrame.TransformQuad(q);
        if(mHasAfterImage) {
            if(mAfterImgTimerCur == 0) {
                Color afterImage;
                afterImage.alpha = 0.75f;
                mAfterImages.push_back(AfterImage(q.GetOrigin(), 10, mCurFrame, afterImage));
                mAfterImgTimerCur = mAfterImageTimer;
            }
            mAfterImgTimerCur--;
        }
    }
    void ImageEffect::TransformMultiQuad(MultiImage& q) {
        mCurFrame.TransformMultiQuad(q);
        if(mHasAfterImage) {
            if(mAfterImgTimerCur == 0) {
                Color afterImage;
                afterImage.alpha = 0.75f;
                mAfterImages.push_back(AfterImage(q.GetOrigin(), 10, mCurFrame, afterImage));
                mAfterImgTimerCur = mAfterImageTimer;
            }
            mAfterImgTimerCur--;
        }
    }

    /*
    void ImageEffect::Draw(Renderer& r, QuadData& q) {
        if(mHasAfterImage && !mAfterImages.empty()) {
            QuadData afterImg = q;
            for(size_t i = 0; i < mAfterImages.size(); i++) {
                QuadData afterImg = q;
                mAfterImages[i].transform.TransformQuad(afterImg);
                afterImg.SetXY(mAfterImages[i].position);
                r.DrawQuad(afterImg, mAfterImages[i].color);
            }
        }
        if(mHasShadow) {
            mShadowColor.alpha = mShadowAlpha * q.GetColor().alpha;
            r.DrawSilhouette(q, mShadowOffset, mShadowColor);
        }
        r.DrawQuad(q);
    }
    */
}

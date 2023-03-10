#ifndef IMAGEEFFECT_H
#define IMAGEEFFECT_H

#include "imagetransform2.h"
#include "geometry/rectangle.h"
#include "graphics/quaddata.h"
#include "animation/animator.h"

#include <deque>
#include <algorithm>
//TODO :  Add acceleration to rotation and scaling
//TODO : Add shaking to image effects (VERTICAL_SHAKE, HORIZONTAL_SHAKE, SHAKE)
namespace solstice {
class ImageEffect : public Animator<ImageTransform2> {
    private:
        std::deque<AfterImage> mAfterImages;
        unsigned int mAfterImgLife, mAfterImageTimer, mAfterImgTimerCur;
        unsigned int mDuration;
        float mStepPerFrame;
        bool mHasShadow, mHasAfterImage;
        Vector2f mShadowOffset;
        Color mShadowColor;
        float mShadowAlpha;
        ImageTransform2 mActiveTransform;
        ImageTransform2Step mCurrentStep;
        QuadData* mQuad;

        void CalculateSteps();
        void Init();
        void Step();
    public:
        ImageEffect();
        ImageEffect(ImageTransform2 start, ImageTransform2 end);
        ImageEffect(const ImageEffect& eff);
        ImageEffect& operator=(const ImageEffect& eff);

        void SetHasShadow(bool f)                       {mHasShadow = f;}
        void SetHasAfterImage(bool f)                   {mHasAfterImage = f;}
        void SetReverses(bool f)                        {mReverses = f;}
        void SetLoops(int loops)                        {mLoops = loops;}
        void SetShadowOffset(Vector2f offset)           {mShadowOffset = offset;}
        void SetShadowAlpha(float a)                    {mShadowAlpha = a;}
        void SetShadowColor(Color c)                    {mShadowColor = c;}
        void SetAfterImageLife(unsigned int life)       {mAfterImgLife = life;}
        void SetAfterImageTimer(unsigned int timer)     {mAfterImageTimer = timer;}
        void SetQuad(QuadData* q)                       {mQuad = q;}

        bool GetHasShadow()                         {return mHasShadow;}
        bool GetHasAfterImage()                     {return mHasAfterImage;}
        bool GetReverses()                          {return mReverses;}
        int GetLoops()                              {return mLoops;}
        ImageTransform2& GetActiveTransform()       {return mActiveTransform;}
        AnimationStatus GetStatus()                 {return mStatus;}
        Color GetShadowColor()                      {return mShadowColor;}
        float GetShadowAlpha()                      {return mShadowAlpha;}
        Vector2f GetShadowOffset()                  {return mShadowOffset;}
        unsigned int GetAfterImgTimer()             {return mAfterImageTimer;}
        unsigned int GetAfterImgTimerCur()          {return mAfterImgTimerCur;}
        unsigned int GetAfterImgLife()              {return mAfterImgLife;}
        QuadData& GetQuad()                         {return *mQuad;}

        void AddEffectPoint(ImageTransform2 effect);
        void AddEffectPoint(ImageTransform2 effect, size_t i);
        ImageTransform2& GetEffect(size_t i);

        //TODO : Implement the helper functions for adding certain effects quickly
        void AddFade(float initial, float end, int steps);
        void AddPulse(float startScale, float endScale, int steps);
        void AddSlide(Vector2f start, Vector2f end, Vector2f velocity, int steps);
        void AddSpin(float startAngle, float endAngle, int steps);
        void AddScale(float startScale, float endScale, int steps);
        void AddAfterImage(unsigned int afterImgTimer, unsigned int afterImgLife);
        void AddTransform(ImageTransform2 start, ImageTransform2 end);

        void Reverse();
        void Play();
        void Start();
        void Start(QuadData& q);
        void Start(Picture& q);
        void Stop();
        void Pause(short duration = -1);
        void Reset();
        void Update(float dur  = 1.0f);
        void Update(QuadData& q);
        void Update(MultiImage& q);
        bool Finished();

        void TransformQuad(QuadData& q);
        void TransformMultiQuad(MultiImage& q);
};
}
#endif // IMAGEEFFECT_H

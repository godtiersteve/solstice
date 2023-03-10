#ifndef ANIMPICTURE_H_INCLUDED
#define ANIMPICTURE_H_INCLUDED

#include "animation/animator.h"
#include "graphics/multiquad.h"

namespace solstice {
class PictureFrame : Picture {
    private:
        unsigned int mDuration, mElapsed;
    public:
        PictureFrame();
        PictureFrame(unsigned int duration);

        unsigned int GetDuration()          {return mDuration;}
        unsigned int GetElapsed()           {return mElapsed;}
        bool Finished()                     {return mElapsed == mDuration;}
        void SetElapsed(unsigned int i)     {mElapsed = i;}
        void SetDuration(unsigned int i)    {mDuration = i ;}
        bool Update();
        void Reset();
};
class AnimatedPicture : public Animator<PictureFrame> {
    protected:
        Vector2f mPosition;
        bool mHoriFlip, mVertFlip;
        string mRef;
        Color mColor;
        bool mHidden;
    public:
        AnimatedPicture();
        AnimatedPicture(string ref);

        void SetColor(Color c)      {mColor = c;}
        Color& GetColor()           {return mColor;}
        bool GetHoriFlip()          {return mHoriFlip;}
        bool GetVertFlip()          {return mVertFlip;}

        void SetHoriFlip(float flip);
        void SetVertFlip(float flip);
        void SetFlip(float hori, float vert);

        void Update();
};
}

#endif // ANIMPICTURE_H_INCLUDED

#include "framedata.h"
namespace solstice {
FrameData::FrameData():
mScale(1.0f), mDuration(0), mElapsed(0), mDrawOffset(0), mFinished(false), mTextureID(-1), mAngle(0)
{
}
FrameData::FrameData(Vector2<float> pos, TextureData tex, short duration):
mScale(1.0f), mDuration(duration), mElapsed(0), mDrawOffset(pos), mFinished(false), mTextureID(-1), mAngle(0)
{
    *this = tex;
}
FrameData& FrameData::operator=(TextureData data) {
    SetTexHW(data.GetTexHeight(), data.GetTexWidth());
    Init(data.GetTopLeft(),
                    data.GetTopRight(),
                    data.GetBotRight(),
                    data.GetBotLeft());
    return *this;
}
void            FrameData::SetDuration(float dur)           {mDuration = dur;}
void            FrameData::SetScale(float scale)            {mScale = scale;}
void            FrameData::SetXOffset(float x)              {mDrawOffset.x = x;}
void            FrameData::GetYOffset(float y)              {mDrawOffset.y = y;}
void            FrameData::SetXYOffset(Vector2<float> off)  {mDrawOffset = off;}
void            FrameData::SetOffset(Vector2<float> off)    {mDrawOffset = off;}
void            FrameData::SetAngle(float angle)            {mAngle = angle;}
Vector2f        FrameData::GetDrawOffset()                  {return mDrawOffset;}
float           FrameData::GetScale()                       {return mScale;}
float           FrameData::GetDuration()                    {return mDuration;}
float           FrameData::GetElapsed()                     {return mElapsed;}
bool            FrameData::GetFinished()                    {return mFinished;}
float           FrameData::GetAngle()                       {return mAngle;}
bool            FrameData::Finished()                       {return mFinished;}
void FrameData::Update(float dur) {
    if(mDuration && mElapsed < mDuration) {
        mElapsed+= dur;
    }
    if(mElapsed >= mDuration)
        mFinished = true;
    else
        mFinished = false;
}
void FrameData::Reset() {
    mFinished = false;
    mElapsed = 0;
}

TextureData FrameData::GetTexData() {return *this;}
};

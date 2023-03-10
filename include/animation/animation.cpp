#include "animation.h"

namespace solstice {
    Animation::Animation():
    mHoriFlip(false), mVertFlip(false),
    mClip(32), mPosition(0),
    mTexH(128), mTexW(128),
    mSheet(false), mHidden(false), mAngle(0)
    {
        mLoopsPerformed = 0;
        SetDrawFrom(DRAW_CENTER);
        mTextureID = -1;
        mRotatesAboutPoint = false;
        mBaseAngle = 0;
        mRotationSpeed = 0;
        mXScaling = 1.0;
        mYScaling = 1.0;
        mAlphaStep = 0;
        mStartAngle = 0;
        mBaseAlpha = 1.0;
        mOriginalSize = 32;
    }
    Animation::Animation(vector<Frame> frames):
    mHoriFlip(false), mVertFlip(false),
    mClip(32), mPosition(0),
    mTexH(128), mTexW(128),
    mSheet(false), mHidden(false), mAngle(0)
    {
        mLoopsPerformed = 0;
        mAnimation = frames;
        SetDrawFrom(DRAW_CENTER);
        mTextureID = -1;
        mRotatesAboutPoint = false;
        mBaseAngle = 0;
        mRotationSpeed = 0;
        mXScaling = 1.0;
        mYScaling = 1.0;
        mAlphaStep = 0;
        mStartAngle = 0;
        mBaseAlpha = 1.0;
        mOriginalSize = 32;
    }
    Animation::operator QuadData&() {
        return mDrawQuad;
    }
    Animation& Animation::Copy(const Animation& anim) {
        mHoriFlip = anim.mHoriFlip;
        mVertFlip = anim.mVertFlip;
        mDrawQuad = anim.mDrawQuad;
        mRef = anim.mRef;
        mColor = anim.mColor;
        mClip = anim.mClip;
        mPosition = anim.mPosition;
        mTexH = anim.mTexH;
        mTexW = anim.mTexW;
        mSheet = anim.mSheet;
        mHidden = anim.mHidden;
        mAngle = anim.mAngle;
        mTransforms = anim.mTransforms;
        mLoops = anim.mLoops;
        mLoopsPerformed = anim.mLoopsPerformed;
        mUpdateInterval = anim.mUpdateInterval;
        mUpdatesPerformed = anim.mUpdatesPerformed;
        mPauseDur = anim.mPauseDur;
        mLoopFrame = anim.mLoopFrame;
        mCurFrame = anim.mCurFrame;
        mCurFrameNum = anim.mCurFrameNum;
        mReverses = anim.mReverses;
        mAnimation = anim.mAnimation;
        mStatus = anim.mStatus;
        mAnimation = anim.mAnimation;
        mTextureID = anim.mTextureID;
        mRotatesAboutPoint = anim.mRotatesAboutPoint;
        mRotateOrigin = anim.mRotateOrigin;
        mBaseAngle = anim.mBaseAngle;
        mRotationSpeed = anim.mRotationSpeed;
        mXScaling = anim.mXScaling;
        mYScaling = anim.mYScaling;
        mAlphaStep = anim.mAlphaStep;
        mBaseAlpha = anim.mBaseAlpha;
        mStartAngle = anim.mStartAngle;
        mOriginalSize = anim.mOriginalSize;
        return *this;
    }
    void Animation::FixTextureIDs(TextureArray& texArr) {
        for(unsigned int i = 0; i < mAnimation.size(); i++) {
            mAnimation[i].SetTextureID(texArr.GetTextureID(mAnimation[i].GetTextureRef()));
        }
    }
    size_t Animation::size()  const           {return mAnimation.size();}
    void Animation::push_back(Frame f)  {mAnimation.push_back(f);}
    void Animation::pop_back()			{mAnimation.pop_back();}
    void Animation::erase(int i)		{mAnimation.erase(mAnimation.begin() +i);}

    void Animation::Init(Frame frame) {
        mAnimation.clear();
        mAnimation.push_back(frame);
        mLoops = -1;
        mHoriFlip = false;
        mVertFlip = false;
        mColor = frame.GetColor();
        mSheet = false;
        mDrawQuad.SetTextureCoords(frame);
        mCurFrame = frame;
        mHidden = false;
    }

    void Animation::AutoAddFramesSep(const char* prefix, unsigned short amount, unsigned short digits) {
        const int numSize = digits + 1;
        char* num = new char[numSize];
        for(unsigned int i = 0; i < amount; i++) {
            TextureData texData(Vector2<float>(0, 1),
                                1,
                                Vector2<float>(1, 0),
                                0);
            Frame foo(texData, 5);

            SetStrNum(num, i, digits);
            const int textrefSize = strlen(prefix) + strlen(num) + 1;
            char* texref = new char[textrefSize];
            strcpy(texref, prefix);
            strcat(texref, num);
            foo.SetTextureRef(texref);
            foo.SetTextureID(-1);
            mAnimation.push_back(foo);
            mAnimation.back().SetTextureID(-1);
            strcpy(num, "");
            strcpy(texref, "");
            delete[] texref;
        }
        mCurFrameNum = 0;
        mCurFrame = mAnimation[0];
        mSheet = false;
        delete[] num;
    }
    void Animation::AutoAddFramesSheet(string texref, unsigned short amount) {
        for(unsigned int i = 0; i < amount; i++) {
            TextureData texData(Vector2f((i * mClip.x), mTexH - mClip.y),
                                mClip.y, mClip.x,
                                mTexH, mTexW);
            Frame foo(texData, 5);
            foo.SetTextureRef(texref);
            foo.SetTextureID(mTextureID);
            mAnimation.push_back(foo);
        }
        mCurFrameNum = 0;
        mCurFrame = mAnimation[0];
        mSheet = true;
    }
    void Animation::AutoAddFramesSheet(int amt, float clipH, float clipW, float row, float col) {
        int colOffset = col;
        int maxCols = mTexW / clipW;
        for(int i = 0; i < amt; i++) {
            Frame frame;
            if(colOffset >= maxCols) {
                colOffset = 0;
                row++;
            }
            Vector2f origin(colOffset * clipW, row * clipH);
            colOffset++;
            frame.Init(origin, clipH, clipW, mTexH, mTexW);
            frame.SetTextureID(mTextureID);
            mAnimation.push_back(frame);
        }
        mCurFrameNum = 0;
        if(!mAnimation.empty()) {
            mCurFrame = mAnimation[0];
        }
        mSheet = true;
    }
    void Animation::AutoAddFramesSheet(string texref, int amt, float clipH, float clipW, float row, float col) {
        SetClip(clipH, clipW);
        for(int i = 0; i < amt; i++) {
            TextureData texData(Vector2f(col + (i * clipW), mTexH - clipH * row),
                                clipH, clipW,
                                mTexH, mTexW);
            Frame foo(texData, 5);
            foo.SetTextureRef(texref);
            foo.SetTextureID(-1);
            mAnimation.push_back(foo);
        }
        mCurFrameNum = 0;
        mCurFrame = mAnimation[0];
        mSheet = true;
    }
    void Animation::Clear() {
        mAnimation.clear();
    }
    Frame& Animation::operator[](unsigned short i) {
        if(i < mAnimation.size())
            return mAnimation[i];
        else
            return mAnimation[0];
    }
    Frame& Animation::GetFrame(unsigned short i) {
        if(i < mAnimation.size())
            return mAnimation[i];
        else {
            return mAnimation[0];
        }
    }
    void Animation::SetTexHW(float h, float w) {mTexH = h; mTexW = w;}
    void Animation::SetClip(Vector2f clip)      {mClip = clip; mOriginalSize = clip;}
    void Animation::SetClip(float h, float w)   {mClip.x = h; mClip.y = w; mOriginalSize = mClip;}
    void Animation::SetPosition(Vector2<float> pos) {mPosition = pos;}
    void Animation::SetHidden(bool hidden)  {mHidden = hidden;}
    void Animation::SetTextureRef(string ref) {
        for(size_t i = 0; i < mAnimation.size(); i++) {
            mAnimation[i].SetTextureRef(ref);
        }
    }
    void Animation::SetTextureID(int id) {
        mTextureID = id;
        for(size_t i = 0; i < mAnimation.size(); i++) {
            mAnimation[i].SetTextureID(id);
        }
    }
    void Animation::SetTextureIDRange(int id, int start) {
        for(size_t i = start; i < mAnimation.size(); i++) {
            mAnimation[i].SetTextureID(id);
        }
    }
    void Animation::SetTextureIDRange(int id, int start, int end) {
        if(end >= mAnimation.size()) {
            end = mAnimation.size();
        }
        for(size_t i = start; i < end; i++) {
            mAnimation[i].SetTextureID(id);
        }
    }
    void Animation::SetLoop(short loop, unsigned short frame) {
        mLoops = loop;
        if(mLoopFrame >= mAnimation.size())
            mLoopFrame = frame;
    }

    void Animation::FlipBoth(bool flip) {
        mHoriFlip = flip;
        mVertFlip = flip;
    }
    void Animation::SetColor(Color col) {
        mColor = col;
        for(unsigned int i = 0; i < mAnimation.size(); i++)
            mAnimation[i].SetColor(col);
        mCurFrame.SetColor(col);
        mBaseAlpha = col.alpha;
    }

    void Animation::ForceFrame(unsigned short i) {
        if(i < mAnimation.size()) {
            mCurFrameNum = i;
            mCurFrame = mAnimation[i];
            UpdateDrawQuad();
        }
    }
    void Animation::DuplicateFrame(unsigned short frame, unsigned short amt) {
        if(frame >= mAnimation.size()) {
            return;
        }
        else {
            for(int i = 0; i < amt; i++) {
                mAnimation.insert(mAnimation.begin() + 1, mAnimation[i]);
            }
        }
    }
    void Animation::AddMotion(unsigned short start, unsigned short end, Vector2<float> distance) {
        if(start < 0 || end > mAnimation.size() - 1 || end < start) {
            return;
        }
        else {
            int xDistance = distance.x / (end - start);
            int yDistance = distance.y / (end - start);
            for(int i = start; i < end; i++) {
                mAnimation[i].SetXYOffset(Vector2<float>(xDistance * i, yDistance * i));
            }
        }
    }
    void Animation::AddRotation(unsigned short start, unsigned short end, float rotation) {
        for(unsigned int i = start; i < mAnimation.size() && i < end; i++) {
            mAnimation[i].SetAngle(rotation / (end - start));
        }
    }
    void Animation::AddScale(unsigned short start, unsigned short end, float scale) {
        for(unsigned int i = start; i < mAnimation.size() && i < end; i++) {
            mAnimation[i].SetScale(scale / (end - start));
        }
    }
    void Animation::AddFade(unsigned short start, unsigned short end, float scale) {
    }
    void Animation::SetDuration(float dur) {
        for(unsigned int i = 0; i < mAnimation.size(); i++) {
            mAnimation[i].SetDuration(dur);
        }
        mCurFrame.SetDuration(dur);
    }
    void Animation::SetOffset(Vector2f offset) {
        for(unsigned int i = 0; i < mAnimation.size(); i++) {
            mAnimation[i].SetOffset(offset);
        }
        mCurFrame.SetOffset(offset);
    }

    void Animation::SetDurationRan(unsigned short start, unsigned short end, unsigned short dur) {
        if(start < mAnimation.size() && end < mAnimation.size()) {
            for(unsigned int i = start; i < end; i++) {
                mAnimation[i].SetDuration(dur);
            }
        }
    }
    void Animation::SetFrameDur(unsigned short frame, unsigned short dur) {
        if(frame < mAnimation.size())
            mAnimation[frame].SetDuration(dur);
    }
    void                Animation::SetDrawFrom(DrawPoint pt) {mDrawQuad.SetDrawFrom(pt);}
    short               Animation::GetLoop()            {return mLoops;}
    bool                Animation::HoriFlipped()        {return mHoriFlip;}
    bool                Animation::VertFlipped()        {return mVertFlip;}
    unsigned short      Animation::GetLoopFrame()       {return mLoopFrame;}
    Frame               Animation::GetCurFrame()        {return mCurFrame;}
    unsigned short      Animation::GetCurFrameNum()     {return mCurFrameNum;}
    Color               Animation::GetColor()           {return mColor;}
    Vector2<float>      Animation::GetClip()            {return mClip;}
    float               Animation::GetClipH()           {return mClip.x;}
    float               Animation::GetClipW()           {return mClip.y;}
    float               Animation::GetTexH()            {return mTexH;}
    float               Animation::GetTexW()            {return mTexW;}
    bool                Animation::Loops()              {return mLoops;}
    QuadData&           Animation::GetDrawQuad()        {
        UpdateDrawQuad();
        return mDrawQuad;
    }
    Vector2<float>      Animation::GetPosition()        {return mPosition;}
    bool                Animation::Done()               {return Finished();}
    bool                Animation::GetHidden()          {return mHidden;}
    void Animation::Update(float dur) {
        if(mAnimation.empty()) {
            mHidden = true;
            return;
        }
        if(mStatus == ANIM_PAUSED) {
            if(mPauseDur > 0) {
                mPauseDur-= dur;
            }
            else if(mPauseDur <= 0) {
                mPauseDur = 0;
                Play();
            }
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
                mCurFrame.Update(dur);
            }
        }
        mColor.alpha+= mAlphaStep;
        mClip.x*= mXScaling;
        mClip.y*= mYScaling;
        mAngle+= mRotationSpeed;
    }
    void Animation::CopyFrames(const Animation& anim) {
        mAnimation = anim.mAnimation;
    }
    void Animation::UpdateDrawQuad() {
        if(mAnimation.empty()) {
            mDrawQuad.SetColor(Color(0.0, 0.0, 0.0, 0.0));
        }
        else {
            mDrawQuad.SetHW(mClip.y, mClip.x);
            mDrawQuad.SetTextureCoords(mCurFrame.GetTexData());
            mDrawQuad.SetHoriFlip(mHoriFlip);
            mDrawQuad.SetVertFlip(mVertFlip);
            mDrawQuad.SetTextureID(mCurFrame.GetTextureID());
            if(!mColor) {
                mDrawQuad.SetColor(mCurFrame.GetColor());
            }
            else {
                mDrawQuad.SetColor(mColor);
            }
            Vector2f offset = mCurFrame.GetDrawOffset();
            offset.Rotate(mCurFrame.GetAngle() + mAngle);
            Vector2f position = mPosition - offset;
            mDrawQuad.SetXY(position);
            mDrawQuad.SetHidden(mHidden);
            if(mRotatesAboutPoint) {
                mDrawQuad.RotateAboutPoint(mCurFrame.GetAngle() + mAngle, mRotateOrigin);
            }
            else {
                mDrawQuad.SetAngle(mCurFrame.GetAngle() + mAngle);
            }
        }
    }
    void Animation::Start() {
        Play();
        Update();
    }
    void Animation::Play() {
        mStatus = ANIM_PLAYING;
        if(!mAnimation.empty()) {
            mCurFrame = mAnimation[0];
        }
    }
    void Animation::Pause(double dur) {
        mStatus = ANIM_PAUSED;
        mPauseDur = dur;
    }
    void Animation::Stop() {
        //ForceFrame(0);
        mStatus = ANIM_STOPPED;
        mPauseDur = 0;
        mLoopsPerformed = 0;
    }

    void Animation::Reset() {
        ForceFrame(0);
        Stop();
        Start();
        mColor.alpha = mBaseAlpha;
        mAngle = mStartAngle;
        ResetSize();
    }

    void Animation::Reverse() {
        Reset();
        std::reverse(mAnimation.begin(), mAnimation.end());
    }
    void Animation::SetFrameAngle(float angle) {
        for(size_t i = 0; i < mAnimation.size(); i++) {
            mAnimation[i].SetAngle(angle);
        }
    }
};

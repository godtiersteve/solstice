#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include "animator.h"
#include "frame.h"
#include "graphics/quaddata.h"
#include "graphics/texarray.h"
#include "utility/sol_str.h"

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
namespace solstice {
    enum AnimationLoop {ANIM_RERVERSE = -2, ANIM_REPEAT = -1};
    class Animation : public Animator<Frame> {
    protected:
        bool mHoriFlip, mVertFlip;
        QuadData mDrawQuad;
        Color mColor;
        Vector2f mClip, mOriginalSize;
        Vector2f mPosition;
        float mTexH, mTexW;
        int mTextureID;
        bool mSheet;
        bool mHidden;
        ModelTransform mTransforms;
        float mAngle, mStartAngle;
        float mBaseAlpha;
        void UpdateDrawQuad();
    public:
        float mRotationSpeed, mXScaling, mYScaling, mAlphaStep;
        float mBaseAngle;
        bool mRotatesAboutPoint;
        Vector2f mRotateOrigin;
        Animation();
        Animation(vector<Frame> frames);
        Animation& Copy(const Animation& anim);
        void Init(Frame frame);
        operator QuadData&();

        ModelTransform& GetTransforms() {return mTransforms;}

        void SetClip(Vector2f clip);
        void SetClip(float h, float w);
        void AutoAddFramesSep(const char* prefix, unsigned short amount, unsigned short digits = 1);
        void AutoAddFramesSheet(string texref, unsigned short amount);
        void AutoAddFramesSheet(int amt, float clipH, float clipW, float row = 0, float col = 0);
        void AutoAddFramesSheet(string texref, int amt, float clipH, float clipW, float row = 0, float col = 0);
        void Clear();
        bool operator==(string ref)           {return mRef == ref;}
        bool operator!=(string ref)           {return mRef != ref;}
        bool operator!=(int id)               {return mID != id;}
        bool operator==(int id)               {return mID == id;}
        bool IDComp(const Animation& anim)    {return mID == anim.mID;}
        bool NameComp(const Animation& anim)  {return mRef == anim.mRef;}

        Frame& operator[](unsigned short i);
        Frame& GetFrame(unsigned short i);

        void SetHeight(float h)             {mClip.y = h;}
        void SetWidth(float w)              {mClip.x = w;}
        void SetHW(float h, float w)        {mClip.y = h; mClip.x = w;}
        void AddHeight(float h)             {mClip.y+= h;}
        void AddWidth(float w)              {mClip.x+= w;}
        void AddHW(float h, float w)        {mClip.x+=w; mClip.y+=h;}
        float GetHeight()                   {return mClip.x;}
        float GetWidth()                   {return mClip.y;}
        void SetTexHW(float h, float w);
        void SetLoop(short loop = -1, unsigned short frame = 0);
        void SetHoriFlip(bool flip) {mHoriFlip = flip;}
        void SetVertFlip(bool flip) {mVertFlip = flip;}
        bool GetHoriFlipped() {return mHoriFlip;}
        bool GetVertFlipped() {return mVertFlip;}
        void FlipBoth(bool flip);
        void SetColor(Color col);
        void ForceFrame(unsigned short i);
        void SetPosition(Vector2<float> pos);
        void SetDuration(float dur);
        void SetOffset(Vector2<float> offset);
        void DuplicateFrame(unsigned short frame, unsigned short amt);
        void AddMotion(unsigned short start, unsigned short end, Vector2<float> distance);
        void AddRotation(unsigned short start, unsigned short end, float rotation);
        void AddScale(unsigned short start, unsigned short end, float scale);
        void AddFade(unsigned short start, unsigned short end, float scale);
        void SetTextureRef(string ref);
        void SetTextureID(int id);
        void SetTextureIDRange(int id, int start, int end);
        void SetTextureIDRange(int id, int start);
        void SetAngle(float angle)  {mAngle = angle; mStartAngle = angle;}
        void SetFrameAngle(float angle);
        float GetAngle()            {return mAngle;}
        void SetDrawFrom(DrawPoint pt);
        void RotateAboutPoint(float angle, Vector2f pt);

        void SetDurationRan(unsigned short start, unsigned short end, unsigned short dur);
        void SetFrameDur(unsigned short frame, unsigned short dur);

        void FixTextureIDs(TextureArray& texArr);

        short GetLoop();
        bool HoriFlipped();
        bool VertFlipped();
        unsigned short GetLoopFrame();
        Frame GetCurFrame();
        unsigned short GetCurFrameNum();
        Color GetColor();
        Vector2<float> GetClip();
        float GetClipH();
        float GetClipW();
        float GetTexH();
        float GetTexW();
        double GetPauseDur();
        Vector2<float> GetPosition();
        bool Loops();
        bool Done();
        bool GetHidden();

        size_t size() const;
        void push_back(Frame f);
        void pop_back();
        void erase(int i);

        QuadData& GetDrawQuad();

        void SetHidden(bool hidden);

        void Update(float dur = 1.0f);
        void Start();
        void Play();
        void Pause(double dur = -1);
        void Stop();
        void Reset();
        void Draw();
        void Reverse();
        void ResetSize() {mClip = mOriginalSize;}
        void CopyFrames(const Animation& anim);

        void SaveToFile(const char* filename);
        void SaveToXML(const char* filename);
        void LoadFromFile(const char* filename);
        void LoadFromXML(const char* filename);
    };
};
#endif // ANIM_H_INCLUDED

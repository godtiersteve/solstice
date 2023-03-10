#ifndef FRAMEDATA_H_INCLUDED
#define FRAMEDATA_H_INCLUDED

#include "graphics/texture.h"
#include "geometry/geometry.h"
#include "utility/sol_str.h"
#include "graphics/texdata.h"
#include <fstream>

using std::ifstream;
using std::ofstream;

namespace solstice {
    class FrameData : public TextureData {
    protected:
        Vector2f mDrawOffset;
        float mDuration, mElapsed;
        bool mFinished;
        float mScale;
        float mAngle;
        Color mColor;
        int mTextureID;
        string mTexRef;
    public:
        FrameData();
        FrameData(Vector2f pos, TextureData tex, short duration = 0);
        FrameData& operator=(TextureData data);

        void SetDuration(float dur);
        void SetScale(float scale);
        void SetXOffset(float x);
        void GetYOffset(float y);
        void SetXYOffset(Vector2f off);
        void SetOffset(Vector2f off);
        void SetAngle(float angle);
        void SetTextureID(int id)       {mTextureID = id;}
        void SetTextureRef(string ref)  {mTexRef = ref;}

        Vector2f GetDrawOffset();
        float GetScale();
        float GetDuration();
        float GetElapsed();
        bool GetFinished();
        float GetAngle();
        bool Finished();
        Color& GetColor()               {return mColor;}
        int GetTextureID()              {return mTextureID;}
        string GetTextureRef()          {return mTexRef;}

        void Update(float dur = 1.0f);
        void Reset();

        TextureData GetTexData();
    };
};
#endif // FRAMEDATA_H_INCLUDED

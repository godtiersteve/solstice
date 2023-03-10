#ifndef MULTIQUADPIC_H_INCLUDED
#define MULTIQUADPIC_H_INCLUDED

#include "graphics/quaddata.h"
#include "graphics/texarray.h"
#include "globals.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace solstice {
    class MultiQuadImg {
    protected:
        vector<QuadData> mQuads;
        Vector2<float> mOrigin;
    public:
        MultiQuadImg();
        MultiQuadImg(Vector2<float> origin);
        MultiQuadImg(string filename, Vector2f position, string textureID = "");
        MultiQuadImg(TextureArray& texArr, Vector2f position, string textureID);
        MultiQuadImg(const MultiQuadImg& img);
        MultiQuadImg& operator=(const QuadData& quad);
        MultiQuadImg& operator=(const MultiQuadImg& img);
        void Init(const MultiQuadImg& img);
        void Init(const QuadData& quad);
        void Init(string filename, Vector2f position, string textureID = "");
        void Init(TextureArray& texArr, Vector2f position, string textureID);
        QuadData& operator[](string ref);
        QuadData& operator[](unsigned int i);
        vector<QuadData>& GetQuads() {return mQuads;}

        void Add(const QuadData& quad, bool relative = true);
        void Remove(unsigned int i);
        bool Empty()                                {return mQuads.empty();}
        void SetTexIDAll(int texid);
        void SetTexIDAll(string texid);
        void Clear();
        unsigned short Size();
        QuadData& GetQuad(int i);
        QuadData& GetQuad(string ref);
        Vector2<float> GetOrigin();

        void SetQuadXY(unsigned int i, Vector2<float> xy);
        void SetXY(Vector2<float> position);
        void AddXY(float x, float y);
        void AddXY(Vector2f amount);
        void SetPosition(Vector2<float> position) {SetXY(position);}
        void Move(Vector2<float> distance);
        //This method loads a single texture as a two-part image, by using the bottom bit of the texture as the right bit of the image. This can be done manually.
        void LoadSingleTexture(float imageHeight, float imageWidth, float textureHeight, float textureWidth, string texref);

        void FlipHoriAll(bool f);
        void FlipVertAll(bool f);
        void FlipBothAll(bool f);
        void HideAll(bool f);
        void SetColorAll(Color col);
        void SetAlpha(float a);
        void SetAngle(float a);
        void SetScale(float scale);
    };
    typedef MultiQuadImg MultiImage;
    typedef MultiQuadImg Picture;
};
#endif // MULTIQUADPIC_H_INCLUDED

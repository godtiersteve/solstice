#ifndef TEXDATA_H_INCLUDED
#define TEXDATA_H_INCLUDED

#include "graphicglobals.h"
#include "geometry/rectangle.h"
#include "clippingrect.h"
// TODO (Stephen#1#): Implement openGL array textures for Texture and Renderer objects, as well as for the shader

namespace solstice {
    class TextureData {
    protected:
        short mTexWidth, mTexHeight;
        float mXAdjust, mYAdjust;
        float mHeight, mWidth;
        Vector2f mTopRight, mTopLeft, mBotRight, mBotLeft;
    public:
        TextureData();
        TextureData(short hw);
        TextureData(short height, short width);
        TextureData(Vector2f tl, Vector2f tr, Vector2f br, Vector2f bl);
        TextureData(Vector2f origin, float h, float w, float texh, float texw);
        TextureData(float texh, float texw, float frameh, float framew, float row, float col);
        void operator=(const Rectanglef& d);

        void Init(Vector2f origin, float height, float width);
        void Init(float texh, float texw, float frameh, float framew, float row, float col);
        void Init(Vector2f origin, float h, float w, float texh, float texw);
        void Init(Vector2f tl, Vector2f tr, Vector2f br, Vector2f bl);
        void Init(ClippingRect clip);

        void SetTexHeight(short h);
        void SetTexWidth(short w);
        void SetTexHW(short h, short w);

        short GetTexHeight();
        short GetTexWidth();
        float GetXAdjust();
        float GetYAdjust();
        void SetHeight(float h)             {mHeight = h;}
        void SetWidth(float w)              {mWidth = w;}
        void SetHW(float h, float w)        {mHeight = h; mWidth = w;}
        float GetHeight();
        float GetWidth();
        void HoriFlip();
        void VertFlip();

        Vector2f GetTopRight() const;
        Vector2f GetTopLeft() const;
        Vector2f GetBotRight() const;
        Vector2f GetBotLeft() const;

        void SetTopRight(Vector2f vt);
        void SetTopLeft(Vector2f vt);
        void SetBotRight(Vector2f vt);
        void SetBotLeft(Vector2f vt);
        void AddCoords(std::vector<float>& coords, int id = 1000);
    };
};
#endif // TEXDATA_H_INCLUDED

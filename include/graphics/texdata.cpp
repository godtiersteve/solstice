#include "texdata.h"

namespace solstice {
TextureData::TextureData() {
    Init(0,
        Vector2f(1, 0),
        1,
        Vector2f(0, 1));
    SetTexHW(1, 1);
}
TextureData::TextureData(short hw) {
    Init(0, hw, hw);
}
TextureData::TextureData(short height, short width) {
    Init(0, height, width);
}
TextureData::TextureData(Vector2f tl, Vector2f tr, Vector2f br, Vector2f bl) {
    Init(tl, tr, br, bl);
}
TextureData::TextureData(Vector2f origin, float h, float w, float texh, float texw) {
    Init(origin, h, w, texh, texw);
}
TextureData::TextureData(float texh, float texw, float frameh, float framew, float row, float col) {
    //Init(texh, texw, framew, framew, row, col);
    Init(Vector2f(col, row),
         frameh, framew,
         texh, texw);
}
void TextureData::operator=(const Rectangle<float>& d) {
   *this = d;
}
void TextureData::Init(Vector2f origin, float height, float width) {
    if(mTexHeight < 2 && mTexWidth < 2) {
        SetTexHW(height, width);
    }
    mHeight = height * mYAdjust;
    mWidth = width * mXAdjust;
    origin.x = origin.x * mXAdjust;
    origin.y = origin.y * mYAdjust;
    float top, left, right, bottom;
    top = origin.y + mHeight;
    bottom = origin.y;
    left = origin.x;
    right = origin.x + mWidth;
    mBotLeft = origin;
    mBotRight.x = right;
    mBotRight.y = bottom;
    mTopRight.x = right;
    mTopRight.y = top;
    mTopLeft.x = left;
    mTopLeft.y = top;
}
void TextureData::Init(Vector2f tl, Vector2f tr, Vector2f br, Vector2f bl) {
    mTopLeft = tl;
    mTopRight = tr;
    mBotRight = br;
    mBotLeft = bl;
    mHeight = tl.y - bl.y;
    mWidth = tr.x - tl.x;
}
void TextureData::Init(Vector2f origin, float h, float w, float texh, float texw) {
    SetTexHW(texh, texw);
    Init(origin, h, w);
}
void TextureData::Init(ClippingRect clip) {
    SetTexHW(clip.height, clip.width);
    Init(clip.GetOrigin(), clip.clipHeight, clip.clipWidth);
}
void TextureData::Init(float texh, float texw, float frameh, float framew, float row, float col) {
    SetTexHW(texh, texw);
    float l, r, t, b;
    l = col * mXAdjust;
    r = (framew + col) *  mXAdjust;
    t = (frameh + row) * mYAdjust;
    b = row * mXAdjust;;
    Init(Vector2f(l, b),
         Vector2f(r, b),
         Vector2f(r, t),
         Vector2f(l, t));
}
void TextureData::HoriFlip() {
    std::swap(mTopRight.x, mTopLeft.x);
    std::swap(mBotLeft.x, mBotRight.x);
}
void TextureData::VertFlip() {
    //Init(mTopLeft, mTopRight, mBotRight, mBotLeft);
    std::swap(mBotRight.y, mTopRight.y);
    std::swap(mBotLeft.y, mTopLeft.y);
}
void TextureData::AddCoords(std::vector<float>& coords, int id) {
    coords.push_back(mBotLeft.x + id);
    coords.push_back(mBotLeft.y + id);
    coords.push_back(mTopLeft.x + id);
    coords.push_back(mTopLeft.y + id);
    coords.push_back(mTopRight.x + id);
    coords.push_back(mTopRight.y + id);
    coords.push_back(mBotRight.x + id);
    coords.push_back(mBotRight.y + id);
}
void TextureData::SetTexHeight(short h)             {mTexHeight = h; mYAdjust = 1.0f / (float)h;}
void TextureData::SetTexWidth(short w)              {mTexWidth = w; mXAdjust = 1.0f / (float)w;}
void TextureData::SetTexHW(short h, short w)        {mTexHeight = h; mTexWidth = w; mXAdjust = 1.0f / (float)w; mYAdjust = 1.0f / (float)h;}
Vector2f TextureData::GetTopRight() const          {return mTopRight;}
Vector2f TextureData::GetTopLeft()  const          {return mTopLeft;}
Vector2f TextureData::GetBotRight() const          {return mBotRight;}
Vector2f TextureData::GetBotLeft() const           {return mBotLeft;}

void TextureData::SetTopRight(Vector2f vt)    {mTopRight = vt;}
void TextureData::SetTopLeft(Vector2f vt)     {mTopLeft = vt;}
void TextureData::SetBotRight(Vector2f vt)    {mBotRight = vt;}
void TextureData::SetBotLeft(Vector2f vt)     {mBotLeft = vt;}
float TextureData::GetHeight()                      {return mHeight;}
float TextureData::GetWidth()                       {return mWidth;}
short TextureData::GetTexHeight()                   {return mTexHeight;}
short TextureData::GetTexWidth()                    {return mTexWidth;}
float TextureData::GetXAdjust()                     {return mXAdjust;}
float TextureData::GetYAdjust()                     {return mYAdjust;}
};

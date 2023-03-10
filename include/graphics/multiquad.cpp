#include "multiquad.h"

namespace solstice {
    MultiQuadImg::MultiQuadImg() {
        mOrigin = 0;
    }
    MultiQuadImg::MultiQuadImg(Vector2<float> origin)
    {
        mOrigin = origin;
    }
    MultiQuadImg::MultiQuadImg(string filename, Vector2f position, string textureID) {
        Init(filename, position, textureID);
    }
    MultiQuadImg::MultiQuadImg(TextureArray& texArr, Vector2f position, string textureID) {
        Init(texArr, position, textureID);
    }
    MultiQuadImg::MultiQuadImg(const MultiQuadImg& img) {
        Init(img);
    }
    MultiQuadImg& MultiQuadImg::operator=(const QuadData& quad) {
        Clear();
        mQuads.push_back(quad);
        return *this;
    }
    MultiQuadImg& MultiQuadImg::operator=(const MultiQuadImg& img) {
        Init(img);
        return *this;
    }
    void MultiQuadImg::Init(const MultiQuadImg& img) {
        mQuads.clear();
        mOrigin = img.mOrigin;
        for(size_t i = 0; i < img.mQuads.size(); i++) {
            mQuads.push_back(img.mQuads[i]);
        }
    }
    void MultiQuadImg::Init(string filename, Vector2f position, string textureID) {
        SDL_Surface* image = IMG_Load(filename.c_str());
        mQuads.clear();
        mQuads.push_back(QuadData(position, image->h, image->w, textureID));
        SDL_FreeSurface(image);
    }
    void MultiQuadImg::Init(TextureArray& texArr, Vector2f position, string textureID) {
        float height = texArr.GetTexture(textureID).GetHeight();
        float width = texArr.GetTexture(textureID).GetWidth();
        mQuads.clear();
        mQuads.push_back(QuadData(position, height, width, textureID));
    }
    void MultiQuadImg::Init(const QuadData& quad) {
        *this = quad;
    }
    void MultiQuadImg::LoadSingleTexture(float imageHeight, float imageWidth, float textureHeight, float textureWidth, string texref) {
        Clear();
        QuadData left, right;
        float leftWidth = textureWidth;
        float leftHeight = imageHeight;
        float rightHeight = imageWidth - textureWidth;
        float rightWidth = imageHeight;
        left.Init(0, leftHeight, leftWidth);
        left.SetTextureCoords(Vector2<float>(0, textureHeight - imageHeight),
                              leftHeight, leftWidth,
                              textureHeight, textureWidth);

        right.Init(0, rightHeight, rightWidth);
        right.SetDrawFrom(DRAW_CENTER);
        right.SetTextureCoords(Vector2<float>(0, textureHeight - imageHeight - rightHeight),
                               rightHeight, rightWidth,
                               textureHeight, textureWidth);
        right.SetAngle(PI / 2);
        right.AddX(left.GetWidth() + right.GetHeight() / 2);
        right.AddY(right.GetWidth() / 2);
        left.SetTextureRef(texref);
        right.SetTextureRef(texref);
        mQuads.push_back(left);
        mQuads.push_back(right);
    }
    void MultiQuadImg::SetTexIDAll(int texid) {
        if(mQuads.empty()) {
            return;
        }
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetTextureID(texid);
        }
    }
    void MultiQuadImg::SetTexIDAll(string texid) {
        if(mQuads.empty()) {
            return;
        }
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetTextureRef(texid);
        }
    }
    QuadData& MultiQuadImg::operator[](string ref) {
        return GetQuad(ref);
    }
    QuadData& MultiQuadImg::operator[](unsigned int i) {
        return GetQuad(i);
    }

    void MultiQuadImg::Add(const QuadData& quad, bool relative) {
        mQuads.push_back(quad);
        if(relative) {
            mQuads[mQuads.size() - 1].AddXY(relative);
        }
    }
    void MultiQuadImg::Remove(unsigned int i) {
        if(i < mQuads.size()) {
            mQuads.erase(mQuads.begin() + i);
        }
    }
    void MultiQuadImg::Clear() {
        mQuads.clear();
        mOrigin = 0;
    }
    unsigned short MultiQuadImg::Size() {
        return mQuads.size();
    }
    QuadData& MultiQuadImg::GetQuad(int i) {
        return mQuads[i];
    }
    QuadData& MultiQuadImg::GetQuad(string ref) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            if(mQuads[i].GetTextureRef() == ref) {
                return mQuads[i];
            }
        }
        return mQuads.front();
    }

    void MultiQuadImg::SetQuadXY(unsigned int i, Vector2<float> xy) {
        if(i < mQuads.size() && !mQuads.empty()) {
            mQuads[i].SetXY(xy);
        }
    }
    void MultiQuadImg::SetXY(Vector2<float> position) {
        mOrigin = position;
    }
    void MultiQuadImg::AddXY(float x, float y) {
        AddXY(Vector2f(x, y));
    }
    void MultiQuadImg::AddXY(Vector2f amount) {
        mOrigin+= amount;
    }
    void MultiQuadImg::Move(Vector2<float> distance) {
        mOrigin+= distance;
    }

    void MultiQuadImg::FlipHoriAll(bool f) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetHoriFlip(f);
        }
    }
    void MultiQuadImg::FlipVertAll(bool f) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetVertFlip(f);
        }
    }
    void MultiQuadImg::FlipBothAll(bool f) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetHoriFlip(f);
            mQuads[i].SetVertFlip(f);
        }
    }
    void MultiQuadImg::HideAll(bool f) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetHidden(f);
        }
    }
    void MultiQuadImg::SetColorAll(Color col) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetColor(col);
        }
    }
    void MultiQuadImg::SetAlpha(float a) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            Color c = mQuads[i].GetColor();
            c.alpha = a;
            mQuads[i].SetColor(c);
        }
    }
    void MultiQuadImg::SetAngle(float a) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetAngle(a);
        }
    }
    void MultiQuadImg::SetScale(float scale) {
        for(size_t i = 0; i < mQuads.size(); i++) {
            mQuads[i].SetScale(scale);
        }
    }
    Vector2<float> MultiQuadImg::GetOrigin() {
        return mOrigin;
    }
};

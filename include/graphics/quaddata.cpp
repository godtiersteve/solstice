#include "quaddata.h"

namespace solstice {

    QuadData::QuadData():
    mHidden(false), mHoriFlipped(false), mVertFlipped(false),
    mColor(COL_WHITE), mTextureID(-1), mShaderID(0), mTextureRef(""), mShaderRef("SHADER_DEFAULT")
    {
        SetDefaultTexCoords();
        Init(0, 1, 1);
    }
    QuadData::QuadData(Vector2f origin, float h, float w, int textureID):
    mHidden(false), mHoriFlipped(false), mVertFlipped(false),
    mColor(COL_WHITE), mTextureID(textureID), mShaderID(0), mTextureRef(""), mShaderRef("SHADER_DEFAULT")
    {
        SetDefaultTexCoords();
        Init(origin, h, w);
    }
    QuadData::QuadData(Vector2f origin, float h, float w, string textureRef):
    mHidden(false), mHoriFlipped(false), mVertFlipped(false),
    mColor(COL_WHITE), mTextureID(-1), mShaderID(0), mTextureRef(textureRef), mShaderRef("SHADER_DEFAULT")
    {
        SetDefaultTexCoords();
        Init(origin, h, w);
        mTextureRef = textureRef;
    }
    QuadData::QuadData(Rectanglef rect, int textureID) {
        *this = rect;
        mTextureID = textureID;
    }
    QuadData::QuadData(Rectanglef rect, int textureID, ClippingRect clip) {
        *this = rect;
        mTextureID = textureID;
        mColor = COL_WHITE;
        mTextureCoords.Init(clip);
    }
    QuadData& QuadData::operator=(const Rectanglef& rect) {
        mTextureID = -1;
        mTextureRef = "";
        mShaderRef = "";
        mShaderID = 0;
        mVerts[VT_TOPRIGHT] = rect.GetTopRight();
        mVerts[VT_TOPLEFT] = rect.GetTopLeft();
        mVerts[VT_BOTRIGHT] = rect.GetBotRight();
        mVerts[VT_BOTLEFT ]= rect.GetBotLeft();
        mCenter = rect.GetCenter();
        mPosition = rect.GetOrigin();
        mH = rect.GetHeight();
        mW = rect.GetWidth();
        mAngle = rect.GetAngle();
        mSize = rect.GetScale();
        mDrawFrom = rect.GetDrawFrom();
        return *this;
    }

    void QuadData::Init(Vector2f pos, float h, float w, DrawPoint drawFrom, float scale, float angle) {
        Rectangle::Init(pos, h, w, drawFrom, scale, angle);
    }
    void QuadData::Init(Vector2f topLeft, Vector2f topRight, Vector2f botRight, Vector2f botLeft) {
        Rectangle::Init(topLeft, topRight, botRight, botLeft);
    }
    void QuadData::SetHidden(bool h)                    {mHidden = h;}
    void QuadData::InitTexData(Rectangle<float> d)      {mTextureCoords = d;}
    void QuadData::SetTextureID(int i)                  {mTextureID = i;}
    void QuadData::SetTextureRef(string ref)            {mTextureRef = ref;}
    void QuadData::SetHoriFlip(bool f)                  {
        if(f) {
            mTextureCoords.HoriFlip();
        }
        mHoriFlipped = f;
    }
    void QuadData::SetVertFlip(bool f)                  {
        if(f) {
            mTextureCoords.VertFlip();
        }
        mVertFlipped = f;
    }
    void QuadData::SetFlip(bool hori, bool vert)        {SetHoriFlip(hori); SetVertFlip(vert);}
    void QuadData::SetShaderID(int id)                  {mShaderID = id;}
    void QuadData::SetShaderRef(string ref)             {mShaderRef = ref;}
    void QuadData::SetColor(Color c)                    {mColor = c;}

    bool QuadData::HoriFlipped()                    {return mHoriFlipped;}
    bool QuadData::VertFlipped()                    {return mVertFlipped;}
    bool QuadData::Hidden()                         {return mHidden;}
    int QuadData::GetTextureID()                    {return mTextureID;}
    string QuadData::GetTextureRef()                {return mTextureRef;}
    int QuadData::GetShaderID()                     {return mShaderID;}
    string QuadData::GetShaderRef()                 {return mShaderRef;}
    Color& QuadData::GetColor()                     {return mColor;}
    void QuadData::SetPosition(float x, float y) {SetXY(Vector2f(x, y));}
    void QuadData::SetPosition(Vector2f pos) {SetXY(pos);}

    void QuadData::SetTextureCoords(Vector2f tl, Vector2f tr, Vector2f br, Vector2f bl) {
        mTextureCoords.Init(tl, tr, br, bl);
    }
    void QuadData::SetTextureCoords(Rectangle<float> coords) {
        mTextureCoords.Init(coords.GetTopLeft(),
                            coords.GetTopRight(),
                            coords.GetBotRight(),
                             coords.GetBotLeft());
    }
    void QuadData::SetTextureCoords(float height, float width, float xAdjust, float yAdjust) {
        float w = width * xAdjust;
        float h = height * yAdjust;
        mTextureCoords.Init(Vector2f(0.0f, h),
                            Vector2f(w, h),
                            Vector2f(w, 0),
                            Vector2f(0));

    }
    void QuadData::SetTextureCoords(Vector2f origin, int h, int w, float texH, float texW) {
        mTextureCoords.Init(origin, h, w, texH, texW);
    }
    void QuadData::SetTextureCoords(TextureData d)  {mTextureCoords = d;}
    void QuadData::SetTextureCoordsTile(Vector2f origin, float texH, float texW) {
        SetTextureCoords(Rectanglef(origin, mH / texH, mW / texW));
    }
    TextureData& QuadData::GetTextureCoords()       {return mTextureCoords;}
    Vertex2f QuadData::GetVertex2f(int i) const {
        Vertex2f ret;
        ret.textureID = mTextureID;
        ret.color = mColor;
        switch(i) {
            case VT_BOTLEFT:
                ret = this->mVerts[VT_TOPLEFT];
                ret.y = this->mVerts[VT_BOTLEFT].y + mH * mTransform.GetScale().y;
                ret.texCoord = mTextureCoords.GetBotLeft();
                break;
            case VT_TOPLEFT:
                ret = this->mVerts[VT_BOTLEFT];
                ret.texCoord = mTextureCoords.GetTopLeft();
                break;
            case VT_TOPRIGHT:
                ret = this->mVerts[VT_BOTRIGHT];
                ret.x = this->mVerts[VT_BOTLEFT].x + mW * mTransform.GetScale().x;
                ret.texCoord = mTextureCoords.GetTopRight();
                break;
            case VT_BOTRIGHT:
            default:
                //ret = this->mVerts[VT_TOPRIGHT];
                ret.x = this->mVerts[VT_BOTLEFT].x + mW * mTransform.GetScale().x;
                ret.y = this->mVerts[VT_BOTLEFT].y + mH * mTransform.GetScale().y;
                ret.texCoord = mTextureCoords.GetBotRight();
                break;
        }
        //ret.x*= mTransform.GetScale().x;
        //ret.y*= mTransform.GetScale().y;
        return ret;
    }
    Vector2f QuadData::GetTexCoord(size_t i) const {
        switch(i) {
            case VT_BOTLEFT:
                return mTextureCoords.GetBotLeft();
                break;
            case VT_TOPLEFT:
                return mTextureCoords.GetTopLeft();
                break;
            case VT_TOPRIGHT:
                return mTextureCoords.GetTopRight();
                break;
            case VT_BOTRIGHT:
            default:
                return mTextureCoords.GetBotRight();
                break;
        }

    }
    void QuadData::AddVertices(std::vector<Vertex2f>& verts, int start, Vector2f norm, Color c) {
        if(!c) {
            c = mColor;
        }
        if(mTextureID < 0) {
            c.alpha = -c.alpha;
        }
        int j = 0;
        for(int i = start; i < start + 4; i++) {
            verts[i].x = this->mVerts[j].x;
            verts[i].y = this->mVerts[j].y;
            verts[i].color = c;
            verts[i].texCoord = GetTexCoord(j);
            j++;
        }
    }
    std::vector<Vertex2f> QuadData::GetVerts(Color c) {
        if(!c) {
            c = mColor;
        }
        vector<Vertex2f> verts(4);
        int texID = -1000;
        if(mTextureID < 1000) {
            texID = mTextureID * 1000;
        }
        else if(mTextureID >= 0) {
            texID = mTextureID;
        }
        for(size_t i = 0; i < 4; i++) {
            verts[i].x = this->mVerts[i].x;
            verts[i].y = this->mVerts[i].y;
            verts[i].color = c;
            verts[i].texCoord = GetTexCoord(i);
            verts[i].texCoord+= texID;
        }
        return verts;
    }
    std::vector<Vertex2f> QuadData::GetVerts(Vector2f scale, Color c) {
        if(!c) {
            c = mColor;
        }
        vector<Vertex2f> verts(4);
        int texID = -1000;
        if(mTextureID < 1000) {
            texID = mTextureID * 1000;
        }
        else if(mTextureID >= 0) {
            texID = mTextureID;
        }
        Rectanglef norm = Normalize(0, scale.x, scale.y);
        for(size_t i = 0; i < 4; i++) {
            verts[i].x = this->mVerts[i].x * scale.x;
            verts[i].y = this->mVerts[i].y * scale.y;
            //verts[i].x = norm.GetVertices()[i].x;
            //verts[i].y = norm.GetVertices()[i].y;
            verts[i].color = c;
            verts[i].texCoord = GetTexCoord(i);
            verts[i].texCoord+= texID;
        }

        return verts;
    }
    void QuadData::TextureCoordsToArray(GLfloat* arr, Vector2f offset) {
        arr[0] = mTextureCoords.GetTopLeft().x + offset.x;
        arr[1] = mTextureCoords.GetTopLeft().y + offset.y;
        arr[2] = mTextureCoords.GetTopRight().x + offset.x;
        arr[3] = mTextureCoords.GetTopRight().y + offset.y;
        arr[4] = mTextureCoords.GetBotRight().x + offset.x;
        arr[5] = mTextureCoords.GetBotRight().y + offset.y;
        arr[6] = mTextureCoords.GetBotLeft().x + offset.x;
        arr[7] = mTextureCoords.GetBotRight().y + offset.y;
    }
    std::vector<float> QuadData::GetVertexArr(float xtrans, float ytrans, float xnorm, float ynorm) {
        std::vector<float> ret;
        ret.push_back((xtrans + GetLeft()) * xnorm);
        ret.push_back((ytrans + GetBottom()) * ynorm);

        ret.push_back((xtrans + GetRight()) * xnorm);
        ret.push_back((ytrans + GetBottom()) * ynorm);

        ret.push_back((xtrans + GetRight()) * xnorm);
        ret.push_back((ytrans + GetTop()) * ynorm);

        ret.push_back((xtrans + GetLeft()) * xnorm);
        ret.push_back((ytrans + GetTop()) * ynorm);

        return ret;
    }
    void QuadData::SetDefaultTexCoords() {
        mTextureCoords.Init(Vector2f(0, 1),
                            Vector2f(1),
                            Vector2f(1, 0),
                            Vector2f(0));
    }
};

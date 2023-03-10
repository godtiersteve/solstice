#ifndef QUAD_DATA_H
#define QUAD_DATA_H

#include "graphicglobals.h"
#include "geometry/rectangle.h"
#include "graphics/texdata.h"
#include "graphics/vertex2.h"
#include "graphics/clippingrect.h"
#include <string>

using std::string;

namespace solstice {
    class QuadData : public Rectanglef
    {
    protected:
        bool mHidden, mHoriFlipped, mVertFlipped;
        string mTextureRef, mShaderRef;
        TextureData mTextureCoords;
    public:
        ModelTransform mTransform;
        Color mColor;
        short mTextureID, mShaderID;
        QuadData();
        QuadData(Vector2f origin, float h, float w, int textureID = -1);
        QuadData(Vector2f origin, float h, float w, string textureRef);
        QuadData(Rectanglef rect, int textureID);
        QuadData(Rectanglef rect, int textureID, ClippingRect clip);

        QuadData& operator=(const Rectanglef& rect);

        void SetHidden(bool h);
        void InitTexData(Rectangle<float> d);
        void SetTextureID(int i);
        void SetTextureRef(string ref);
        void SetHoriFlip(bool f);
        void SetVertFlip(bool f);
        void SetFlip(bool hori, bool vert);
        void SetShaderID(int id);
        void SetShaderRef(string ref);
        void SetColor(Color c);
        void Init(Vector2f pos, float h, float w, DrawPoint drawFrom = DRAW_BOTLEFT, float scale = 1.0f, float angle = 0.0f);
        void Init(Vector2f topLeft, Vector2f topRight, Vector2f botRight, Vector2f botLeft);

        bool HoriFlipped();
        bool VertFlipped();
        bool Hidden();
        int GetTextureID();
        string GetTextureRef();
        int GetShaderID();
        string GetShaderRef();
        Color& GetColor();
        const GLuint& GetVAO();
        const GLuint& GetVBO();
        std::vector<float> GetVertexArr(float xtrans = 0, float ytrans = 0, float xnorm = 1, float ynorm = 1);
        std::vector<Vertex2f> GetVerts(Vector2f scale = 1.0f, Color c = COL_EMPTY);
        std::vector<Vertex2f> GetVerts(Color c = COL_EMPTY);
        void AddVertices(std::vector<Vertex2f>& verts, int start, Vector2f norm, Color c);

        void SetPosition(float x, float y);
        void SetPosition(Vector2f pos);

        void SetTextureCoords(Vector2f tl, Vector2f tr, Vector2f br, Vector2f bl);
        void SetTextureCoords(Rectanglef coords);
        void SetTextureCoords(float height, float width, float xAdjust = 1, float yAdjust =  1);
        void SetTextureCoords(Vector2f origin, int h, int w, float texH, float texW);
        void SetTextureCoords(TextureData d);
        void SetTextureCoords(ClippingRect r) {mTextureCoords.Init(r);}
        void SetTextureCoordsTile(Vector2f origin, float texH, float texW);
        TextureData& GetTextureCoords();
        Vector2f GetTexCoord(size_t i) const;
        Vertex2f GetVertex2f(int i) const;
        void TextureCoordsToArray(GLfloat* arr, Vector2f offset = 0);

        void SetDefaultTexCoords();
        bool Empty() {return mColor == COL_EMPTY;}
    };
};

#endif // DRAWQUAD_H

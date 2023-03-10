#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "graphicglobals.h"
#include "geometry/rectangle.h"
#include "utility/sol_math.h"

#include <string>
#include <iostream>
using std::string;
namespace solstice {
    enum TexLoadType {TEX_IMAGE, TEX_ARCHIVE, TEX_FOLDER, TEX_CYTEX, TEX_PIXELDATA};

    class Texture {
    protected:
        GLuint mTexture;
        SDL_Surface* mPixelData;
        float mYAdjust, mXAdjust;
        GLenum mType;
        short mHeight, mWidth, mDepth, mLastUsedArrayID;
        string mNameRef;
        std::vector<string> mLayerRefs;
        void GetSurface(const char* file);
    public:
        Texture();
        Texture(const char* filename, string nameRef = "");
        Texture(GLubyte* pdata, int h, int w, string nameRef = "");
        void Clean();
        void ClearSurface();
        void FreePixelData();
        void LoadImage(const char* filename, GLenum tileType = GL_REPEAT, GLenum minFilter = GL_LINEAR, GLenum maxFilter = GL_NEAREST);
        void LoadPixelData(GLubyte* pdata, int h, int w);
        void LoadPixelDataArrays(GLubyte* pdata, int h, int w, int count);
        void CreateArrayTexture(int width, int height, int layers, int wrapType = GL_CLAMP_TO_EDGE);
        void LoadToArray(const char* filename, const char* ref = "");

        void Bind();
        void Bind(GLenum textureUnit);
        void Unbind();
        void Rebind(const char* filename);
        void Delete();
        void SetHeight(float h) {mHeight = h;}
        void SetWidth(float w) {mWidth = w;}
        void SetHW(float h, float w) {mHeight = h; mWidth = w;}

        void SetNameRef(string ref);
        int GetArrayID(string ref);
        short GetHeight();
        short GetWidth();
        float GetXAdjust();
        float GetYAdjust();
        string GetNameRef();
        GLenum GetType()            {return mType;}

        Vector2f ConvertPoint(Vector2f pt);
        Rectangle<float> ConvertRect(Rectangle<float> rect);

        bool Empty();
        GLuint& GetTexture();
    };
};

#endif // TEXTURE_H_INCLUDED

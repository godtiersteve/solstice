#ifndef SOLFONT_H_INCLUDED
#define SOLFONT_H_INCLUDED

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "globals.h"
#include "graphics/texarray.h"
#include "graphics/graphicglobals.h"
#include "graphics/shader.h"
#include "graphics/shaderprogram.h"
#include "text/sol_char.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftstroke.h>

using std::vector;
using std::string;

namespace solstice {
    class FontData
    {
    protected:
        ShaderProgram mShader;
        glm::mat4 mProjection;
        int mPointSize, mSpace, mLineBreakHeight;
        string mFilename;
        //Renderer mRenderer;
        TextureArray mTexArray;
        vector<SolChar> mCharSet;
        float mScale;
        bool mEmpty, mStroked;
        short mStrokeSize;
        FT_Library mLibrary;
        GLuint mVao, mVbo, mEbo, mPositionBuffer[2];
        std::vector<Vertex2f> mQuad;
        size_t mCacheSize;
        std::string mStoredText;
        std::vector<glm::vec2> mPositions;
        std::vector<glm::vec2> mTexturePositions;
        size_t mLastIndex;
        void CheckBufferSize(size_t size);
    public:
        Color mColor;
        Texture texture;
        FontData();
        FontData(const char* fname, unsigned int h, bool stroked = false, int strokeSize = 2);
        FontData& operator=(const FontData& data);
        void Init();
        void Init(const char* fname, unsigned int h, bool stroked = false, int strokeSize = 2);
        void ClearBuffer() {mLastIndex = 0;}

        void Clean();

        void Print(int x, int y, const char* str, ...);
        void Print(Color color, int x, int y, const char* str, ...);
        void PrintBreak(Color color, int x, int y, short lineBreak, const char* str, ...);
        void PrintBreak(int x, int y, short lineBreak, const char* str, ...);
        void Print(string text, float x, float y, Color color = COL_WHITE);
        void PrintBreak(string text, int x, int y, short lineBreak, Color color = COL_WHITE);
        void CreateTextureAtlas();

        void Draw();

        void SetStroke(int size);

        void SetSpaceWidth(float width);
        glm::mat4& GetProjection() {return mProjection;}
        ShaderProgram& GetShader() {return mShader;}
        size_t GetCacheSize()       {return mCacheSize;}
        size_t GetLineHeight()      {return mLineBreakHeight;}
        Vector2i GetStringDimensions(short lineBreak, const char* str, ...);

        float GetScale();
        int GetPointSize();
        int GetSpaceWidth();
        bool Empty();
        bool Stroked();
        int GetStrokeSize();
        //Renderer& GetRenderer() {return mRenderer;}

        short GetNumberOfLines (short lineBreak, string text);
        short GetStringWidth(short lineBreak, const char* str, ...);
    };
};
#endif // GLFONT_H_INCLUDED

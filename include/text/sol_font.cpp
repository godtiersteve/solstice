#include "sol_font.h"
namespace solstice {
    FontData::FontData():
    mScale(1.0f), mEmpty(true), mStroked(false)
    {
        mLastIndex = 0;
    }

    FontData::FontData(const char* fname, unsigned int h, bool stroked, int strokeSize):
    mScale(1.0f), mStroked(stroked)
    {
        mEmpty = true;
        Init(fname, h, stroked, strokeSize);
        mLastIndex = 0;
    }
    void FontData::Clean() {
        mCharSet.clear();
        mLastIndex = 0;
        mEmpty = true;
        glDeleteBuffers(1, &mVbo);
        glDeleteBuffers(2, mPositionBuffer);
        glDeleteVertexArrays(1, &mVao);
        mQuad.clear();
    }
    FontData& FontData::operator=(const FontData& data) {
        Init(data.mFilename.c_str(), data.mPointSize, data.mStroked, data.mStrokeSize);
        mLastIndex = 0;
        return *this;
    }
    void FontData::Init() {
        Init(mFilename.c_str(), mPointSize, mStroked, mStrokeSize);
    }
    void FontData::Init(const char* fname, unsigned int pointSize, bool stroked, int strokeSize) {
        mPointSize = pointSize;
        mStroked = stroked;
        mStrokeSize = stroked ? strokeSize : 0;
        mFilename = fname;
        mCharSet.resize(128);
        mEmpty = false;
        CreateTextureAtlas();
    }
    void FontData::CreateTextureAtlas() {
        if(FT_Init_FreeType(&mLibrary))
            throw std::runtime_error("FT_Iiit_FreeType failed");

        FT_Face face;
        if(FT_New_Face(mLibrary, mFilename.c_str(), 0, &face))
            std::cout << "FT_New_Face failed loading the font file.\n";

        FT_Set_Char_Size(face, mPointSize * 64, mPointSize * 64, 72, 72);
        FT_GlyphSlot g = face->glyph;
        int w = 0;
        int h = 0;
        for(int i = 32; i < 128; i++) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }
            w = std::max(w, g->bitmap.width);
            h = std::max(h, g->bitmap.rows);
        }
        GLuint& tex = texture.GetTexture();
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        std::vector<GLubyte> arr(w * 96 * h, 0);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     w * 96,
                     h,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     &arr[0]);

        int x = 0;
        for(int i = 32; i < 128; i++) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                continue;
            }
            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            x,
                            0,
                            g->bitmap.width,
                            g->bitmap.rows,
                            GL_RED,
                            GL_UNSIGNED_BYTE,
                            g->bitmap.buffer);
            x+= w;

        }
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        texture.SetHeight(h);
        texture.SetWidth(w * 96);

        float xx = 0;
        int maxHeight = 0, maxWidth = 0;
        for(int i = 0; i < mCharSet.size(); i++) {
            FT_Load_Char(face, i, FT_LOAD_RENDER);
            FT_Glyph_Metrics& faceMetrics = g->metrics;
            float width = g->bitmap.width;
            SolChar c = mCharSet[i];
            mCharSet[i].GetDrawingQuad().SetTextureCoords(Vector2f(xx, 0), h, w, h, w * 96);
            mCharSet[i].GetDrawingQuad().SetHW(h, w);
            mCharSet[i].pitch = g->bitmap.pitch;
            float yOff = (h - g->bitmap.rows) + (faceMetrics.height - faceMetrics.horiBearingY) / 64;
            mCharSet[i].yOff = yOff;
            mCharSet[i].xOff = w - width;
            mCharSet[i].width = w;
            mCharSet[i].height = g->bitmap.rows;
            mCharSet[i].advance = (faceMetrics.horiAdvance / 64) - (faceMetrics.horiBearingX / 64);
            if(maxHeight < mCharSet[i].height) {
                maxHeight = mCharSet[i].height;
            }
            if(maxWidth < mCharSet[i].width) {
                maxWidth = mCharSet[i].width;
            }
            if(i >= 32) {
                xx+= w;
            }
        }

        mSpace = maxWidth / 128;
        mLineBreakHeight = maxHeight;

        FT_Done_Face(face);
        FT_Done_FreeType(mLibrary);

        mQuad.clear();
        float texWidth = 1.0f / 96.0f;
        float quadWidth = 1.0f;
        mQuad.push_back(solstice::Vertex2f(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        mQuad.push_back(solstice::Vertex2f(glm::vec2(0.0f, h), glm::vec2(0.0f, 1.0f)));
        mQuad.push_back(solstice::Vertex2f(glm::vec2(quadWidth , h), glm::vec2(texWidth, 1.0f)));
        mQuad.push_back(solstice::Vertex2f(glm::vec2(quadWidth , 0.0f), glm::vec2(texWidth, 0.0f)));
        glGenBuffers(1, &mVbo);
        glGenBuffers(2, mPositionBuffer);
        glGenBuffers(1, &mEbo);
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER,
                     mQuad.size() * sizeof(Vertex2f),
                     &mQuad[0],
                     GL_STATIC_DRAW);
        mCacheSize = 500;
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER,
                     mCacheSize * sizeof(glm::vec2),
                     nullptr,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer[2]);
        glBufferData(GL_ARRAY_BUFFER,
                     mCacheSize * sizeof(glm::vec2),
                     nullptr,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(SOLSTICE_RENDER_TRIANGLES),
                     SOLSTICE_RENDER_TRIANGLES,
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(solstice::Vertex2f), NULL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(solstice::Vertex2f), (const GLvoid*)offsetof(solstice::Vertex2f, texCoord));
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer[0]);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribDivisor(2, 1);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer[1]);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribDivisor(3, 1);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void FontData::SetSpaceWidth(float width) {mSpace = fabs(width);}

    float FontData::GetScale()          {return mScale;}
    int FontData::GetPointSize()        {return mPointSize;}
    int FontData::GetSpaceWidth()       {return mSpace;}
    bool  FontData::Empty()             {return mEmpty;}
    bool FontData::Stroked()            {return mStroked;}
    int FontData::GetStrokeSize()       {return mStrokeSize;}
    void FontData::SetStroke(int size) {
        if(size <= 0) {
            if(mStroked) {
                mStroked = false;
                mStrokeSize = 0;
                Init();
            }
            mStroked = false;
            return;
        }
        else {
            mStroked = true;
            mStrokeSize = size;
            Init();
        }
    }
    void  FontData::Print(int x, int y, const char* str, ...) {
        char text[strlen(str) * 2];
        va_list ap;
        if(str == NULL)
            *text = NULL;
        else {
            va_start(ap, str);
            vsprintf(text, str, ap);
            va_end(ap);
        }

       Print(text, x, y);
    }

    void FontData::Print(Color color, int x, int y, const char* str, ...) {
        char text[strlen(str) * 2];
        va_list ap;
        if(str == NULL)
            *text = NULL;
        else {
            va_start(ap, str);
            vsprintf(text, str, ap);
            va_end(ap);
        }
        Print(text, x, y, color);
    }

    void FontData::PrintBreak(Color color, int x, int y, short lineBreak, const char* str, ...) {
        char text[strlen(str) * 2];
        va_list ap;

        if(str == NULL)
            *text = NULL;
        else {
            va_start(ap, str);
            vsprintf(text, str, ap);
            va_end(ap);
        }
        PrintBreak(text, x, y, lineBreak, color);
    }
    void FontData::PrintBreak(int x, int y, short lineBreak, const char* str, ...) {
        char text[strlen(str) * 2];
        va_list ap;

        if(str == NULL)
            *text = NULL;
        else {
            va_start(ap, str);
            vsprintf(text, str, ap);
            va_end(ap);
        }
        PrintBreak(text, x, y, lineBreak, COL_WHITE);
    }
    void FontData::PrintBreak(string text, int x, int y, short lineBreak, Color color) {
        CheckBufferSize(text.size());
        int curLine = y;
        int charY = curLine;
        int charX = x;
        short lineWidth = 0;
        for(int i = 0; i < text.size(); i++) {
            SolChar& currChar = mCharSet[text[i]];
            charY = (curLine - currChar.yOff) * mScale;
            lineWidth+= currChar.width;
            if(text[i] != ' ' && text[i] != '\n') {
                mPositions[mLastIndex].x = charX;
                mPositions[mLastIndex].y = charY;
                mTexturePositions[mLastIndex].x = mCharSet[text[i]].GetDrawingQuad().GetTextureCoords().GetTopLeft().x;
                mTexturePositions[mLastIndex].y = mCharSet[text[i]].width;
                mLastIndex++;
            }
            if(text[i] == ' ') {
                int spaceLeft = lineBreak - lineWidth;
                int wordSize = 0;
                int j = i + 1;

                while(j < text.size()) {
                    if(text[j] != ' ' && text[j] != '\n') {
                        wordSize+= currChar.width;
                    }
                    else
                        break;
                    j++;
                }
                if(wordSize > spaceLeft) {
                    curLine-= mLineBreakHeight;
                    charX = x;
                    lineWidth = 0;
                    wordSize = 0;
                }
                else {
                    charX+= currChar.advance * mScale;
                    lineWidth+= currChar.advance;
                }
            }
            else if(text[i] == '\n') {
                curLine-= mLineBreakHeight;
                charX = x;
                lineWidth = 0;
            }
            else {
                charX+= currChar.advance * mScale;
            }
        }
        mColor = color;
    }
    void FontData::Print(string text, float x, float y, Color color) {
        CheckBufferSize(text.size());
        int curLine = y;
        int charY = curLine;
        int charX = x;
        for(unsigned int i = 0; i < text.size(); i++) {
            char& ch = text[i];
            SolChar& currChar = mCharSet[ch];
            charY = curLine - currChar.yOff;
            if(text[i] != ' ' && text[i] != '\n') {
                mPositions[mLastIndex].x = charX;
                mPositions[mLastIndex].y = charY;
                mTexturePositions[mLastIndex].x = mCharSet[text[i]].GetDrawingQuad().GetTextureCoords().GetTopLeft().x;
                mTexturePositions[mLastIndex].y = mCharSet[text[i]].width;
                mLastIndex++;
            }
            charX+= currChar.advance;
            if(text[i] == ' ')
                charX+= mSpace * mScale;
            else if(text[i] == '\n') {
                curLine-= mLineBreakHeight;
                charX = x;
            }
        }
        mColor = color;
    }
    void FontData::CheckBufferSize(size_t size) {
        if(mLastIndex + size >= mPositions.size()) {
            size = mPositions.size() ? mPositions.size() * 2 : size * 2;
            mPositions.resize(size);
            mTexturePositions.resize(size);
        }
    }
    void FontData::Draw() {
        texture.Bind();
        glBindVertexArray(mVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER,
                     mPositions.size() * sizeof(glm::vec2),
                     &mPositions[0],
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer[1]);
        glBufferData(GL_ARRAY_BUFFER,
                     mTexturePositions.size() * sizeof(glm::vec2),
                     &mTexturePositions[0],
                     GL_DYNAMIC_DRAW);
        mShader.UseProgram();
        mShader.SetUniform4f("fragColor", mColor.red, mColor.green, mColor.blue, mColor.alpha);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, mLastIndex);
        glBindVertexArray(0);
    }
    short FontData::GetNumberOfLines(short lineBreak, string text) {
        if(mCharSet.empty()) {
           return 0;
        }
        short lines = 1;
        short lineWidth = 0;
        for(unsigned int i = 0; i < text.size(); i++) {
            SolChar& currChar = mCharSet[text[i]];
            if(text[i] == ' ') {
                int spaceLeft = lineBreak - lineWidth;
                int wordSize = 0;
                int j = i + 1;

                while(j < text.size() && text[j] != '\0') {
                    if(text[j] != ' ' && text[j] != '\n') {
                        wordSize+= currChar.advance;
                    }
                    else {
                        break;
                    }
                    j++;
                }
                if(wordSize >= spaceLeft) {
                    lines++;
                    lineWidth = 0;
                }
            }
            else if(text[i] == '\n') {
                lineWidth = 0;
                lines++;
            }
            lineWidth+= currChar.advance;
        }
        return lines;
    }
    Vector2i FontData::GetStringDimensions(short lineBreak, const char* str, ...) {
        char text[strlen(str) * 2];
        va_list ap;
        if(str == NULL)
            *text = NULL;
        else {
            va_start(ap, str);
            vsprintf(text, str, ap);
            va_end(ap);
        }

        short size = strlen(text);
        short maxWidth = 0;
        short height = mLineBreakHeight;
        int lineWidth = 0;
        for(unsigned int i = 0; i < size; i++) {
            SolChar& currChar = mCharSet[text[i]];
            if(text[i] == ' ') {
                int spaceLeft = lineBreak - lineWidth;
                int wordSize = 0;
                int j = i + 1;

                while(j < size && text[j] != '\0') {
                    if(text[j] != ' ' && text[j] != '\n') {
                        wordSize+= currChar.advance;
                    }
                    else {
                        break;
                    }
                    j++;
                }
                if(wordSize >= spaceLeft) {
                    if(lineWidth > maxWidth) {
                        maxWidth = lineWidth;
                    }
                    lineWidth = 0;
                    height+= mLineBreakHeight;
                }
            }
            else if(text[i] == '\n') {
                if(lineWidth > maxWidth) {
                    maxWidth = lineWidth;
                }
                lineWidth = 0;
                height+= mLineBreakHeight;
            }
            lineWidth+= currChar.advance;
        }
        int width =lineWidth > maxWidth ? lineWidth : maxWidth;
        return Vector2i(width, height);
    }
    short FontData::GetStringWidth(short lineBreak, const char* str, ...) {
        char text[strlen(str) * 2];
        va_list ap;
        if(str == NULL)
            *text = NULL;
        else {
            va_start(ap, str);
            vsprintf(text, str, ap);
            va_end(ap);
        }
        return GetStringDimensions(lineBreak, text).x;
    }
};

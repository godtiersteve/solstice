#ifndef SOLTEXT_H_INCLUDED
#define SOLTEXT_H_INCLUDED

#include "sol_font.h"
#include "gl/glew.h"

using std::string;

namespace solstice {
    enum TextAlignment {ALIGN_LEFT = 0, ALIGN_RIGHT, ALIGN_CENTER, ALIGN_JUSTIFY};
    class SolText : public FontData
    {
    protected:
        string mText;
        vector<Letter> mLetters;
        Vector2<float> mOrigin;
        Color mBaseColor;
        short mLineBreak;

        unsigned short mLettersToStep, mLettersStepped, mFrameSkip, mFrameTimer;
        unsigned short mCharsPerFrame;
        int mCurXPos;
        bool mPaused, mPeriodPause;
        bool mNewlinePause;
        short mElipsisDel, mCommaDel, mColonDel, mSemiColDel;
        short mElipsisType;
        short mDelayTimer;

        bool mHasShadow, mWarps, mFades, mScales;
        float mFrequency, mAmplitude, mPhase, mAngleStep;
        float mFadeMin, mFadeMax, mShadowAlpha, mFadeStep;
        float mScaleStart, mScaleEnd, mScaleStep;

        TextAlignment mAlignment;

        //VertexBuffer mVBO;
        GLuint vbo, vao, ebo;
        GLuint* indices;
        GLuint INDICES;

        void Reset();
        void CheckPunctuation(unsigned int i);
        void AlignSubString(unsigned int start, unsigned int end, int stepped, int spaceLeft = 0);
        void UpdateCharPositions(Vector2f pos);
    public:
        SolText();
        void Clean();
        SolText(const char* font, unsigned int pointSize, bool stroked = false, int strokeSize = 2);
        SolText(const char* font, unsigned int pointSize, string text, Vector2<float> origin = 0, bool stroked = false, int strokeSize = 2);
        SolText& operator=(const SolText& text);
        Letter& operator[](unsigned int i);
        void Init(const char* filename, int h, string text, bool stroked = false, int strokeSize = 2);
        void GenCharPositions();

        void SetString(string text);
        void SetX(int x);
        void SetY(int y);
        void SetXY(int x, int y);
        void SetPosition(Vector2<float> pos);
        void SetColor(Color col);
        void SetLineBreakH(short lb);
        void SetLineBreak(short max);
        void SetFrameSkip(short skip);
        void SetFrequency(float freq);
        void SetAmplitude(float amp);
        void SetPhase(float phase);
        void SetAngleStep(float step);
        void SetFadeMin(float fadeMin);
        void SetFadeMax(float fadeMax);
        void SetFadeStep(float step);
        void SetShadowAlpha(float a);
        void SetScaleStart(float start);
        void SetScaleEnd(float end);
        void SetScaleStep(float step);
        void SetTextAlignment(TextAlignment alignment);
        void SetCharsPerFrame(int c);
        void SetNewlinePause(bool nl);

        void Fade(float start, float end, float step);
        void Fade();
        void Scale(float start, float end, float step);
        void Warp(float freq, float amp, float phase, float step);
        void SetDelays(short elipsis, short comma, short colon, short semicolon);
        void FadeRange(float start, float end, float step, unsigned int first, unsigned int last);
        void ScaleRange(float start, float end, float step, unsigned int first, unsigned int last);
        void WarpRange(float freq, float amp, float phase, float step, unsigned int first, unsigned int last);
        void ColorRange(Color col, unsigned int first, unsigned int last);

        string GetText();
        string GetString() {return mText;}
        std::vector<Letter>& GetLetters() {return mLetters;}
        Vector2<float> GetOrigin();
        Color GetColor();
        short GetLineBreakH();
        short GetLineBreak();
        short GetFrameSkip();
        short GetFrameTimer();
        short GetCharsPerFrame();
        short GetLetterStepped();
        float GetFrequency();
        float GetAmplitude();
        float GetPhase();
        float GetAngle();
        float GetAngleStep();
        float GetFadeMin();
        float GetFadeMax();
        float GetFadeStep();
        float GetScaleStart();
        float GetScaleEnd();
        float GetScaleStep();
        float GetShadowAlpha();
        bool GetNewlinePause();
        int GetLettersStepped() {return mLettersStepped;}
        TextAlignment GetTextAlignment();

        bool Paused();
        bool Delayed();
        bool Done();
        short GetElipsisDelay();
        short GetCommaDelay();
        short GetColonDelay();
        short GetSemiColDelay();
        bool PeriodPause();
        bool GetShadow();
        bool Fades();
        bool Scales();
        bool Warps();
        void SetElipsisDelay(short timer);
        void SetCommaDelay(short timer);
        void SetColonDelay(short timer);
        void SetSemiColDelay(short timer);
        void PeriodPause(bool flag);
        void SetShadow(bool flag);
        void Fades(bool flag);
        void Scales(bool flag);
        void Warps(bool flag);
        unsigned int CheckStringWidth(const string& s);
        unsigned int CheckWordLength(const string& s);
        unsigned int GetLineLength(const string& s);
        void ShiftCharacters(unsigned int start, unsigned int end, int amount);

        void Update();
        void Pause();
        void Stop();
        void Unpause();
        void Next();
        void FinishLine();
        void FinishText();

        void SetupVBO();
        void Draw2();
        void Draw2(ShaderProgram& shader);
    };
};
#endif // SolText_H_INCLUDED

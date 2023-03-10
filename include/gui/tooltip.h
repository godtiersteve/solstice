#ifndef TOOLTIP_H_INCLUDED
#define TOOLTIP_H_INCLUDED

#include "geometry/rectangle.h"
#include "graphics/quaddata.h"
#include "text/sol_font.h"
#include "guicomponent.h"
#include <string>

using std::string;

namespace solstice {
    class ToolTip : public GUIComponent {
    protected:
        string mText;
        float mMaxWidth;
        float mScale, mAlpha;
        float mScaleStep, mFadeStep;
        float mFadeMax, mScaleMin;
        bool mScales, mFades;
        Color mTextColor;
        bool mTextShadow;
        short mPaddingLeft, mPaddingRight, mPaddingTop, mPaddingBottom;
        FontData* mFontData;

        void AdjustHeight();
        void AdjustSize();
        Vector2i mTextDimensions;
    public:
        ToolTip();
        ToolTip(string text, FontData* font);
        ToolTip(string text, QuadData quad, FontData* font);
        ToolTip(const ToolTip& tip);

        ToolTip& operator=(const ToolTip& tip);

        void Init(string text, FontData* font);

        void SetMaxWidth(float w);
        void SetText(string text);
        void SetScaleStep(float step);
        void SetFadeStep(float step);
        void SetFadeMax(float max);
        void SetScaleMin(float min);
        void Scale(float step, float min);
        void Fade(float step, float max);
        void SetScales(bool f);
        void SetFade(bool f);
        void SetScale(float scale);
        void SetAlpha(float a);
        void SetPadding(short padding);
        void SetPadding(short rl, short tb);
        void SetPadding(short l, short r, short t, short b);
        void SetLeftPadding(short l);
        void SetRightPadding(short r);
        void SetTopPadding(short t);
        void SetBottomPadding(short b);
        void SetTextColor(Color c);
        void SetFontData(FontData* fontData);

        float GetMaxWidth();
        bool Scales();
        bool Fades();
        bool Empty();
        string GetText();
        float GetScaleStep();
        float GetFadeStep();
        float GetFadeMax();
        float GetScaleMin();
        float GetScale();
        float GetAlpha();
        short GetPaddingRight();
        short GetPaddingLeft();
        short GetPaddingTop();
        short GetPaddingBottom();
        Color GetTextColor();
        Vector2i GetTextDimensions() {return mTextDimensions;}
        int GetLineBreakWidth() {return mMaxWidth - mPaddingRight - mPaddingLeft;}

        void Update();
        void SetHidden(bool h);

        QuadData GetDrawQuad();
        virtual void Update(SDL_Event& event, Vector2f pt) {Update();}
        virtual void Update(Vector2f pt) {Update();}
        virtual void Draw(SceneRenderer& rend);
    };
}

#endif // TOOLTIP_H_INCLUDED

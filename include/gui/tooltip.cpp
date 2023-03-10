#include "tooltip.h"

namespace solstice {
    ToolTip::ToolTip():
    mText(""), mScale(1.0f),
    mScaleStep(0.025f), mFadeStep(0.025f), mAlpha(1.0f),
    mFadeMax(1.0f), mScaleMin(0.05f),
    mFades(true), mScales(true), mMaxWidth(256),
    mPaddingTop(0), mPaddingBottom(0), mPaddingLeft(0), mPaddingRight(0)
    {
        SetHidden(true);
        mFontData = NULL;
    }

    ToolTip::ToolTip(string text, FontData* font):
     mText(text), mScale(1.0f),
    mScaleStep(0.025f), mFadeStep(0.025f), mAlpha(1.0f),
    mFadeMax(1.0f), mScaleMin(0.05f),
    mFades(true), mScales(true), mMaxWidth(256),
    mPaddingTop(0), mPaddingBottom(0), mPaddingLeft(0), mPaddingRight(0),
    mFontData(font)
    {
        SetHidden(true);
        SetText(text);
    }

    ToolTip::ToolTip(string text, QuadData quad, FontData* font):
    mText(text), mScale(1.0f),
    mScaleStep(0.025f), mFadeStep(0.025f), mAlpha(1.0f),
    mFadeMax(1.0f), mScaleMin(0.05f),
    mFades(true), mScales(true), mMaxWidth(256),
    mPaddingTop(0), mPaddingBottom(0), mPaddingLeft(0), mPaddingRight(0),
    mFontData(font)
    {
        SetHidden(true);
        SetText(text);
    }
    ToolTip::ToolTip(const ToolTip& tip) {
        *this = tip;
    }
    void ToolTip::Init(string text, FontData* font) {
        mFontData = font;
        SetText(text);
    }

    ToolTip& ToolTip::operator=(const ToolTip& ttip) {
        mColor = ttip.mColor;
        mTextureID = ttip.mTextureID;
        mTextureCoords = ttip.mTextureCoords;
        SetXYHW(ttip.GetOrigin(), ttip.GetHeight(), ttip.GetWidth());
        SetDrawFrom(ttip.GetDrawFrom());
        SetScale(ttip.mScale);
        SetAngle(ttip.mAngle);
        mText = ttip.mText;
        mMaxWidth = ttip.mMaxWidth;
        mScale = ttip.mScale;
        mAlpha = ttip.mAlpha;
        mScaleStep = ttip.mScaleStep;
        mFadeStep = ttip.mFadeStep;
        mFadeMax = ttip.mFadeMax;
        mScaleMin = ttip.mScaleMin;
        mScales = ttip.mScales;
        mFades = ttip.mFades;
        mTextColor = ttip.mTextColor;
        mTextShadow = ttip.mTextShadow;
        mFontData = ttip.mFontData;
        mPaddingBottom = ttip.mPaddingBottom;
        mPaddingLeft = ttip.mPaddingLeft;
        mPaddingRight = ttip.mPaddingRight;
        mPaddingTop = ttip.mPaddingTop;
        mTextDimensions = ttip.mTextDimensions;
        return *this;
    }
    void ToolTip::AdjustHeight() {
        SetHeight(mTextDimensions.y + mPaddingTop + mPaddingBottom);
    }
    void ToolTip::AdjustSize() {
        if(mFontData) {
            mTextDimensions = mFontData->GetStringDimensions(GetLineBreakWidth(), mText.c_str());
        }
        SetHeight(mTextDimensions.y + mPaddingTop + mPaddingBottom);
        SetWidth(mTextDimensions.x + mPaddingRight + mPaddingLeft);
    }
    void ToolTip::SetText(string text)          {
        mText = text;
        AdjustSize();
    }
    void ToolTip::SetScaleStep(float step)      {mScaleStep = step;}
    void ToolTip::SetFadeStep(float step)       {mFadeStep = step;}
    void ToolTip::SetFadeMax(float max)         {mFadeMax = max;}
    void ToolTip::SetScaleMin(float min)        {mScaleMin = min;}
    void ToolTip::Scale(float step, float min)  {mScaleStep = step; mScaleMin = min; mScales = true;}
    void ToolTip::Fade(float step, float max)   {mFadeStep = step; mFadeMax = max; mFades = true; mAlpha = 0.0f;}
    void ToolTip::SetScales(bool f)             {mScales = f; mScale = mScaleMin;}
    void ToolTip::SetFade(bool f)               {mFades = f; if(f) mAlpha = 0.0f;}
    void ToolTip::SetScale(float scale)         {mScale = scale; if(scale <= 0) mScale = 1.0f;}
    void ToolTip::SetAlpha(float a)             {mAlpha = a;}
    void ToolTip::SetMaxWidth(float w)          {mMaxWidth = w; AdjustHeight();}

    void ToolTip::SetPadding(short padding) {
        mPaddingRight = padding;
        mPaddingLeft = padding;
        mPaddingTop = padding;
        mPaddingBottom = padding;
        AdjustSize();
    }
    void ToolTip::SetPadding(short rl, short tb) {
        mPaddingRight = rl;
        mPaddingLeft = rl;
        mPaddingTop = tb;
        mPaddingBottom = tb;
        AdjustSize();
    }
    void ToolTip::SetPadding(short l, short r, short t, short b) {
        mPaddingLeft = l;
        mPaddingRight = r;
        mPaddingTop = t;
        mPaddingBottom = b;
        AdjustSize();
    }
    void ToolTip::SetLeftPadding(short l)           {mPaddingLeft = l;}
    void ToolTip::SetRightPadding(short r)          {mPaddingRight = r;}
    void ToolTip::SetTopPadding(short t)            {mPaddingTop = t;}
    void ToolTip::SetBottomPadding(short b)         {mPaddingBottom = b;}
    void ToolTip::SetTextColor(Color c)             {mTextColor = c;}
    void ToolTip::SetFontData(FontData* fontData)   {mFontData = fontData; AdjustHeight();}

    bool        ToolTip::Scales()           {return mScales;}
    bool        ToolTip::Fades()            {return mFades;}
    string      ToolTip::GetText()          {return mText;}
    float       ToolTip::GetScaleStep()     {return mScaleStep;}
    float       ToolTip::GetFadeStep()      {return mFadeStep;}
    float       ToolTip::GetFadeMax()       {return mFadeMax;}
    float       ToolTip::GetScaleMin()      {return mScaleMin;}
    float       ToolTip::GetScale()         {return mScale;}
    short       ToolTip::GetPaddingRight()  {return mPaddingRight;}
    short       ToolTip::GetPaddingLeft()   {return mPaddingLeft;}
    short       ToolTip::GetPaddingTop()    {return mPaddingTop;}
    short       ToolTip::GetPaddingBottom() {return mPaddingBottom;}
    Color       ToolTip::GetTextColor()     {return mTextColor;}
    float       ToolTip::GetAlpha()         {return mAlpha;}
    float       ToolTip::GetMaxWidth()      {return mMaxWidth;}
    bool        ToolTip::Empty()            {return (mFontData == NULL || mText.empty());}
    QuadData ToolTip::GetDrawQuad() {
        QuadData drawQuad(GetOrigin(), GetHeight(), GetWidth(), GetTextureRef());
        drawQuad.GetTextureCoords() = mTextureCoords;
        drawQuad.SetColor(mColor);
        drawQuad.SetDrawFrom(mDrawFrom);
        return drawQuad;
    }
    void ToolTip::Update() {
        if(!Hidden()) {
            if(mScales && mScale < 1.0f)
                mScale+= fabs(mScaleStep);

            if(mFades && mAlpha < mFadeMax)
                mAlpha+= fabs(mFadeStep);

            if(mAlpha > mFadeMax)
                mAlpha = mFadeMax;

            SetScale(mScale);
            mColor.alpha = mAlpha;
            mTextColor.alpha = mAlpha;
        }
    }
    void ToolTip::Draw(SceneRenderer& rend) {
        if(!mFontData || mText.empty() || mHidden) {
            return;
        }
        float x, y, lb = mMaxWidth;
        mTransform.SetScale(mScale, mScale, 1.0);
        rend.DrawQuadPoly(*this);
        if(mScale >= 1.0) {
            x = GetLeft() + mPaddingRight;
            y = GetTop() - mPaddingTop - mFontData->GetLineHeight();
            float pad = GetWidth() - (mFontData->GetStringWidth(lb, mText.c_str()));
            mFontData->PrintBreak(mTextColor,
                                x, y, lb,
                                mText.c_str());
        }
    }
    void ToolTip::SetHidden(bool h) {
        mHidden = h;
        if(mScales)
            mScale = mScaleMin;
        else
            mScale = 1.0f;
        if(mFades)
            mAlpha = 0.0f;
        else
            mAlpha = mFadeMax;
    }
}

#include "slider.h"
namespace solstice {
ButtonSlider::ButtonSlider():
mMax(100), mMin(0), mCurrentValue(50), mAbsolute(100),
mBarWidth(50), mPosition(0), mRatio(1.0f), mSliding(false),
mFont(NULL), mHighlightColor(COL_BLUE)
{
    Init(0, 100, 50, 100, 0);
}
ButtonSlider::ButtonSlider(short min, short max, short init, int barWidth, Vector2<float> center):
mMax(max), mMin(min), mCurrentValue(init), mAbsolute(abs(max - min)),
mPosition(center), mSliding(false),
mFont(NULL), mHighlightColor(COL_BLUE)
{
    Init(min, max, init, barWidth, center);
}
void ButtonSlider::Init(short min, short max, short init, int barWidth, Vector2f center) {
    mPosition = center;
    mCurrentValue = init;
    mMin = min;
    mMax = max;
    mAbsolute = abs(max - min);
    mRatio =  float(barWidth) / float(max - min);
    mBarButton.SetXY(center);
    mBarButton.SetHW(5.0f, barWidth);
    mBarButton.SetDrawFrom(DRAW_CENTER);

    mMinPos.x = mBarButton.GetLeft();
    mMinPos.y = center.y;

    mMaxPos.x = mBarButton.GetRight();
    mMaxPos.y = center.y;

    mMaxButton.SetXY(mMaxPos);
    mMaxButton.SetDrawFrom(DRAW_LEFTCENTER);

    mMinButton.SetXY(mMinPos);
    mMinButton.SetDrawFrom(DRAW_RIGHTCENTER);

    mSliderButton.SetXY(Vector2<float>(mBarButton.GetLeft() + init * mRatio, center.y));
    mSliderButton.SetHW(30.0f, 7.0f);
    mSliderButton.SetDrawFrom(DRAW_CENTER);
    mBarWidth = barWidth;
}

void ButtonSlider::SetButtons(Button slider, Button min, Button max, Button bar){
    mSliderButton = slider;
    mMinButton = min;
    mMaxButton = max;
    mBarButton = bar;
}
void ButtonSlider::SetButtons(Button slider, Button min, Button max) {
    mSliderButton = slider;
    mMinButton = min;
    mMaxButton = max;
}

void ButtonSlider::SetSliderButton(Button slider) {mSliderButton = slider;}
void ButtonSlider::SetMinButton(Button min)       {mMinButton = min;}
void ButtonSlider::SetMaxButton(Button max)       {mMaxButton = max;}
void ButtonSlider::SetBarButton(Button bar)       {mBarButton = bar;}

void ButtonSlider::SetX(float x)                {mPosition.x = x;}
void ButtonSlider::SetY(float y)                {mPosition.y  = y;}
void ButtonSlider::SetXY(Vector2<float> pt)           {mPosition = pt;}
void ButtonSlider::SetMin(short min)            {mMin = min; mAbsolute = fabs(mMax - min);}
void ButtonSlider::SetMax(short max)            {mMax = max; mAbsolute = fabs(max - mMin);}
void ButtonSlider::SetCurrent(short current)    {mCurrentValue = current;}
void ButtonSlider::SetBarWidth(short width)     {mBarWidth = width;}
void ButtonSlider::SetFontData(FontData* font){mFont = font;}
void ButtonSlider::SetValue(int v) {
    if(v < mMin) {
        v = mMin;
    }
    if(v > mMax) {
        v = mMax;
    }
    mCurrentValue = v;
}

Button& ButtonSlider::GetSliderButton()   {return mSliderButton;}
Button& ButtonSlider::GetMinButton()      {return mMinButton;}
Button& ButtonSlider::SetMaxButton()      {return mMaxButton;}
Button& ButtonSlider::SetBarButton()      {return mBarButton;}
short   ButtonSlider::GetMax()          {return mMax;}
short   ButtonSlider::GetMin()          {return mMin;}
short   ButtonSlider::GetCurrent()      {return mCurrentValue;}
short   ButtonSlider::GetAbsolute()     {return mAbsolute;}
short   ButtonSlider::GetBarWidth()     {return mBarWidth;}
void ButtonSlider::AddX(float x) {
    mSliderButton.AddX(x);
    mMinButton.AddX(x);
    mMaxButton.AddX(x);
    mBarButton.AddX(x);
    QuadData::AddX(x);
}
void ButtonSlider::AddY(float y) {
    mSliderButton.AddY(y);
    mMinButton.AddY(y);
    mMaxButton.AddY(y);
    mBarButton.AddY(y);
    QuadData::AddY(y);
}
void ButtonSlider::AddXY(float x, float y) {
    mSliderButton.AddXY(x, y);
    mMinButton.AddXY(x, y);
    mMaxButton.AddXY(x, y);
    mBarButton.AddXY(x, y);
    QuadData::AddXY(x, y);
}
void ButtonSlider::SetColor(Color c, Color highlight) {
    mSliderButton.mColor = c;
    mMinButton.mColor = c;
    mMaxButton.mColor = c;
    mBarButton.mColor = c;
    mColor = c;
    if(highlight) {
        mHighlightColor = highlight;
    }
}
void ButtonSlider::Update(SDL_Event& event, Vector2f pt) {
    if(PointRectCollision(pt, mSliderButton)) {
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            mSliding = true;
            mSliderButton.SetColor(mHighlightColor);
        }
    }
    else if(!mSliding) {
        mSliderButton.SetColor(COL_WHITE);
    }

    if(PointRectCollision(pt, mMinButton) &&
       event.type == SDL_MOUSEBUTTONUP &&
       event.button.button == SDL_BUTTON_LEFT)
    {
        mCurrentValue = mMin;
    }
    if(PointRectCollision(pt, mMaxButton) &&
       event.type == SDL_MOUSEBUTTONUP &&
       event.button.button == SDL_BUTTON_LEFT)
    {
        mCurrentValue = mMax;
    }

    if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        mSliding = false;

    if(mSliding) {
        if(pt.x < mBarButton.GetLeft())
            mCurrentValue = mMin;
        else if (pt.x > mBarButton.GetRight())
            mCurrentValue = mMax;
        else
            mCurrentValue = (pt.x - mBarButton.GetLeft()) / mRatio;
    }
    mSliderButton.SetX(mBarButton.GetLeft() + mCurrentValue * mRatio);
}
void ButtonSlider::Update(Vector2f pt) {
}

/*
void ButtonSlider::Draw(Renderer& r) {
    mMinButton.SetColor(COL_RED);
    r.DrawQuad(mMinButton);

    mMaxButton.SetColor(COL_GREEN);
    r.DrawQuad(mMaxButton);

    mBarButton.SetColor(COL_BLUE);
    r.DrawQuad(mBarButton);

    r.DrawQuad(mSliderButton);

    if(mSliding && mFont != NULL) {
        short x, y;
        x = mSliderButton.GetCenter().x - mFont->GetStringWidth(100, "%i", mCurrentValue) * 0.5;
        y = mFont->GetPointSize() + mSliderButton.GetTop() + mSliderButton.GetHeight() * 0.25;

        mFont->Print(COL_BLACK, x + 1, y - 1, "%i", mCurrentValue);
        mFont->Print(COL_WHITE, x, y, "%i", mCurrentValue);
    }
}
*/
void ButtonSlider::Draw(SceneRenderer& rend) {
    rend.DrawQuadPoly(mMinButton);
    rend.DrawQuadPoly(mMaxButton);
    rend.DrawQuadPoly(mBarButton);
    rend.DrawQuadPoly(mSliderButton);

    if(mSliding && mFont) {
        short x, y;
        x = mSliderButton.GetCenter().x - mFont->GetStringWidth(100, "%i", mCurrentValue) * 0.5;
        y = mSliderButton.GetTop() + mFont->GetLineHeight();

        mFont->Print(COL_BLACK, x + 1, y - 1, "%i", mCurrentValue);
        mFont->Print(COL_WHITE, x, y, "%i", mCurrentValue);
    }
}
};

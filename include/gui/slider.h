#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED
// TODO (Stephen#1#): Test slider.h

#include "text/sol_font.h"
#include "button.h"
#include "graphics/scenerenderer.h"
#include "guicomponent.h"

namespace solstice {
class ButtonSlider : public GUIComponent {
protected:
    short mMax, mMin, mCurrentValue, mAbsolute;
    short mBarWidth;
    float mRatio;
    Vector2<float> mPosition, mMaxPos, mMinPos;
    bool mIsVertical, mSliding;
    FontData* mFont;
public:
    Color mHighlightColor;
    Button mSliderButton, mMinButton, mMaxButton, mBarButton;
    ButtonSlider();
    ButtonSlider(short min, short max, short init, int barWidth, Vector2<float> center);

    void Init(short min, short max, short init, int barWidth, Vector2f center);

    void SetButtons(Button slider, Button min, Button max, Button bar);
    void SetButtons(Button slider, Button min, Button max);
    void SetSliderButton(Button slider);
    void SetMinButton(Button min);
    void SetMaxButton(Button max);
    void SetBarButton(Button bar);

    void SetX(float x);
    void SetY(float y);
    void AddX(float x);
    void AddY(float y);
    void AddXY(float x, float y);
    void SetXY(Vector2<float> pt);
    void SetMin(short min);
    void SetMax(short max);
    void SetCurrent(short current);
    void SetBarWidth(short width);
    void SetVertical(bool vert);
    void SetFontData(FontData* font);
    void SetValue(int v);

    Button& GetSliderButton();
    Button& GetMinButton();
    Button& SetMaxButton();
    Button& SetBarButton();
    short GetMax();
    short GetMin();
    short GetCurrent();
    short GetAbsolute();
    short GetBarWidth();
    int GetValue() {return mCurrentValue;}
    bool Vertical();
    void SetColor(Color c, Color highlight = COL_EMPTY);
    void SetFont(FontData* font)       {mFont = font;}

    void Update(SDL_Event& event, Vector2f pt);
    void Update(Vector2f pt);
    void Draw(SceneRenderer& rend);
};
}
#endif // SLIDER_H_INCLUDED

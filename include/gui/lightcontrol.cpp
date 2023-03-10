#include "lightcontrol.h"

namespace solstice {
void ColorControl::Update(SDL_Event& event, Vector2f pt) {
    mRed.Update(event, pt);
    mGreen.Update(event, pt);
    mBlue.Update(event, pt);
}

void ColorControl::Update(Vector2f pt) {
    mRed.Update(pt);
    mGreen.Update(pt);
    mBlue.Update(pt);
}

void ColorControl::Draw(SceneRenderer& rend) {
    mRed.Draw(rend);
    mGreen.Draw(rend);
    mBlue.Draw(rend);
}
void ColorControl::InitSliders(Color c, ButtonSlider slider, int yGap) {
    InitSliders(slider, yGap);
    SetColor(c);
}
void ColorControl::InitSliders(ButtonSlider slider, int yGap) {
    mRed = slider;
    mGreen = slider;
    mBlue = slider;
    mGreen.AddY(yGap);
    mBlue.AddY(yGap * 2);
    mBlue.SetColor(COL_BLUE, COL_WHITE);
    mGreen.SetColor(COL_GREEN, COL_WHITE);
    mRed.SetColor(COL_RED, COL_WHITE);
}
void ColorControl::SetFonts(FontData* font) {
    mRed.SetFont(font);
    mGreen.SetFont(font);
    mBlue.SetFont(font);
}
void ColorControl::Update() {
    float mod = 1.0 / 255.0;
    mColor.red = mRed.GetValue() * mod;
    mColor.green = mGreen.GetValue() * mod;
    mColor.blue = mBlue.GetValue() * mod;
}
void ColorControl::OnNotify(const GUIEntity& entity, SolsticeGUIEvent event) {

}
void ColorControl::SetColor(Color c) {
    float mod = 1.0 / 255.0;
    mRed.SetValue(c.red / mod);
    mGreen.SetValue(c.green / mod);
    mBlue.SetValue(c.blue / mod);
    mColor = c;
}
LightControl::LightControl(Vector2f pos) {
    Init(ButtonSlider(0, 255, 255, 100, 0), pos);
}
LightControl::LightControl(ButtonSlider base, Vector2f pos) {
    Init(base, pos);
}
void LightControl::Init(ButtonSlider base, Vector2f pos) {
    SetPosition(pos);
    float width = base.GetBarWidth() * 1.2;
    mDistance.Init(0, 10, 5, 100, base.GetPosition());
    //mDistance.AddX(-width * 0.5 - 10);
    mDistance.AddY((-base.mSliderButton.GetHeight() - 15) * 3);
    mAmbient.InitSliders(base, -base.mSliderButton.GetHeight() - 15);
    mAmbient.SetColor(Color(0.25, 0.25, 0.25));
    base.AddX(width);
    mDiffuse.InitSliders(base, -base.mSliderButton.GetHeight() - 15);
    mDiffuse.SetColor(Color(0.5, 0.5, 0.5));
    base.AddX(width);
    mSpecular.InitSliders(base, -base.mSliderButton.GetHeight() - 15);
    mSpecular.SetColor(COL_BLACK);
}
void LightControl::Update(SDL_Event& event, Vector2f pt) {
    mDistance.Update(event, pt);
    mDiffuse.Update(event, pt);
    mAmbient.Update(event, pt);
    mSpecular.Update(event, pt);
}

void LightControl::Update(Vector2f pt) {
    mDistance.Update(pt);
    mDiffuse.Update(pt);
    mAmbient.Update(pt);
    mSpecular.Update(pt);
}
void LightControl::Update() {
    mAmbient.Update();
    mSpecular.Update();
    mDiffuse.Update();
    if(mLight) {
        mLight->mAmbient = mAmbient.GetColor();
        mLight->mDiffuse = mDiffuse.GetColor();
        mLight->mSpecular = mSpecular.GetColor();
        mLight->PresetAttenuation(mDistance.GetValue());
    }
}
void LightControl::Draw(SceneRenderer& rend) {
    mDistance.Draw(rend);
    mDiffuse.Draw(rend);
    mAmbient.Draw(rend);
    mSpecular.Draw(rend);
}
void LightControl::OnNotify(const GUIEntity& entity, SolsticeGUIEvent event) {
}
void LightControl::SetFonts(FontData* font) {
    mDiffuse.SetFonts(font);
    mAmbient.SetFonts(font);
    mSpecular.SetFonts(font);
    mDistance.SetFont(font);
}

}

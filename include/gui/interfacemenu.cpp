#include "interfacemenu.h"

namespace solstice {
InterfaceMenu::InterfaceMenu(Vector2f position) {
    mVisible = true;
    mPosition = position;
}
InterfaceMenu::InterfaceMenu(const InterfaceMenu& menu)  {
    *this = menu;
}
InterfaceMenu& InterfaceMenu::operator=(const InterfaceMenu& menu) {
    mVisible = menu.mVisible;
    mPosition = menu.mPosition;
    return *this;
}
void InterfaceMenu::AddButton(Button b) {
    mButtons.push_back(b);
}
void InterfaceMenu::SetPosition(Vector2f pos) {
    mPosition = pos;
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].AddXY(mPosition);
    }
    for(size_t i = 0; i < mSliders.size(); i++) {
        mButtons[i].AddXY(mPosition);
    }
    for(size_t i = 0; i < mGraphics.size(); i++) {
        mGraphics[i].AddXY(mPosition);
    }
    mBackground.AddXY(mPosition);
}
void InterfaceMenu::AddButtons(TextureArray& arr, string texref, int cellHeight, int cellWidth, int amount) {
    int texid = arr.GetTextureID(texref);
    Texture& t = arr.GetTexture(texid);
    for(int i = 0; i < amount; i++) {
        Vector2f origin(i * cellWidth, 0);
        Button b(origin, cellHeight, cellWidth, texid);
        QuadData inactive(0, cellHeight, cellWidth, texref);
        inactive.SetTextureCoords(origin, cellHeight, cellWidth, t.GetHeight(), t.GetWidth());
        b.SetAllButtons(inactive, inactive, inactive, inactive, inactive);
        mButtons.push_back(b);
    }
}
void InterfaceMenu::SetAllButtonsInactiveColor(Color c) {
    for(size_t i = 0; i < mButtons.size(); i++) {
            mButtons[i].GetInactive().SetColor(c);
            mButtons[i].ChangeState(mButtons[i].GetState());
        }
}
void InterfaceMenu::SetAllButtonsHoverColor(Color c) {
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].GetHover().SetColor(c);
        mButtons[i].ChangeState(mButtons[i].GetState());
    }
}
void InterfaceMenu::SetAllButtonsPressColor(Color c) {
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].GetPress().SetColor(c);
        mButtons[i].ChangeState(mButtons[i].GetState());
    }
}
void InterfaceMenu::SetAllButtonsReleaseColor(Color c) {
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].GetRelease().SetColor(c);
        mButtons[i].ChangeState(mButtons[i].GetState());
    }
}
void InterfaceMenu::SetAllButtonsPressOffset(Vector2f offset) {
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].SetPressOffset(offset);
        mButtons[i].ChangeState(mButtons[i].GetState());
    }
}
void InterfaceMenu::SetButtonFont(FontData* font) {
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].GetToolTip().SetFontData(font);
    }
}
void InterfaceMenu::AddSlider(ButtonSlider slider) {
    mSliders.push_back(slider);
}
void InterfaceMenu::AddGraphic(Picture graphic) {
    mGraphics.push_back(graphic);
}
void InterfaceMenu::RemoveButton(size_t i) {
    mButtons.erase(mButtons.begin() + i);
}
void InterfaceMenu::RemoveSlider(size_t i)  {
    mSliders.erase(mSliders.begin() + i);
}
void InterfaceMenu::RemoveGraphic(size_t i) {
    mGraphics.erase(mGraphics.begin() + i);
}

Button& InterfaceMenu::GetButton(size_t i)  {
    return mButtons[i];
}
ButtonSlider& InterfaceMenu::GetSlider(size_t i) {
    return mSliders[i];
}
Picture& InterfaceMenu::GetGraphics(size_t i) {
    return mGraphics[i];
}
void InterfaceMenu::Update(SDL_Event& event, Vector2f pt) {
    for(size_t i = 0; i < mButtons.size(); i++) {
        mButtons[i].Update(event, pt);
    }
    for(size_t i = 0; i < mSliders.size(); i++) {
        mSliders[i].Update(event, pt);
    }
}
/*
void InterfaceMenu::Draw(Renderer& r) {
    r.DrawMultiQuadImg(mBackground);
    for(size_t i = 0; i < mGraphics.size(); i++) {
        r.DrawMultiQuadImg(mGraphics[i]);
    }
    for(size_t i = 0; i < mButtons.size(); i++) {
        r.DrawQuad(mButtons[i].GetDrawQuad());
        mButtons[i].GetToolTip().Draw(r);
    }
    for(size_t i = 0; i < mSliders.size(); i++) {
        mSliders[i].Draw(r);
    }
}
*/
}

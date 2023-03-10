#include "toolbar.h"

namespace solstice {
void ToolBar::Update() {
    for(size_t i = 0; i < mComponents.size(); i++) {
        mComponents[i]->Update();
    }
}
void ToolBar::Update(SDL_Event& event, Vector2f pt) {
    for(size_t i = 0; i < mComponents.size(); i++) {
        mComponents[i]->Update(event, pt);
    }
}

void ToolBar::Update(Vector2f pt) {
    for(size_t i = 0; i < mComponents.size(); i++) {
        mComponents[i]->Update(pt);
    }
}
void ToolBar::AddButton(Button b) {
    //b.AddXY(GetOrigin());
    mComponents.push_back(std::make_shared<Button>(b));
}
void ToolBar::Draw(SceneRenderer& rend) {
    rend.DrawQuadPoly(*this);
    for(size_t i = 0; i < mComponents.size(); i++) {
        mComponents[i]->Draw(rend);
    }
}
}

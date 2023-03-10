#ifndef LIGHTCONTROL_H_INCLUDED
#define LIGHTCONTROL_H_INCLUDED

#include "slider.h"
#include "graphics/3d/light.h"
namespace solstice {
    class ColorControl : public GUIComponent {
    private:
        Color mColor;
    public:
        ButtonSlider mRed, mGreen, mBlue;
        void Update();
        void Update(SDL_Event& event, Vector2f pt);
        void Update(Vector2f pt);
        Color GetColor() {return mColor;}
        void SetColor(Color c);
        void Draw(SceneRenderer& rend);
        void InitSliders(ButtonSlider slider, int yGap);
        void InitSliders(Color c, ButtonSlider slider, int yGap);
        void SetFonts(FontData* font);
        void OnNotify(const GUIEntity& entity, SolsticeGUIEvent event);
    };
    class LightControl : public GUIComponent {
    private:
        ButtonSlider mDistance;
    public:
        Light* mLight;
        ColorControl mDiffuse, mAmbient, mSpecular;

        LightControl(Vector2f pos = 0);
        LightControl(ButtonSlider base, Vector2f pos);
        void Init(ButtonSlider base, Vector2f pos);
        void Update();
        void Update(SDL_Event& event, Vector2f pt);
        void Update(Vector2f pt);
        void Draw(SceneRenderer& rend);

        void SetFonts(FontData* font);
        void OnNotify(const GUIEntity& entity, SolsticeGUIEvent event);
    };
}

#endif // LIGHTCONTROL_H_INCLUDED

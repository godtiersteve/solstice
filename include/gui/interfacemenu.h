#ifndef INTERFACE_MENU_H_INCLUDED
#define INTERFACE_MENU_H_INCLUDED

#include <vector>
#include "button.h"
#include "slider.h"
#include "tooltip.h"
#include "graphics/multiquad.h"

namespace solstice {
    class InterfaceMenu {
    private:
        std::vector<Button> mButtons;
        std::vector<ButtonSlider> mSliders;
        std::vector<Picture> mGraphics;
        Vector2f mPosition;
        bool mVisible;
        Picture mBackground;
    public:
        InterfaceMenu(Vector2f position = 0);
        InterfaceMenu(const InterfaceMenu& menu);
        InterfaceMenu& operator=(const InterfaceMenu& menu);

        Vector2f GetPosition() const                {return mPosition;}
        Picture& GetBackground()                    {return mBackground;}
        bool GetVisible() const                     {return mVisible;}

        void SetVisible(bool visible)       {mVisible = visible;}
        void SetPosition(Vector2f pos);
        void SetBackground(Picture bg)      {mBackground = bg;}

        void AddButton(Button b);
        void AddButtons(TextureArray& arr, string texref, int cellHeight, int cellWidth, int amount);
        void AddSlider(ButtonSlider slider);
        void AddGraphic(Picture graphic);
        void RemoveButton(size_t i);
        void RemoveSlider(size_t i);
        void RemoveGraphic(size_t i);

        void SetAllButtonsHoverColor(Color c);
        void SetAllButtonsInactiveColor(Color c);
        void SetAllButtonsPressColor(Color c);
        void SetAllButtonsReleaseColor(Color c);
        void SetAllButtonsPressOffset(Vector2f offset);
        void SetButtonFont(FontData* font);

        Button& GetButton(size_t i);
        ButtonSlider& GetSlider(size_t i);
        Picture& GetGraphics(size_t i);

        void Update(SDL_Event& event, Vector2<float> pt);
    };
}


#endif // MENU_H_INCLUDED

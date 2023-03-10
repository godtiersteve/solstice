#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED

#include "guicomponent.h"
#include "button.h"
#include <vector>
namespace solstice {
    class ToolBar : public GUIComponent {
    public:
        std::vector<std::shared_ptr<GUIComponent> > mComponents;
        Vector2f mPosition;
        void Update();
        void Update(SDL_Event& event, Vector2f pt);
        void Update(Vector2f pt);
        void AddButton(Button b);
        virtual void Draw(SceneRenderer& rend);
    };

}

#endif // TOOLBAR_H_INCLUDED

#ifndef GUICOMPONENT_H_INCLUDED
#define GUICOMPONENT_H_INCLUDED

#include "graphics/quaddata.h"
#include "graphics/scenerenderer.h"
#include "events/guievent.h"

namespace solstice {
    enum GUIEntityTypes {GUI_ENTITY_BUTTON, GUI_ENTITY_TOOLTIP, GUI_ENTITY_CUSOR, GUI_ENTITY_GAUGE, GUI_ENTITY_TEXTBOX, GUI_ENTITY_SLIDER};
    class GUIEntity {
    protected:
        int mEntityType;
    public:
        bool IsType(int t) const {return mEntityType == t;}
    };
    class GUIComponent : public QuadData, public GUIEntity {
    public:
        GUIComponent() {mPosition = 0;}
        virtual void Update() {}
        virtual void Update(SDL_Event& event, Vector2f pt) = 0;
        virtual void Update(Vector2f pt) = 0;
        virtual void Draw(SceneRenderer& rend) {rend.DrawQuadPoly(*this);}
    };


}

#endif // GUICOMPONENT_H_INCLUDED

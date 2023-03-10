#ifndef INFOPANEL_H_INCLUDED
#define INFOPANEL_H_INCLUDED

#include "controls.h"
#include "geometry/rectangle.h"
#include "graphics/scenerenderer.h"
#include "text/sol_text.h"

namespace solstice {
    class InfoPanel : public Rectanglef {
    public:
        std::vector<Label> mLabels;
        Color mColor;
        void Draw(FontData& font, SceneRenderer& rend);
    };
}

#endif // INFOPANEL_H_INCLUDED

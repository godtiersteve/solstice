#ifndef GUICONTROL_H_INCLUDED
#define GUICONTROL_H_INCLUDED

#include "graphics/graphics.h"

namespace solstice {

class GUIControl {
protected:
    Vector2f mPosition;
    std::vector<QuadData> mQuads;
public:
    virtual void Draw(SceneRenderer& rend) = 0;
    virtual void SetPosition(Vector2f pos)  {mPosition = pos; for(auto& q : mQuads) {q.SetPosition(pos);}}
    virtual void SetDrawPoints(DrawPoint pt) {for(auto& q : mQuads) {q.SetDrawFrom(pt);}}

    Vector2f GetPosition()          {return mPosition;}
};

}

#endif // GUICONTROL_H_INCLUDED

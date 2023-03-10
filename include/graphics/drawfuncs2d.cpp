#include "drawfuncs2d.h"
namespace solstice {
    void DrawTeleporter(SceneRenderer& rend, const Teleporter& t) {
        rend.DrawRectPoly(t.mHitBox, COL_BLUE);
        rend.DrawRectPoly(t.mEndPoint, COL_RED);
    }
}

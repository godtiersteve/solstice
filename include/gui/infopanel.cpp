#include "infopanel.h"

namespace solstice {

void InfoPanel::Draw(FontData& font, SceneRenderer& rend) {
    rend.DrawRectPoly(*this, mColor);
    std::string str;
    for(size_t i = 0; i < mLabels.size(); i++) {
        //str+= mLabels[i].
    }
}

}

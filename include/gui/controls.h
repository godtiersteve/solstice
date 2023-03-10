#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include <string>
#include <vector>
#include "graphics/vector2.h"
#include "graphics/color.h"

namespace solstice {
    class Label {
    public:
        std::string mText;
        Vector2f mPosition;
        Color mColor;
    };

}

#endif // CONTROLS_H_INCLUDED

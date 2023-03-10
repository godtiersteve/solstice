#ifndef ACTIONINPUT_H_INCLUDED
#define ACTIONINPUT_H_INCLUDED

#include "actionkey.h"

namespace solstice {
    struct ActionInputMotion {
    };
    class ActionInput {
        std::vector<ActionKey> mInputKeys;
        ActionInputMotion mMotion;
        bool CheckInput();
        void UpdateKeys(SDL_Event& event);
    };
}

#endif // ACTIONINPUT_H_INCLUDED

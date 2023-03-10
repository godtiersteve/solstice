#ifndef INPUTKEYSET_H_INCLUDED
#define INPUTKEYSET_H_INCLUDED

#include "player/actionkey.h"
#include <vector>

namespace solstice {
    enum InputKey {
        INPUT_KEY_UP, INPUT_KEY_DOWN, INPUT_KEY_LEFT, INPUT_KEY_RIGHT,
        INPUT_KEY_A, INPUT_KEY_B, INPUT_KEY_X, INPUT_KEY_Y,
        INPUT_KEY_L1, INPUT_KEY_L2, INPUT_KEY_R1, INPUT_KEY_R2,
        INPUT_KEY_START, INPUT_KEY_SELECT,
        INPUT_KEY_RIGHTSTICK, INPUT_KEY_LEFTSTICK, INPUT_KEY_RIGHTSTICK_IN, INPUT_KEY_LEFTSTICK_IN,
        INPUT_KEY_MAX
    };
    class InputKeyset {
    public:
        std::vector<ActionKey> mInputKeys;
        InputKeyset(int numKeys = INPUT_KEY_MAX);
        InputKeyset(std::vector<ActionKey> inputKeys);
        bool IsKeyDown(int i);
        bool IsKeyReleased(int i);
        KeyState GetKeyState(int i);
        void UpdateKeyStates(SDL_Event& event);
        void SetDefaultKeybinds();
        void SetKeyDownchecked(int i);
        bool HasNoInput();
        void CheckKeyDown(SDL_Keycode code);
        void CheckKeyUp(SDL_Keycode code);
    };
}

#endif // INPUTKEYSET_H_INCLUDED

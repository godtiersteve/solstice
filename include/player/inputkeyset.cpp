#include "inputkeyset.h"
namespace solstice {
InputKeyset::InputKeyset(int numKeys) {
    mInputKeys.resize(numKeys);
    SetDefaultKeybinds();
}

InputKeyset::InputKeyset(std::vector<ActionKey> inputKeys) {
    mInputKeys = inputKeys;
}

bool InputKeyset::IsKeyDown(int i) {return mInputKeys[i].keyState == KEY_DOWN;}
bool InputKeyset::IsKeyReleased(int i) {return mInputKeys[i].keyState == KEY_RELEASED;}
KeyState InputKeyset::GetKeyState(int i) {return mInputKeys[i].keyState;}
void InputKeyset::SetKeyDownchecked(int i) {
    if(mInputKeys[i].keyState == KEY_DOWN) {
        mInputKeys[i].keyState = KEY_DOWNCHECKED;
    }
}
void InputKeyset::CheckKeyDown(SDL_Keycode code) {
    for(size_t i = 0; i < mInputKeys.size(); i++) {
        mInputKeys[i].CheckKeyDown(code);
    }
}
void InputKeyset::CheckKeyUp(SDL_Keycode code) {
    for(size_t i = 0; i < mInputKeys.size(); i++) {
        mInputKeys[i].CheckKeyUp(code);
    }
}
void InputKeyset::UpdateKeyStates(SDL_Event& event) {
    for(size_t i = 0; i < mInputKeys.size(); i++) {
        if(event.type == SDL_KEYUP) {
            mInputKeys[i].Update(event);
        }
        else if(event.type == SDL_KEYDOWN) {
            mInputKeys[i].Update(event);
        }
    }
}
bool InputKeyset::HasNoInput() {
    for(size_t i = 0; i < mInputKeys.size(); i++) {
        if(mInputKeys[i].keyState == KEY_DOWN) {
            return false;
        }
    }
    return true;
}
void InputKeyset::SetDefaultKeybinds() {
    if(mInputKeys.size() >= INPUT_KEY_MAX) {
        mInputKeys[INPUT_KEY_UP].keyCode = SDLK_UP;
        mInputKeys[INPUT_KEY_DOWN].keyCode = SDLK_DOWN;
        mInputKeys[INPUT_KEY_LEFT].keyCode = SDLK_LEFT;
        mInputKeys[INPUT_KEY_RIGHT].keyCode = SDLK_RIGHT;
        mInputKeys[INPUT_KEY_A].keyCode = SDLK_z;
        mInputKeys[INPUT_KEY_B].keyCode = SDLK_x;
        mInputKeys[INPUT_KEY_X].keyCode = SDLK_c;
        mInputKeys[INPUT_KEY_Y].keyCode = SDLK_c;
        mInputKeys[INPUT_KEY_L1].keyCode = SDLK_a;
        mInputKeys[INPUT_KEY_L2].keyCode = SDLK_d;
        mInputKeys[INPUT_KEY_R1].keyCode = SDLK_s;
        mInputKeys[INPUT_KEY_R2].keyCode = SDLK_f;
        mInputKeys[INPUT_KEY_START].keyCode = SDLK_RETURN;
        mInputKeys[INPUT_KEY_SELECT].keyCode = SDLK_BACKSPACE;
        mInputKeys[INPUT_KEY_RIGHTSTICK].keyCode = -1;
        mInputKeys[INPUT_KEY_LEFT].keyCode = -1;
        mInputKeys[INPUT_KEY_LEFTSTICK_IN].keyCode = SDLK_q;
        mInputKeys[INPUT_KEY_RIGHTSTICK_IN].keyCode = SDLK_e;
    }
}

}

#ifndef ACTIONKEY_H_INCLUDED
#define ACTIONKEY_H_INCLUDED

#include "SDL2/sdl.h"
#include <string>

namespace solstice {
enum PlayerKey {PKEY_LEFT, PKEY_RIGHT, PKEY_UP, PKEY_DOWN, PKEY_JUMP, PKEY_ATTACK, PKEY_SHOOT,
                PKEY_BUTTON1, PKEY_BUTTON2, PKEY_BUTTON3, PKEY_BUTTON4, PKEY_BUTTON5, PKEY_BUTTON6,
                PKEY_CUSTOM};
enum KeyState {KEY_NONE, KEY_DOWN, KEY_RELEASED, KEY_DOWNCHECKED};
enum ExtendedButtons {SDL_CONTROLLER_BUTTON_L2 = SDL_CONTROLLER_BUTTON_MAX + 1,
                     SDL_CONTROLLER_BUTTON_R2,
                     SDL_CONTROLLER_BUTTON_LEFTSTICK_X,
                     SDL_CONTROLLER_BUTTON_LEFTSTICK_Y,
                     SDL_CONTROLLER_BUTTON_RIGHTSTICK_X,
                     SDL_CONTROLLER_BUTTON_RIGHTSTICK_Y};
struct ActionKey {
    private:
    int mUpdate;
    public:
    SDL_Keycode keyCode;
    int button;
    std::string name;
    int playerKey;
    KeyState keyState;
    bool keyDown, buttonDown;
    int axisMin, axisStrength;
    int mBehaviorID;
    ActionKey() {
        keyCode = SDLK_SPACE;
        playerKey = PKEY_JUMP;
        keyState = KEY_NONE;
        button = SDL_CONTROLLER_BUTTON_INVALID;
        keyDown = false;
        buttonDown = false;
        axisMin = 0;
        axisStrength = 0;
        mBehaviorID = -1;
        mUpdate = 5;
    }
    ActionKey(SDL_Keycode key, int pkey) {
        keyCode = key;
        playerKey = pkey;
        keyState = KEY_NONE;
        button = SDL_CONTROLLER_BUTTON_INVALID;
        keyDown = false;
        buttonDown = false;
        axisMin = 0;
        axisStrength = 0;
        mBehaviorID = -1;
        mUpdate = 5;
    }
    ActionKey(SDL_Keycode key, int pkey, int b) {
        keyCode = key;
        playerKey = pkey;
        keyState = KEY_NONE;
        button = b;
        keyDown = false;
        buttonDown = false;
        axisMin = 0;
        axisStrength = 0;
        mBehaviorID = -1;
        mUpdate = 5;
    }
    void SetAnalog(int b, int threshold) {
        axisMin = threshold;
    }
    void Update(SDL_Event& ev) {
        if(keyState == KEY_RELEASED && !mUpdate) {
            keyState = KEY_NONE;
        }
        if(mUpdate > 0) {
            mUpdate--;
        }
        if(ev.type == SDL_KEYUP) {
            if(ev.key.keysym.sym == keyCode) {
                keyDown = false;
                keyState = KEY_RELEASED;
                mUpdate = 5;
            }
        }
        else if(ev.type == SDL_KEYDOWN) {
            if(ev.key.keysym.sym == keyCode && keyState != KEY_DOWNCHECKED) {
                keyDown = true;
                keyState = KEY_DOWN;
            }
        }
    }
    void Update(SDL_Event& ev, SDL_GameController* controller) {
        Update(ev);
        if(controller && button != SDL_CONTROLLER_BUTTON_INVALID && button < SDL_CONTROLLER_BUTTON_L2) {
            if(SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)button)) {
                buttonDown = true;
            }
            else {
                buttonDown = false;
            }
            if(axisMin) {
                switch(button) {
                    case SDL_CONTROLLER_BUTTON_L2:
                        axisStrength = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                        break;
                    case SDL_CONTROLLER_BUTTON_R2:
                        axisStrength = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK_X:
                        axisStrength = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK_Y:
                        axisStrength = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK_X:
                        axisStrength = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK_Y:
                        axisStrength = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
                        break;
                    default:
                        axisStrength = 0;
                        break;
                }
                if(axisMin > 0) {
                    if(axisStrength <= axisMin) {
                        buttonDown = true;
                    }
                }
                else if(axisMin > 0) {
                    if(axisStrength >= axisMin) {
                        buttonDown = true;
                    }
                }
            }
        }
        if(!keyDown && !buttonDown) {
            keyState = KEY_RELEASED;
        }
        else if(keyState != KEY_DOWNCHECKED) {
            keyState = KEY_DOWN;
        }
    }
    void CheckKeyDown(SDL_Keycode code) {
        if(keyCode == code && keyState != KEY_DOWNCHECKED) {
            keyState = KEY_DOWN;
        }
    }
    void CheckKeyUp(SDL_Keycode code) {
        if(keyCode == code) {
            keyState = KEY_RELEASED;
        }
    }
};
}

#endif // ACTIONKEY_H_INCLUDED

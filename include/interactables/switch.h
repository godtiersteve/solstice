#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

#include "activeinteractable.h"
#include <memory>
#include <functional>

namespace solstice {
    enum SwitchType{SWITCH_TOUCH_TOP = 1, SWITCH_PULL = 2,
                    SWITCH_INTERACT = 4, SWITCH_SHOOT = 8};
    class Switch : public ActiveInteractable {
    protected:
        int mSwitchType;
        void OnActivate(Interactable* obj) {if(OnActivateFunc) OnActivateFunc(this);}
        void OnDeactivate(Interactable* obj) {if(OnDeactivateFunc) OnDeactivateFunc(this);}
    public:
        Switch(bool activated = false) {SetActivated(activated);}
        Switch(bool activated, std::function<void(Switch*)> onActivate, std::function<void(Switch*)> onDeactivate);
        std::function<void(Switch*)> OnActivateFunc;
        std::function<void(Switch*)> OnDeactivateFunc;
        std::shared_ptr<Interactable> mObject;
        bool GetActivated() {return mActivated;}
        bool SetActivated(bool b);
        virtual bool Toggle() {mActivated = !mActivated; if(OnActivateFunc) OnActivateFunc(this);}
        void SetObject(std::shared_ptr<Interactable> obj) {mObject = obj;}
        void OnNotify(const Interactable& entity, InteractableEvent event);
    };
}

#endif // SWITCH_H_INCLUDED

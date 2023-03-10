#include "switch.h"

namespace solstice {
    Switch::Switch(bool activated, std::function<void(Switch*)> onActivate, std::function<void(Switch*)> onDeactivate) {
        OnActivateFunc = onActivate;
        OnDeactivateFunc = onDeactivate;
        mActivated = activated;
        mInteractableType = INTERACT_SWITCH;
    }
    bool Switch::SetActivated(bool b) {
        mActivated = b;
        if(mActivated && OnActivateFunc) {
            OnActivateFunc(this);
        }
        else if(!mActivated && OnDeactivateFunc) {
            OnDeactivateFunc(this);
        }
    }
    void Switch::OnNotify(const Interactable& entity, InteractableEvent event) {
    }
}

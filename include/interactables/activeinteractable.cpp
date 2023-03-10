#include "activeinteractable.h"

namespace solstice {
    void ActiveInteractable::SetActivated(bool activated) {
        if(activated != mActivated) {
            if(activated) {
                OnActivate(this);
            }
            else {
                OnDeactivate(this);
            }
        }
        mActivated = activated;
    }
}

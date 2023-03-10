#ifndef ACTIVEINTERACTABLE_H_INCLUDED
#define ACTIVEINTERACTABLE_H_INCLUDED

#include "interactable.h"
#include "events/interactableevent.h"

namespace solstice {
    class ActiveInteractable : public Interactable, public InteractableEventObserver {
    protected:
        bool mActivated;
        virtual void OnActivate(Interactable* obj) = 0;
        virtual void OnDeactivate(Interactable* obj) = 0;
    public:
        ActiveIntercatable(bool active = false) {mActivated = active;}
        void SetActivated(bool activated);
        virtual void OnNotify(const Interactable& entity, InteractableEvent event) = 0;
    };
}

#endif // ACTIVEINTERACTABLE_H_INCLUDED

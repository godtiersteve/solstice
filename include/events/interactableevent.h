#ifndef INTERACTABLEEVENT_H_INCLUDED
#define INTERACTABLEEVENT_H_INCLUDED

#include "eventobserver.h"
#include "interactables/interactable.h"

namespace solstice {
    enum InteractableEvent {
        INTERACTABLE_ACTIVATED,
        INTERACTABLE_DEACTIVATED,
        INTERACTABLE_DESTROYED,
        INTERACTABLE_SPAWNED,
        INTERACTABLE_SPOTTED
    };
    typedef EventObserver<InteractableEvent, Interactable> InteractableEventObserver;
    typedef EventSubject<InteractableEvent, Interactable> InteractableEventSubject;
}

#endif // INTERACTABLEEVENT_H_INCLUDED

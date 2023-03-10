#ifndef CHECKPOINT_H_INCLUDED
#define CHECKPOINT_H_INCLUDED

#include "graphics/vector2.h"
#include "interactable.h"

namespace solstice {
    class Checkpoint : public ActiveInteractable {
    protected:
        void OnActivate(Interactable* obj) {}
        void OnDeactivate(Interactable* obj) {}
    public:
        Checkpoint(Vector2f spawnpos = 0) {
            mHitBox.SetPosition(spawnpos);
            mInteractableType = INTERACT_CHECKPOINT;
        }
        Vector2f GetPosition() {return mHitBox.GetPosition();}
        void OnNotify(const Interactable& entity, InteractableEvent event) {
    };
}


#endif // CHECKPOINT_H_INCLUDED

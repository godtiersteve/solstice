#include "door.h"

namespace solstice {

Door::Door(bool locked) {
    mObjectType = OBJECT_DOOR;
    mMaxHealth = 0;
    mKeyID = -1;
    mLocked = locked;
    mInteractableType = INTERACT_DOOR;
}

void SwitchUnlockDoor(Switch* swi) {
    if(swi->mObject && swi->mObject->IsType(OBJECT_DOOR)) {
        if(swi->GetActivated()) {
            static_cast<Door*>(swi->mObject.get())->Open();
        }
        else {
            static_cast<Door*>(swi->mObject.get())->Close();
        }
    }
}
std::shared_ptr<Switch> CreateDoorSwitch() {
    return nullptr;
}
}

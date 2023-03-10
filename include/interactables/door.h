#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "interactable.h"
#include "switch.h"
#include <memory>

namespace solstice {
    enum DoorType {DOOR_SWITCH, DOOR_DOOR, DOOR_KEY, DOOR_BREAKABLE};
    class Door : public Interactable {
        protected:
            bool mLocked;
            DoorType mDoorType;
            int mMaxHealth, mCurrentHealth;
        public:
            int mKeyID;
            Door(bool locked = true);
            virtual void Open() {mLocked = false;}
            virtual void Close() {mLocked = true;}
            void Toggle() {mLocked = !mLocked;}
            void SetLocked(bool b) {mLocked = b;}
            bool GetLocked() {return mLocked;}
            bool IsOpen() {return !mLocked;}
            DoorType GetDoorType() {return mDoorType;}
            bool IsDoorType(DoorType t) {return mDoorType == t;}
            bool IsIndestructable() {return mMaxHealth <= 0;}
            int GetMaxHealth() {return mMaxHealth;}
            int GetCurrentHealth() {return mCurrentHealth;}
            void SetHealth(int max, int cur) {mMaxHealth = max, mCurrentHealth = cur;}
    };
    void SwitchUnlockDoor(Switch* swi);
    std::shared_ptr<Switch> CreateDoorSwitch();
}

#endif // DOOR_H_INCLUDED

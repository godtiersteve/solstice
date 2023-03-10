#include "crate.h"

namespace solstice {
Crate::Crate(int health, int modelID) {
    mHealth.mMax = health;
    mHealth.mCurrent = health;
    mModelID = modelID;
}
void Crate::SetCrateType(CrateType t) {
    mType = t;
    if(t == CRATE_GRATED || t == CRATE_METAL) {
        mNoFloat = true;
    }
    else {
        mNoFloat = false;
    }
}
}

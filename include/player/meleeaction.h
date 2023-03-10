#ifndef MELEEACTION_H_INCLUDED
#define MELEEACTION_H_INCLUDED

#include <vector>

namespace solstice {
    struct MeleeAction {
        int mCancelWindowStart, mCancelWindowEnd;
        int mNextActionID;
        int mFirstActiveFrame;
        bool mSpawnsPorjectile;
        std::vector<int> mCancelsInto;

    };
    struct MeleeCombo {
        int mCurrentAction;
        std::vector<MeleeAction> mActions;
    };
}

#endif // MELEEACTION_H_INCLUDED

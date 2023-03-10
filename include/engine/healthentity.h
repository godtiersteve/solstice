#ifndef HEALTHENTITY_H_INCLUDED
#define HEALTHENTITY_H_INCLUDED

namespace solstice {
    struct HealthEntity {
        int mMin, mMax, mCurrent;
        HealthEntity(int min = 0 , int max = 100, int cur = 0) {
            mMin = min;
            mMax = max;
            mCurrent = cur;
        }
    };

}

#endif // HEALTHENTITY_H_INCLUDED

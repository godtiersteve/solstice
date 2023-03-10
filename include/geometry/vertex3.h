#ifndef VERTEX3_H_INCLUDED
#define VERTEX3_H_INCLUDED

#include "graphics/Vector2.h"

namespace solstice {
    template<class T>
    class Vertex3 : public Vector2<T>
    {
        protected:
            float mPitch, mYaw, mRoll;
        public:
            T z;
    };
}

#endif // VERTEX3_H_INCLUDED

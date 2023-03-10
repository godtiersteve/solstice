#ifndef GAMELEVEL_H_INCLUDED
#define GAMELEVEL_H_INCLUDED

#include "room.h"

namespace solstice {
class GameLevel {
private:
    int mCurrentRoom;
public:
    std::vector<LevelRoom> mRooms;
};

}

#endif // GAMELEVEL_H_INCLUDED

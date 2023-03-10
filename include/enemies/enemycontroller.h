#ifndef ENEMYCONTROLLER_H_INCLUDED
#define ENEMYCONTROLLER_H_INCLUDED

#include "enemybase.h"
#include "player/inputkeyset.h"

namespace solstice {
    class EnemyController {
    public:
        InputKeyset mKeyset;
        std::shared_ptr<EnemyBase> mEnemy;

        EnemyController(std::shared_ptr<EnemyBase> enemy = nullptr);
        virtual void Update(double dt);
        void CheckInput(SDL_Event& event);
    };
}

#endif // ENEMYCONTROLLER_H_INCLUDED

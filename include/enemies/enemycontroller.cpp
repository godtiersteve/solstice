#include "enemycontroller.h"

namespace solstice {
EnemyController::EnemyController(std::shared_ptr<EnemyBase> enemy) {
    mEnemy = enemy;
}
void EnemyController::Update(double dt) {
    if(mEnemy) {
        if(!mEnemy->CheckState(MOVE_SWIMMING_UNDERWATER)) {
            if(mKeyset.IsKeyDown(INPUT_KEY_LEFT)) {
                mEnemy->ChangeToBehavior(BEHAVIOR_WALK, Vector2f(-1.0, 1.0));
            }
            else if(mKeyset.IsKeyDown(INPUT_KEY_RIGHT)) {
                mEnemy->ChangeToBehavior(BEHAVIOR_WALK, Vector2f(1.0, 1.0));
            }
            if(mKeyset.IsKeyDown(INPUT_KEY_UP)) {
                mEnemy->ChangeToBehavior(BEHAVIOR_JUMP, Vector2f(0.0, 1.0));
            }
            if(mEnemy->GetUnderwater()) {
                mEnemy->ChangeToBehavior(BEHAVIOR_DIVE, Vector2f(0.0, 1.0));
            }
            if(mKeyset.IsKeyDown(INPUT_KEY_SELECT)) {
                mEnemy->ForceBehavior(-1);
            }
            if(mKeyset.HasNoInput()) {
                mEnemy->ChangeBehavior(-1);
            }
        }
        else {
            Vector2i direction(0);
            if(mKeyset.IsKeyDown(INPUT_KEY_LEFT)) {
                direction.x = -1;
            }
            else if(mKeyset.IsKeyDown(INPUT_KEY_RIGHT)) {
                direction.x = 1;
            }
            if(mKeyset.IsKeyDown(INPUT_KEY_UP)) {
                direction.y = 1;
            }
            else if(mKeyset.IsKeyDown(INPUT_KEY_DOWN)) {
                direction.y = -1;
            }
            if(direction.Magnitude2()) {
                mEnemy->ChangeToBehavior(BEHAVIOR_SWIM, direction);
            }
            else if(mKeyset.IsKeyDown(INPUT_KEY_SELECT)) {
                mEnemy->ForceBehavior(-1);
            }
            else if(mKeyset.HasNoInput()) {
                mEnemy->ChangeBehavior(-1);
            }
        }
        for(size_t i = 0; i < mKeyset.mInputKeys.size(); i++) {
            if(mKeyset.IsKeyDown(i) && mKeyset.mInputKeys[i].mBehaviorID > -1) {
                mEnemy->ChangeBehavior(mKeyset.mInputKeys[i].mBehaviorID);
            }
        }
        mEnemy->Update(dt);
    }
}

void EnemyController::CheckInput(SDL_Event& event) {
    mKeyset.UpdateKeyStates(event);
}
}

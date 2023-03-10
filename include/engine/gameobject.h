#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED


#include <string>

namespace solstice {

enum GameObjectTypes {  OBJECT_PLAYER, OBJECT_ENEMY, OBJECT_BACKGROUND, OBJECT_OBJECT,
                        OBJECT_BULLET, OBJECT_BUTTON, OBJECT_DOOR, OBJECT_SWITCH, OBJECT_HITBOX,
                        OBJECT_BARREL, OBJECT_CRATE};
enum GameEngineEntities {ENTITY_ENEMY, ENTITY_PLAYER, ENTITY_SHOOTING};

class GameObject {
protected:
    int mObjectType;
    bool mCanShoot;
public:
    std::string mName;
    GameObject() {
        mCanShoot = false;
        mObjectType = -1;
    }
    bool IsType(int type) const {return mObjectType == type;}
    bool CanShoot() {return mCanShoot;}
    virtual bool HasEntity(int e) {return false;}
    virtual GameObject* GetEntity(int e) {return nullptr;}
    virtual int GetCurFrameNum() {return -1;}
};
}


#endif // GAMEOBJECT_H_INCLUDED

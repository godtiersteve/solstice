#ifndef CRATE_H_INCLUDED
#define CRATE_H_INCLUDED

#include "physics/physics.h"
#include "graphics/renderableobject.h"
#include "engine/healthentity.h"

namespace solstice {
enum CrateType {CRATE_WOOD, CRATE_METAL, CRATE_GRATED};
class Crate : public PhysicsBox2f, public RenderableObject {
    bool mDestructable, mThrowable, mShootThough;
    CrateType mType;
public:
    HealthEntity mHealth;
    Crate(int health = 10, int modelID = -1);
    int GetModelID()                {return mModelID;}
    bool GetDestructable()          {return mDestructable;}
    bool GetThrowable()             {return mThrowable;}
    bool GetShootThrough()          {return mShootThough;}
    CrateType GetType()             {return mType;}
    void SetModelID(int id)         {mModelID = id;}
    void SetDestructable(bool d)    {mDestructable = d;}
    void SetThrowable(bool t)       {mThrowable = t;}
    void SetShootThrough(bool s)    {mShootThough = s;}
    void SetCrateType(CrateType t);
};
}

#endif // CRATE_H_INCLUDED

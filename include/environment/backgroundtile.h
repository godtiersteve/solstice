#ifndef BACKGROUNDTILE_H_INCLUDED
#define BACKGROUNDTILE_H_INCLUDED

#include "geometry/rectangle.h"
#include "geometry/polygon.h"
#include "geometry/hitbox.h"
#include "physics/spring.h"
#include "graphics/multiquad.h"

#include <fstream>
#include <memory>

namespace solstice {
enum TileType {TILE_GROUND = 0, TILE_MOVING = 2, TILE_PLATFORM = 4, TILE_WATER = 8};
enum TileStyle {TILE_FLAT, TILE_SLOPE_30_L, TILE_SLOPE_45_L, TILE_SLOPE_60_L, TILE_SLOPE_30_R, TILE_SLOPE_45_R, TILE_SLOPE_60_R};
enum CollisionHint {COLHINT_GROUND = 1, COLHINT_WALL = 2, COLHINT_CEILING = 4, COLHINT_SLOPE = 8};
class BackgroundTile2 {
private:
    float mFriction;
    Vector2f mStartPosition, mEndPosition;
    TileType mType;
    float mPathSpeed;
    Spring mSpring;
    bool mSprings;
    bool mReleased;
    short mCollisions;
    float mWeightHeld;
    Rectanglef mHitBox;
    float mElasticity;
    void Init(const BackgroundTile2& bg);
public:
    HitBoxf mHitBox2;
    int mStyle, mCollisionHint;
    float mAngle;
    bool mFallThrough;
    BackgroundTile2(TileType type = TILE_GROUND);
    BackgroundTile2(Rectanglef hitbox, TileType type = TILE_GROUND);
    BackgroundTile2(std::shared_ptr<IShapef> shape, TileType type = TILE_GROUND);
    BackgroundTile2(const BackgroundTile2& bg);
    BackgroundTile2& operator=(BackgroundTile2 bg);
    BackgroundTile2& operator=(Rectanglef& hitbox);
    Rectanglef& operator()() {return mHitBox;}
    Rectanglef& GetHitBox()                  {return mHitBox;}
    Rectanglef* GetHitBoxPtr()               {return &mHitBox;}
    HitBoxf& GetHitBox2()                    {return mHitBox2;}
    std::shared_ptr<IShapef> GetHBox()      {return mHitBox2.Get();}
    void SetPosition(Vector2f position);
    void SetFriction(float f)               {mFriction = f;}
    void SetType(TileType t)                {mType = t; if(t == TILE_PLATFORM) {mFallThrough = true;} else {mFallThrough = false;}}
    void SetStartPosition(Vector2f pos)     {mStartPosition = pos;}
    void SetEndPosition(Vector2f pos)       {mEndPosition = pos;}
    void SetSprings(bool s)                {mSprings = s;}
    void AddCollision()                     {mCollisions++;}
    void AddWeight(float weight)            {mWeightHeld+= weight;}
    void SetElasticity(float e)             {mElasticity = e;}
    void SetPath(Vector2f start, Vector2f end, float speed);
    void Push(float velocity);

    float GetElasticity()               {return mElasticity;}
    short GetCollisions()               {return mCollisions;}
    void ResetCollisions()              {mCollisions = 0;}
    float GetFriction() const           {return mFriction;}
    TileType GetType()  const           {return mType;}
    bool GetSprings() const             {return mSprings;}
    bool Springing() const              {return fabs(mSpring.height - mSpring.targetHeight) > 0.01;}
    Spring& GetSpring()                 {return mSpring;}
    Spring* GetSpringPtr()              {return &mSpring;}
    void Release()                      {mReleased = true;}
    std::string Style();
    std::string Type();
    int HitBoxType()                    {return mHitBox2.Get()->GetType();}

    void SetHitBox(Rectanglef rect);

    void Update(float tstep = 1.0f);
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
    void Write2(std::ofstream& file);
    void Read2(std::ifstream& file);
};
}

#endif // BACKGROUNDTILE_H_INCLUDED

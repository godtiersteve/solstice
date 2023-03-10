#ifndef BULLET_TYPE_H_INCLUDED
#define BULLET_TYPE_H_INCLUDED

#include "bullet.h"
#include "geometry/hitdetection.h"

namespace solstice {
class BulletType {
protected:
    int mLastInactive, mLastActive;
    float mFireRate, mCurTime;
    int mMagicCost;
    float mShotForce;
    size_t mMaxShots, mShotsPerFrame;
    Vector2f mMultiShotDirStart, mMultiShotDirEnd;
    float mMultiShotAngleStart, mMultiShotAngleEnd;
    float mMultiShotAnglestep;
    std::string mName;
    Bullet mBaseBullet;
    std::vector<Bullet> mBullets;
    BulletType* mSubBullet;
    void InitBullets();
    kaguya::State* mState;
    std::string mPackageName;
    Color mFlashColor;
    kaguya::LuaRef mCreateFunc;
    bool mHoldShot, mSubBulletsOnSpawn, mChargeable, mShootOnRelease, mChargeHeld;
    Vector2f mChargeForce, mChargeForceStep, mChargeForceStart, mChargeDirection;
    Vector2f mTargetPosition;
public:
    BulletType();
    BulletType(Bullet base, int maxShots, float fireRate, float force, int type = BTYPE_PROJECTILE);
    BulletType(kaguya::State* state, std::string package, int maxShots, float fireRate, float force, int type = BTYPE_PROJECTILE);

    void Init(Bullet base, int maxShots, float fireRate, float force, int type = BTYPE_PROJECTILE);
    void Update(float dt);
    void Reset();
    void Draw(SceneRenderer& rend);
    void DrawHitBoxes(SceneRenderer& rend, Color col);
    void DrawSprayers(ShaderProgram& sh, TextureArray& textures);
    void DrawTargetRanges(SceneRenderer& rend, int verts, Color col);
    void DrawBlastRadiuses(SceneRenderer& rend, int verts, Color col);
    bool CreateShot(Vector2f direction);
    bool CreateShot(Vector2f direction, Vector2f position);
    bool CreateShot(Vector2f direction, Vector2f position, Vector2f vel);
    bool DefaultCreateShot(Vector2f directon, Vector2f position, Vector2f vel);
    bool CreateShotBullet(Vector2f direction, Vector2f position, Vector2f vel);
    bool CreateShotLaser(Vector2f direction, Vector2f position, Vector2f vel);
    bool CreateShotSustained(Vector2f direction, Vector2f position, Vector2f vel);
    Color& GetFlashColor()                          {return mFlashColor;}
    void SetFlashColor(Color c)                     {mFlashColor = c;}
    void SetFlashColor(float r, float g, float b)   {mFlashColor = Color(r, g, b);}
    int GetLastInactive()                           {return mLastInactive;}
    void IncrementLastInactive()                    {mLastInactive++;}
    bool CanCreate()                                {return mLastInactive >= 0 && mLastInactive < mBullets.size() && mCurTime >= mFireRate;}
    float GetAngleStep()                            {return DeltaAngle(mMultiShotAngleStart, mMultiShotAngleEnd) / mShotsPerFrame;}
    float GetShotForce()                            {return mShotForce;}
    void SetShotForce(float f)                      {mShotForce = f;}
    void SetCurTime(float c)                        {mCurTime = c;}
    float GetCurTime()                              {return mCurTime;}
    bool GetHoldShot()                              {return mHoldShot;}
    void SetHoldShot(bool h)                        {mHoldShot = h;}
    void SetTargetPosition(Vector2f t)              {mTargetPosition = t;}
    Vector2f GetTargetPosition()                    {return mTargetPosition;}
    int GetType()                                   {return mBaseBullet.GetType();}
    int GetMagicCost()                              {return mMagicCost;}
    void SetMagicCost(int m)                        {mMagicCost = m;}
    void SetSubBullet(BulletType* b)                {mSubBullet = b;}
    BulletType* GetSubBullet()                      {return mSubBullet;}
    void SetSubBulletsOnSpawn(bool s)               {mSubBulletsOnSpawn = s;}
    void SetChargeable(bool c)                      {mChargeable = c;}
    void SetShootOnRelease(bool s)                  {mShootOnRelease = s;}
    void SetChargeHeld(bool c)                      {mChargeHeld = c;}
    bool GetSubBulletsOnSpawn()                     {return mSubBulletsOnSpawn;}
    bool GetChargeable()                            {return mChargeable;}
    bool GetShootOnRelease()                        {return mShootOnRelease;}
    bool GetChargeHeld()                            {return mChargeHeld;}
    void SetChargeForce(Vector2f f)                 {mChargeForce = f;}
    void SetChargeForceStop(Vector2f c)             {mChargeForceStep = c;}
    void SetChargeForceStart(Vector2f s)            {mChargeForceStart = s;}
    void SetChargeDirection(Vector2f c)             {mChargeDirection = c;}
    Vector2f GetChargeDirection()                   {return mChargeDirection;}
    Vector2f GetChargeForce()                       {return mChargeForce;}
    Vector2f GetChargeForceStop()                   {return mChargeForceStart;}
    Vector2f GetChargeForceStart()                  {return mChargeForceStart;}
    void ReleaseCharge();

    void SetFireRate(float f)               {mFireRate = f;}
    void SetMaxShots(int m)                 {mMaxShots = m; InitBullets();}
    void SetName(string n)                  {mName = n;}
    void SetShotsPerFrame(size_t s)         {mShotsPerFrame = s;}
    void SetBaseBullet(Bullet b)            {mBaseBullet = b; InitBullets();}
    void SetMultiShotDirStart(Vector2f d)   {mMultiShotDirStart = d;}
    void SetMultiShotDirEnd(Vector2f d)     {mMultiShotDirEnd = d;}
    void SetMultiShotAngleStep(float a)    {mMultiShotAnglestep = a;}
    void SetMultiShotAngleArc(float start, float end) {mMultiShotAngleStart = start; mMultiShotAngleEnd = end;}
    void SetCreateFunc(kaguya::LuaRef func)           {mCreateFunc = func;}
    void SetState(kaguya::State* state, std::string package = "");

    Bullet& GetBaseBullet()                 {return mBaseBullet;}
    float GetFireRate()                     {return mFireRate;}
    int GetMaxShots()                       {return mMaxShots;}
    string GetName()                        {return mName;}
    Bullet& GetBullet(size_t i)             {return mBullets[i];}
    Bullet* GetBulletPtr(size_t i)          {return &mBullets[i];}
    size_t GetShotsPerFrame()               {return mShotsPerFrame;}
    Vector2f GetMultiShotDirStart()         {return mMultiShotDirStart;}
    Vector2f GetMultiShotDirEnd()           {return mMultiShotDirEnd;}
    float GetMultiShotAngleStart()          {return mMultiShotAngleStart;}
    float GetMultiShotAngleEnd()            {return mMultiShotAngleEnd;}
    void CheckCollisions(RigidBody* b);
    void CheckCollisions(BackgroundTile2& bg);
    //void CheckCollisions();
};

}

#endif // BULLET_TYPE_H_INCLUDED

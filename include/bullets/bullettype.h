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
    float mMultiShotAngleStart, mMultiShotAngleEnd;
    std::string mName;
    Bullet mBaseBullet;
    std::vector<Bullet> mBullets;
    BulletType* mSubBullet;
    void InitBullets();
    kaguya::State* mState;
    Color mFlashColor;
    kaguya::LuaRef mCreateFunc;
    float mSubCreateDelay;
    bool mHoldShot, mChargeable, mShootOnRelease;
    float mDelayTimer, mDelayTimerCur;
    Vector2f mChargeForceStep, mChargeForceStart, mChargeDirection;
    int mChargeFrames, mMaxChargeFrames, mChargeStep;
    Vector2f mTargetPosition, mAimDirection;
    Vector2f mNextSpawnPosition, mNextSpawnDir, mNextSpawnVel;
    int mNextToMake, mNextPerShot;
    std::string mPackageName, mCreateFuncName;
    int mShotType, mModType, mSubBulletID;
    bool mShotQueued, mChargingShot, mAutoReleaseCharge;
public:
    BulletType();
    BulletType(Bullet base, int maxShots, float fireRate, float force, int type);
    BulletType(kaguya::State* state, std::string package, int maxShots, float fireRate, float force, int type = BTYPE_PROJECTILE);
    BulletType(std::string package, int maxShots, float fireRate, float force, int type = BTYPE_PROJECTILE);
    bool operator<(const BulletType& t);
    bool operator>(const BulletType& t);

    virtual void Init(Bullet base, int maxShots, float fireRate, float force, int type = BTYPE_PROJECTILE);
    void SetDefaultAttributes();
    virtual void Update(float dt);
    virtual void SetPosition(glm::vec3 pos);
    void ForceCreate()              {mCurTime = mFireRate;}
    virtual void Reset();
    void Draw(SceneRenderer& rend);
    void DrawHitBoxes(SceneRenderer& rend, Color col);
    void DrawSprayers(ShaderProgram& sh, TextureArray& textures);
    void DrawTargetRanges(SceneRenderer& rend, int verts, Color col);
    void DrawBlastRadiuses(SceneRenderer& rend, int verts, Color col);
    virtual int CreateShot(Vector2f direction);
    virtual int CreateShot(Vector2f direction, Vector2f position);
    virtual int CreateShot(Vector2f direction, Vector2f position, Vector2f vel);
    virtual int DefaultCreateShot(Vector2f directon, Vector2f position, Vector2f vel);
    virtual int CreateShotBullet(Vector2f direction, Vector2f position, Vector2f vel);
    virtual int CreateShotLaser(Vector2f direction, Vector2f position, Vector2f vel);
    virtual int CreateShotSustained(Vector2f direction, Vector2f position, Vector2f vel);
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
    int GetType()                                   {return mBaseBullet.GetType();}
    int GetMagicCost()                              {return mMagicCost;}
    void SetMagicCost(int m)                        {mMagicCost = m;}
    void SetSubBullet(BulletType* b)                {mSubBullet = b;}
    BulletType* GetSubBullet()                      {return mSubBullet;}
    void SetChargeable(bool c)                      {mChargeable = c;}
    void SetShootOnRelease(bool s)                  {mShootOnRelease = s;}
    bool GetChargeable()                            {return mChargeable;}
    bool GetShootOnRelease()                        {return mShootOnRelease;}
    void SetChargeForceStep(Vector2f c)             {mChargeForceStep = c;}
    void SetChargeForceStart(Vector2f s)            {mChargeForceStart = s;}
    void SetChargeDirection(Vector2f c)             {mChargeDirection = c;}
    Vector2f GetChargeDirection()                   {return mChargeDirection;}
    Vector2f GetChargeForce()                       {return mChargeForceStart + mChargeForceStep * (float)(mChargeFrames / mChargeStep);}
    Vector2f GetChargeForceStep()                   {return mChargeForceStep;}
    Vector2f GetChargeForceStart()                  {return mChargeForceStart;}
    void SetSubCreateDelay(float f)                 {mSubCreateDelay = f;}
    float GetSubCreateDelay(float f)                {return mSubCreateDelay;}
    Vector2f GetNextSpawnPos()                       {return mNextSpawnPosition;}
    void SetNextSpawnPos(Vector2f pos)                {mNextSpawnPosition = pos;}
    float GetDelayTimer()                           {return mDelayTimer;}
    void SetDelayTimer(float t)                     {mDelayTimer = t;}
    float GetDelayTimerCur()                            {return mDelayTimerCur;}
    void IncrementDelayTimer()                          {mDelayTimerCur++;}
    Vector2f GetNextSpawnDir()                          {return mNextSpawnDir;}
    void SetNextSpawnDir(Vector2f d)                    {mNextSpawnDir = d;}
    void SetNextToMake(int s)                           {mNextPerShot = s;}
    void SetNextPerShot(int s)                          {mNextPerShot = s;}
    int GetNextToMake()                                 {return mNextToMake;}
    int GetNextPerShot()                                {return mNextPerShot;}
    void SetTargetPosition(Vector2f pos)                {mTargetPosition = pos;}
    Vector2f GetTargetPosition()                        {return mTargetPosition;}
    std::string GetPackageName()                        {return mPackageName;}
    void SetPackageName(std::string package)            {mPackageName = package;}
    void SetModType(int t)                              {mModType = t;}
    void SetShotType(int t)                             {mShotType = t;}
    int GetModType()                                    {return mModType;}
    int GetShotType()                                   {return mShotType;}
    int GetSubBulletID()                                {return mSubBulletID;}
    void SetSubBulletID(int id)                         {mSubBulletID = id;}
    void SetAimDirection(Vector2f dir)                  {mAimDirection = dir;}
    Vector2f GetAimDirection()                          {return mAimDirection;}
    bool GetShotQueued()                                {return mShotQueued;}
    void SetNextSpawnVel(Vector2f vel)                  {mNextSpawnVel = vel;}
    Vector2f GetNextSpawnVel()                          {return mNextSpawnVel;}
    bool GetChargingShot()                              {return mChargingShot;}
    void SetChargingShot(bool c)                        {mChargingShot = c;}
    void SetShotQueued(bool q)                          {mShotQueued = q;}
    void SetChargeStep(int step)                        {mChargeStep = step;}
    int GetChargeStep()                                 {return mChargeStep;}
    int GetChargeFrames()                               {return mChargeFrames;}
    void SetTypes(int shotType, int modType)            {mShotType = shotType; mModType = modType;}
    void SetCharge(Vector2f startForce, Vector2f stepForce, int maxFrames, int frameStep, bool autoRelease);
    void SetAutoReleaseCharge(bool a)                   {mAutoReleaseCharge;}
    bool GetAutoReleaseCharge()                         {return mAutoReleaseCharge;}

    void QueueShot();
    void InitBullets(kaguya::State* state);
    void CreateSubBullet(Vector2f dir, Vector2f pos);
    void SetRenderer(ParticleRenderer* p);
    void ReleaseCharge();
    int FindNextInactive();

    void SetFireRate(float f)               {mFireRate = f;}
    void SetMaxShots(int m)                 {mMaxShots = m; InitBullets();}
    void SetName(string n)                  {mName = n;}
    void SetShotsPerFrame(size_t s)         {mShotsPerFrame = s;}
    void SetBaseBullet(Bullet b)            {mBaseBullet = b; InitBullets();}
    void SetMultiShotAngleArc(float start, float end) {mMultiShotAngleStart = start; mMultiShotAngleEnd = end;}
    void SetCreateFunc(kaguya::LuaRef func)           {mCreateFunc = func;}
    void SetCreateFuncStr(std::string package, std::string str);
    void SetState(kaguya::State* state, std::string package = "");

    Bullet& GetBaseBullet()                 {return mBaseBullet;}
    float GetFireRate()                     {return mFireRate;}
    int GetMaxShots()                       {return mMaxShots;}
    string GetName()                        {return mName;}
    Bullet& GetBullet(size_t i)             {return mBullets[i];}
    Bullet* GetBulletPtr(size_t i)          {return &mBullets[i];}
    size_t GetShotsPerFrame()               {return mShotsPerFrame;}
    float GetMultiShotAngleStart()          {return mMultiShotAngleStart;}
    float GetMultiShotAngleEnd()            {return mMultiShotAngleEnd;}
    void CheckCollisions(RigidBody* b);
    void CheckCollisions(BackgroundTile2& bg);
    //void CheckCollisions();
};

}

#endif // BULLET_TYPE_H_INCLUDED

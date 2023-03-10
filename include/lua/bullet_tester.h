#ifndef BULLET_TESTER_H_INCLUDED
#define BULLET_TESTER_H_INCLUDED

#include "bullets/bullettype.h"
#include "graphics/texture.h"

namespace solstice {
class BulletTester {
    std::vector<PhysicsBox2f> mBodies;
    ParticleRenderer mAddRenderer, mOpaqueRenderer;
    Texture mTexture;
    int mActiveBullet;
public:
    std::vector<std::shared_ptr<BulletType> > mBullets, mSubBullets;
    BulletTester();
    Color hitBoxColor, enemyColor, targetColor, blastColor;
    void Draw(SceneRenderer& rend);
    void Update(double dt);
    void DrawHitboxes(SceneRenderer& rend);
    void DrawRadii(SceneRenderer& red);
    void DrawParticles(ShaderProgram& sh);
    void CheckCollisions(BackgroundTile2& ground);
    void CheckCollisions();
    void InitBullets(kaguya::State& state);
    void SetupBinds(kaguya::State& state);
    void Reset();
    void AddBody(PhysicsBox2f body)             {mBodies.push_back(body);}
    ParticleRenderer& GetAddRenderer()         {return mAddRenderer;}
    ParticleRenderer& GetOpaqueRenderer()      {return mOpaqueRenderer;}
    void AddBullet(BulletType b)            {mBullets.push_back(std::make_shared<BulletType>(b));}
    void AddSubBullet(BulletType b)         {mSubBullets.push_back(std::make_shared<BulletType>(b));}
    std::vector<std::shared_ptr<BulletType> >& GetBullets()   {return mBullets;}
    std::vector<std::shared_ptr<BulletType> >& GetSubBullets()    {return mSubBullets;}
    BulletType* GetBullet(size_t i)         {return mBullets[i].get();}
    BulletType* GetSubBullet(size_t i)      {return mSubBullets[i].get();}
    BulletType* GetLastBullet()             {return mBullets.back().get();}
    BulletType* GetLastSubBullet()          {return mSubBullets.back().get();}
    Texture* GetTexture()                   {return &mTexture;}
    BulletType& GetActiveBullet()           {return *mBullets[mActiveBullet].get();}
    bool empty() {return mBullets.empty();}
    void ReleaseShots();
    void NextActive();
    void LastActive();
    void CreateShot(Vector2f dir, Vector2f pos, Vector2f speed);
    void ResetBodies();
    void ClearBullets()                         {mBullets.clear(); mSubBullets.clear();}
    int GetActiveBulletID()                     {return mActiveBullet;}
};
}

#endif // BULLET_TESTER_H_INCLUDED

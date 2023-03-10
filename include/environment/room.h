#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "environment/waterbody.h"
#include "player/camera2d.h"
#include "physics/physicsrect.h"
#include "physics/chain.h"
#include "graphics/3d/light.h"
#include "physics/crate.h"
#include "physics/barrel.h"
#include <vector>
#include <fstream>

namespace solstice {
struct ChainBoxPair {
    size_t boxID, chainID;
    ChainBoxPair(size_t box = 0, size_t chain = 0) {
        boxID = box;
        chainID = chain;
    }
};
class LevelRoom {
private:
    std::vector<BackgroundTile2> mGroundTiles;
    std::vector<WaterBody> mWaterBodies;
    std::vector<Crate> mBoxes;
    std::vector<Barrel*> mBarrels;
    std::vector<RigidBody*> mBodyPointers;
    std::vector<Chain> mChains;
    std::vector<Light> mLights;
    std::vector<ChainBoxPair> mChainBoxPairs;
    float mHeight, mWidth;
    Vector2f mPosition;
    CameraBoundary mCamBoundary, mWaterCamBoundary;
    std::string mName;
    int mModelID;
public:
    LevelRoom();
    LevelRoom(string path);
    LevelRoom(std::vector<BackgroundTile2> groundTiles, std::vector<WaterBody> waterBodies);
    LevelRoom& operator=(const LevelRoom& r);
    LevelRoom(const LevelRoom& r);

    void SetGroundTiles(std::vector<BackgroundTile2> tiles) {mGroundTiles = tiles;}
    void SetWaterBodies(std::vector<WaterBody> waterBodies) {mWaterBodies = waterBodies;}
    void SetBoxes(std::vector<Crate> boxes)                 {mBoxes = boxes;}
    void SetChains(std::vector<Chain> chains)               {mChains = chains;}
    void SetLights(std::vector<Light> lights)               {mLights = lights;}
    std::vector<BackgroundTile2>& GetGroundTiles()          {return mGroundTiles;}
    std::vector<WaterBody>& GetWaterBodies()                {return mWaterBodies;}
    std::vector<Crate>&  GetBoxes()                          {return mBoxes;}
    std::vector<Barrel*>& GetBarrels()                      {return mBarrels;}
    std::vector<Chain>  GetChains()                         {return mChains;}
    std::vector<Light>& GetLights()                         {return mLights;}
    std::vector<RigidBody*>& GetBodyPointers()              {return mBodyPointers;}
    Crate& GetCrate(size_t i)                               {return mBoxes[i];}
    Crate* GetCratePtr(size_t i)                            {return &mBoxes[i];}
    void AddBarrel(Barrel* b)                               {mBarrels.push_back(b);}
    void AddCrate(Crate c);
    void AddChain(Chain c);
    void InitChainBoxPairs();
    void ClearChainBoxPairs()                               {mChainBoxPairs.clear();}
    void AddTile(BackgroundTile2 t);
    void AddLight(Light l)                                  {mLights.push_back(l);}
    void AddWaterBody(WaterBody b)                          {mWaterBodies.push_back(b);}
    void ClearCrates()                                      {mBoxes.clear();}
    void PopulateBodyPointers();
    float GetHeight()                                       {return mHeight;}
    float GetWidth()                                        {return mWidth;}
    Vector2f GetPosition()                                  {return mPosition;}
    void BindCamera(Camera& cam)                            {cam.Bind(mCamBoundary);}
    void BindWaterCamera(Camera& cam)                       {cam.Bind(mWaterCamBoundary);}
    CameraBoundary& GetCamBoundary()                        {return mCamBoundary;}
    void SetCamBoundary(CameraBoundary b)                   {mCamBoundary = b;}
    void SetWaterCamBoundary(CameraBoundary b)              {mWaterCamBoundary = b;}
    void SetName(std::string n)                             {mName = n;}
    std::string GetName()                                   {return mName;}
    void SetModelID(int id)                                 {mModelID = id;}
    int GetModelID()                                        {return mModelID;}
    BackgroundTile2& GetTile(size_t i)                      {return mGroundTiles[i];}
    void AttachBoxToChain(int boxID, int chainID);

    void SetHeight(float h) {mHeight = h;}
    void SetWidth(float w) {mWidth = w;}
    void SetHW(float h, float w) {mHeight = h; mWidth = w;}
    void SetPosition(Vector2f position) {mPosition = position;}
    void Move();
    void Move(Vector2f pos);
    void Move(float x, float y);
    void ResetCollisions();

    void Update(float tstep = 1.0f);
    void UpdateBoxes(double t);
    void CheckCollisions(RigidBody* body);
    void CheckSelfCollisions();

    void Write(string path);
    void Read(string path);

    void Write2(string path);
    void Read2(string path);
    void DrawHitBoxes(SceneRenderer& rend);
    virtual void DrawModels();
    virtual void Draw2DGraphics(SceneRenderer& rend);
};

}


#endif // ROOM_H_INCLUDED

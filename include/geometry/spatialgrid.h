#ifndef SPATIALGRID_H_INCLUDED
#define SPATIALGRID_H_INCLUDED

#include "hitbox.h"
#include "physics/physics.h"
#include "hitdetection.h"
#include "physics/collisionresolution.h"
#include "bullets/bullet.h"

#include <vector>
#include <cstdlib>
///***TODO: Implement collision partitioning for bullets***///
namespace solstice {
class RectNode : public Rectanglef {
private:
    std::vector<RigidBody*> mBodies;
    std::vector<Bullet*> mBullets;
    std::vector<BackgroundTile2*> mTiles;
    size_t mBodiesSize, mBulletsSize, mTilesSize;
public:
    RectNode(size_t size = 0);
    void Resize(size_t size);
    void AddBody(RigidBody* body);
    void AddBullet(Bullet* b);
    void AddTile(BackgroundTile2* b);
    bool Empty() {return !mBodiesSize && !mBulletsSize && !mTilesSize;}
    void Clear() {mBodiesSize = 0; mBulletsSize = 0; mTilesSize = 0;}
    size_t GetBodiesSize() {return mBodiesSize;}
    size_t GetBulletsSize() {return mBulletsSize;}
    size_t GetTilesSize() {return mTilesSize;}
    std::vector<RigidBody*> GetBodies() {return mBodies;}
    std::vector<Bullet*>& GetBullets() {return mBullets;}
    std::vector<BackgroundTile2*>& GetTiles() {return mTiles;}
    RigidBody* GetBody(size_t i) {return mBodies[i];}
    Bullet* GetBullet(size_t i) {return mBullets[i];}
    BackgroundTile2* GetTile(size_t i) {return mTiles[i];}
};
class SpatialGrid {
private:
    int mCelSize;
    size_t mXNodes, mYNodes;
    Vector2f mMin;
    std::vector<std::vector<RectNode> > mNodes;
    std::vector<std::string> mChecked;
public:
    SpatialGrid(int celSize = 40, int xnodes = 10, int ynodes = 10, size_t buffer = 20);
    void Init(int celSize, int xnodes, int ynodes, size_t buffer = 20);

    RectNode& GetNode(size_t i, size_t j) {return mNodes[i][j];}
    inline std::string GetHash(size_t id1, size_t id2);
    std::vector<std::vector<RectNode> > GetNodes() {return mNodes;}
    void PopulateNodes(std::vector<RigidBody*>& bodies);
    void PopulateNodes(std::vector<Bullet*>& bullets);
    void PopulateNodes(std::vector<BackgroundTile2*>& tiles);
    void PopulateNodes(std::vector<PhysicsBox2f>& bodies);
    void PopulateNodes(RigidBody* body);
    void CheckCollisions();
    void ResetGrid();
    GridCell GetCells(Rectanglef& rect);
};

}

#endif // SPATIALGRID_H_INCLUDED

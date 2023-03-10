#include "spatialgrid.h"

namespace solstice {
    RectNode::RectNode(size_t size) {
        mBulletsSize = 0;
        mBodiesSize = 0;
        mTilesSize = 0;
        mBodies = std::vector<RigidBody*>(size);
        mBullets = std::vector<Bullet*>(size);
        mTiles = std::vector<BackgroundTile2*>(size);
    }
    void RectNode::Resize(size_t size) {
        mBodies.resize(size);
        mBullets.resize(size);
        mTiles.resize(size);
    }
    void RectNode::AddBody(RigidBody* body) {
        if(mBodiesSize >= mBodies.size()) {
            mBodies.resize(mBodies.size() * 2);
        }
        mBodies[mBodiesSize++] = body;
    }
    void RectNode::AddBullet(Bullet* b) {
        if(mBulletsSize >= mBullets.size()) {
            mBullets.resize(mBullets.size() * 2);
        }
        mBullets[mBulletsSize++] = b;
    }

    void RectNode::AddTile(BackgroundTile2* b) {
        if(mTilesSize >= mTiles.size()) {
            mTiles.resize(mTiles.size() * 2);
        }
        mTiles[mTilesSize++] = b;
    }
    SpatialGrid::SpatialGrid(int celSize, int xnodes, int ynodes, size_t buffer) {
        Init(celSize, xnodes, ynodes, buffer);
    }
    void SpatialGrid::Init(int celSize, int xnodes, int ynodes, size_t buffer) {
        mXNodes = xnodes;
        mYNodes = ynodes;
        mCelSize = celSize;
        mMin.x = -640;
        mMin.y = -360;
        mNodes.resize(xnodes);
        mChecked.resize(xnodes * xnodes);
        for(size_t i = 0; i < mXNodes; i++) {
            mNodes[i].resize(mYNodes);
            for(size_t j = 0; j < mYNodes; j++) {
                Vector2f pos(celSize * i + mMin.x, celSize * j + mMin.y);
                mNodes[i][j].Init(pos, celSize, celSize);
                mNodes[i][j].Resize(buffer);
            }
        }
    }
    void SpatialGrid::PopulateNodes(std::vector<RigidBody*>& bodies) {
        for(size_t i = 0; i < bodies.size(); i++) {
            PopulateNodes(bodies[i]);
        }
    }
    void SpatialGrid::PopulateNodes(std::vector<PhysicsBox2f>& bodies) {
        for(size_t i = 0; i < bodies.size(); i++){
            PopulateNodes(&bodies[i]);
        }
    }
    inline GridCell SpatialGrid::GetCells(Rectanglef& rect) {
        GridCell ret;
        ret.left = (int)(rect.GetLeft() - mMin.x) / mCelSize;
        ret.right = (int)(rect.GetRight() - mMin.x) / mCelSize;
        ret.top = (int)(rect.GetTop() - mMin.y) / mCelSize;
        ret.bottom = (int)(rect.GetBottom() - mMin.y) / mCelSize;
        return ret;
    }
    void SpatialGrid::PopulateNodes(RigidBody* body) {
        Rectanglef rect = body->GetHitBox().GetAABB();
        GridCell cel = GetCells(rect);
        if(!cel.OutOfRange(mXNodes, mYNodes)) {
            for(unsigned int i = cel.left; i <= cel.right; i++) {
                for(unsigned int j = cel.bottom; j <= cel.top; j++) {
                    mNodes[i][j].AddBody(body);
                }
            }
        }
    }
    void SpatialGrid::PopulateNodes(std::vector<Bullet*>& bullets) {
        for(size_t i = 0; i < bullets.size(); i++) {
            Rectanglef rect = (*bullets[i]).GetHitBox().GetAABB();
            GridCell cel = GetCells(rect);
            if(!cel.OutOfRange(mXNodes, mYNodes)) {
                for(unsigned int i = cel.left; i <= cel.right; i++) {
                    for(unsigned int j = cel.bottom; j <= cel.top; j++) {
                        mNodes[i][j].AddBullet(bullets[i]);
                    }
                }
            }
        }
    }
    void SpatialGrid::PopulateNodes(std::vector<BackgroundTile2*>& tiles) {
        for(size_t i = 0; i < tiles.size(); i++) {
            Rectanglef rect = (*tiles[i]).GetHitBox().GetAABB();
            GridCell cel = GetCells(rect);
            if(!cel.OutOfRange(mXNodes, mYNodes)) {
                for(unsigned int i = cel.left; i <= cel.right; i++) {
                    for(unsigned int j = cel.bottom; j <= cel.top; j++) {
                        mNodes[i][j].AddTile(tiles[i]);
                    }
                }
            }
        }
    }
    void SpatialGrid::CheckCollisions() {
        int usedHashes = 0;
        for(size_t i = 0; i < mXNodes; i++) {
            for(size_t j = 0; j < mYNodes; j++) {
                if(mNodes[i][j].GetBodiesSize() > 1) {
                    for(size_t k = 0; k < mNodes[i][j].GetBodiesSize(); k++) {
                        for(size_t l = 0; l < mNodes[i][j].GetBodiesSize(); l++) {
                            RigidBody* b1 = mNodes[i][j].GetBody(k);
                            RigidBody* b2 = mNodes[i][j].GetBody(l);
                            std::string hashA = GetHash(b1->GetID(), b2->GetID());
                            std::string hashB = GetHash(b2->GetID(), b1->GetID());
                            auto end = mChecked.begin() + usedHashes;
                            if(std::find(mChecked.begin(), end, hashA) == end &&
                               std::find(mChecked.begin(), end, hashB) == end) {
                                if((b1->GetID() != b2->GetID())) {
                                    b1->CheckAndResolveCollision(b2);
                                }
                                if(usedHashes >= mChecked.size()) {
                                    mChecked.resize(mChecked.size() * 2);
                                }
                                mChecked[usedHashes++] = hashA;
                                mChecked[usedHashes++] = hashB;
                                //mChecked.push_back(hashA);
                                //mChecked.push_back(hashB);
                            }
                        }
                    }
                }
            }
        }
    }
    std::string SpatialGrid::GetHash(size_t id1, size_t id2) {
        char buff[15];
        sprintf(buff, "%i", id1);
        std::string ret = buff;
        ret+= ":";
        sprintf(buff, "%i", id2);
        return ret + buff;
    }
    void SpatialGrid::ResetGrid() {
        for(size_t i = 0; i < mXNodes; i++) {
            for(size_t j = 0; j < mYNodes; j++) {
                mNodes[i][j].Clear();
            }
        }
    }
}

#include "room.h"

namespace solstice {
    LevelRoom::LevelRoom() {
        mHeight = 0;
        mWidth = 0;
        mPosition = 0;
        mModelID = -1;
    }
    LevelRoom::LevelRoom(std::vector<BackgroundTile2> groundTiles, std::vector<WaterBody> waterBodies) {
        mGroundTiles = groundTiles;
        mWaterBodies = waterBodies;
        mHeight = 0;
        mWidth = 0;
        mPosition = 0;
    }
    LevelRoom::LevelRoom(string path) {
        Read(path);
    }

    LevelRoom& LevelRoom::operator=(const LevelRoom& r) {
        mGroundTiles = r.mGroundTiles;
        mWaterBodies = r.mWaterBodies;
        mBoxes = r.mBoxes;
        mBodyPointers.clear();
        mChains = r.mChains;
        mChainBoxPairs = r.mChainBoxPairs;
        mLights = r.mLights;
        mHeight = r.mHeight;
        mWidth = r.mWidth;
        mPosition = r.mPosition;
        mCamBoundary = r.mCamBoundary;
        mWaterCamBoundary = r.mWaterCamBoundary;
        mName = r.mName;
        mModelID = r.mModelID;
        PopulateBodyPointers();
        InitChainBoxPairs();
        return *this;
    }
    LevelRoom::LevelRoom(const LevelRoom& r) {
        *this = r;
    }

    void LevelRoom::Move() {
        for(size_t i = 0; i < mGroundTiles.size(); i++) {
            mGroundTiles[i].GetHitBox().SetPosition(mGroundTiles[i].GetHitBox().GetOrigin() + mPosition);
        }
        for(size_t i = 0; i < mWaterBodies.size(); i++) {
            mWaterBodies[i].GetHitBox().SetPosition(mWaterBodies[i].GetPosition() + mPosition);
        }
    }
    void LevelRoom::Move(Vector2f pos) {
        Vector2f oldPos = mPosition;
        mPosition = pos;
        Move();
        mPosition = oldPos;
    }
    void LevelRoom::Move(float x, float y) {
        Vector2f oldPos = mPosition;
        mPosition.x = x;
        mPosition.y = y;
        Move();
        mPosition = oldPos;
    }
    void LevelRoom::ResetCollisions() {
        for(size_t i = 0; i < mGroundTiles.size(); i++) {
            mGroundTiles[i].ResetCollisions();
        }
    }
    void LevelRoom::Update(float tstep) {
        for(size_t i = 0; i < mWaterBodies.size(); i++) {
            mWaterBodies[i].Update(tstep);
        }
        for(size_t i = 0; i < mGroundTiles.size(); i++) {
            mGroundTiles[i].Update(tstep);
        }
    }
    void LevelRoom::UpdateBoxes(double t) {
        for(auto& chain : mChains) {
            chain.Update(t);
        }
        for(auto& box : mBoxes) {
            box.Update(t);
        }
        for(auto& barrel : mBarrels) {
            barrel->Update(t);
        }
    }
    void LevelRoom::CheckCollisions(RigidBody* body) {
        for(auto& g : mGroundTiles) {
            body->CheckAndResolveCollision(g);
        }
        bool hasCollision = false;
        for(auto& w : mWaterBodies) {
            if(body->CheckCollision(w)) {
                mWaterCamBoundary = w.GetCamBoundary();
                hasCollision = true;
            }
        }
        body->SetUnderwater(hasCollision);
    }
    void LevelRoom::CheckSelfCollisions() {
        for(auto& body : mBodyPointers) {
            body->SetGrounded(false);
            CheckCollisions(body);
        }
    }
    void LevelRoom::PopulateBodyPointers() {
        if(mBoxes.empty() && mBarrels.empty()) {
            return;
        }
        if(mBodyPointers.size() != mBoxes.size() + mBarrels.size()) {
            mBodyPointers.resize(mBoxes.size() + mBarrels.size());
        }
        for(size_t i = 0; i < mBoxes.size(); i++) {
            mBodyPointers[i] = &mBoxes[i];
        }
        for(size_t i = 0; i < mBarrels.size(); i++) {
            mBodyPointers[i + mBoxes.size()] = mBarrels[i];
        }
    }
    void LevelRoom::AddCrate(Crate c) {
        mBoxes.push_back(c);
        InitChainBoxPairs();
        PopulateBodyPointers();
    }
    void LevelRoom::AddChain(Chain c) {
        mChains.push_back(c);
        InitChainBoxPairs();
    }
    void LevelRoom::InitChainBoxPairs() {
        for(auto& p : mChainBoxPairs) {
            if((p.boxID < mBoxes.size()) && (p.chainID < mChains.size())) {
                mChains[p.chainID].Attach(&mBoxes[p.boxID]);
            }
        }
    }
    void LevelRoom::AttachBoxToChain(int boxID, int chainID) {
        mChainBoxPairs.push_back(ChainBoxPair(boxID, chainID));
        InitChainBoxPairs();
    }
    void LevelRoom::Write(string path) {
        ofstream file(path);
        BinaryWrite(file, mGroundTiles.size());
        for(std::vector<BackgroundTile2>::iterator i = mGroundTiles.begin(); i != mGroundTiles.end(); i++) {
            i->Write(file);
        }
        file.close();
    }
    void LevelRoom::Read(string path) {
        mGroundTiles.clear();
        mWaterBodies.clear();
        ifstream file(path);
        int amount;
        BinaryRead(file, amount);
        for(int i = 0; i < amount; i++) {
            BackgroundTile2 tile;
            tile.Read(file);
            mGroundTiles.push_back(tile);
        }
        file.close();
    }
    void LevelRoom::Write2(string path) {
        ofstream file(path);
        BinaryWrite(file, mGroundTiles.size());
        for(auto& tile : mGroundTiles) {
            tile.Write2(file);
        }
        file.close();
    }
    void LevelRoom::Read2(string path) {
        mGroundTiles.clear();
        mWaterBodies.clear();
        ifstream file(path);
        int amount;
        BinaryRead(file, amount);
        for(int i = 0; i < amount; i++) {
            BackgroundTile2 tile;
            tile.Read2(file);
            mGroundTiles.push_back(tile);
        }
        file.close();
    }
    void LevelRoom::AddTile(BackgroundTile2 t) {
        mGroundTiles.push_back(t);
    }
    void LevelRoom::DrawHitBoxes(SceneRenderer& rend) {
        for(size_t i = 0; i < mGroundTiles.size(); i++) {
            rend.DrawRect(mGroundTiles[i].GetHitBox(), Color(0.0, 1.0, 0.25, 0.75));
        }
        for(size_t i = 0; i < mBoxes.size(); i++) {
            rend.DrawRect(mBoxes[i].GetHitBox(), Color(1.0, 0.0, 0.0, 0.75));
        }
        for(size_t i = 0; i < mBarrels.size(); i++) {
            if(mBarrels[i]->GetKnockedOver()) {
                //rend.DrawShape(mBarrels[i]->GetHitBoxShape(), Color(1.0, 0.0, 0.0, 0.75f));
                rend.DrawCircle(mBarrels[i]->GetCircleHitBox(), 32, Color(1.0, 0.0, 0.0, 0.75f));
            }
            else {
                rend.DrawRect(mBarrels[i]->GetHitBox(), Color(1.0, 0.0, 0.0, 0.75f));
            }
            //rend.DrawRect(mBarrels[i]->GetHitBox(), Color(1.0, 0.0, 0.0, 0.75f));
        }
        for(size_t i = 0; i < mChains.size(); i++) {
            mChains[i].DrawHitBoxes(rend, Color(0.5, 0.5, 0.0, 0.25));
        }
        for(size_t i = 0; i < mWaterBodies.size(); i++) {
            rend.DrawRect(mWaterBodies[i].GetSurfaceRect(), Color(0.0, 1.0, 0.0, 0.75));
        }
    }
    void LevelRoom::DrawModels() {
    }
    void LevelRoom::Draw2DGraphics(SceneRenderer& rend) {
        for(size_t i = 0; i < mChains.size(); i++) {
            mChains[i].Draw(rend);
        }
    }
}

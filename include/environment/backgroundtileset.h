#ifndef BACKGROUNDTILESET_H_INCLUDED
#define BACKGROUNDTILESET_H_INCLUDED

#include "backgroundtile.h"
#include "rendertile.h"
#include "graphics/3d/mesh.h"
#include "graphics/3d/model.h"
#include "graphics/scenerenderer.h"
#include "engine/resizableobject.h"
#include "geometry/hitdetection.h"

namespace solstice {
    enum BackgroundTilesetType {BGTILE_LEFT, BGTILE_TOP, BGTILE_BOTTOM, BGTILE_RIGHT, BGTILE_TOPLEFT, BGTILE_TOPRIGHT, BGTILE_BOTLEFT, BGTILE_BOTRIGHT, BGTILE_CENTER};
    struct BackgroundTileset {
        RenderTile mTiles[9];
        BackgroundTileset() {}
        BackgroundTileset(const BackgroundTileset& tile) {
            *this = tile;
        }
        BackgroundTileset& operator=(const BackgroundTileset& tile) {
            for(size_t i = 0; i < 9; i++) {
                mTiles[i] = tile.mTiles[i];
            }
            return *this;
        }
        RenderTile& GetTile(size_t i) {return mTiles[i];}
        RenderTile& operator[](size_t i) {return mTiles[i];}
    };
    class TiledBackground : public ResizableObject {
    private:
        Vector2f mPosition;
        size_t mHeight, mWidth;
        Model mModel;
        std::vector<RenderTile> mTiles;
        std::vector<HitBoxf> mCollisionBoxes;
        Rectanglef mHitBox;
        void Resize();
        void Reposition();
    public:
        int mTileHeight, mTileWidth;
        std::shared_ptr<BackgroundTileset> mTileset;
        TiledBackground();
        TiledBackground(BackgroundTileset* tiles, size_t height, size_t width);
        TiledBackground(BackgroundTileset* tiles, size_t height, size_t width, size_t tileH, size_t tileW);
        void Init(BackgroundTileset* tiles, size_t height, size_t width, size_t tileH, size_t tileW);
        void Resize(size_t h, size_t w);
        void SetHeight(size_t h);
        void SetWidth(size_t w);
        void CreateModel(std::vector<Model>& models);
        void Draw(ShaderProgram& sh, TextureArray& textures);
        void Draw(std::vector<Model>& models, ShaderProgram& sh, TextureArray& textures, glm::mat4 trans, glm::mat4 projection);
        void DrawTiles(SceneRenderer& rend);
        BackgroundTilesetType GetTileType(size_t x, size_t y);
        size_t GetHeight() {return mHeight;}
        size_t GetWidth() {return mWidth;}
        Vector2f GetPosition() {return mPosition;}
        void SetPosition(Vector2f pos);
        void SetTileDimensions(int height, int width);
        std::vector<RenderTile> GetTiles() {return mTiles;}
        RenderTile& GetTile(size_t i) {return mTiles[i];}

        void SetX(float x) {SetPosition(Vector2f(x, mPosition.y));}
        void SetY(float y) {SetPosition(Vector2f(mPosition.x, y));}
        void SetXY(float x, float y) {SetPosition(Vector2f(x, y));}
        void SetXY(Vector2f xy) {SetPosition(xy);}
        float GetX() {return mPosition.y;}
        float GetY() {return mPosition.x;}
        void IncreaseWidth(int w) {SetWidth(mWidth + w);}
        void IncreaseHeight(int h) {SetHeight(mHeight + h);}
        void IncreaseSize(int height, int width) {Resize(mHeight + height, mWidth + width);}
        void AddX(float x) {SetPosition(mPosition + Vector2f(x, 0));}
        void AddY(float y) {SetPosition(mPosition + Vector2f(0, y));}
        void AddXY(float x, float y)    {SetPosition(mPosition + Vector2f(x, y));}
        void AddXY(Vector2f xy) {SetPosition(mPosition + xy);}
        void SetAngle(float a) {}
        void AddAngle(float a) {}
        float GetAngle() {return 0;}
        IShapef* GetShape() {return &mHitBox;}
        Rectanglef GetHitRect() {return mHitBox;}
        bool PointInside(Vector2f pt);
        Collision CheckCollision(IShapef* shape);
        Collision CheckCollision(HitBoxf& hitbox);
        void GenerateCollisionBoxes(int size);
    };
}


#endif // BACKGROUNDTILESET_H_INCLUDED

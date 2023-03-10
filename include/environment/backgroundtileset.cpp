#include "backgroundtileset.h"

namespace solstice {
TiledBackground::TiledBackground()
{
    mHeight = 0;
    mWidth = 0;
    mTileHeight = 100;
    mTileWidth = 100;
}

TiledBackground::TiledBackground(BackgroundTileset* tiles, size_t height, size_t width)
{
    Init(tiles, height, width, 100, 100);
}
TiledBackground::TiledBackground(BackgroundTileset* tiles, size_t height, size_t width, size_t tileH, size_t tileW) {
    Init(tiles, height, width, tileH, tileW);
}
void TiledBackground::Init(BackgroundTileset* tiles, size_t height, size_t width, size_t tileH, size_t tileW) {
    mTileset = std::make_shared<BackgroundTileset>(*tiles);
    mTileHeight = tileH;
    mTileWidth = tileW;
    Resize(height, width);
}
void TiledBackground::Resize(size_t h, size_t w) {
    if(h) {
        mHeight = h;
    }
    if(w) {
        mWidth = w;
    }
    Resize();
}

void TiledBackground::SetHeight(size_t h) {
    if(h) {
        mHeight = h;
        Resize();
    }
}

void TiledBackground::SetWidth(size_t w) {
    if(w) {
        mWidth = w;
        Resize();
    }
}
void TiledBackground::Resize() {
    mTiles.resize(mHeight * mWidth);
    if(!mTileset) {

    }
    else if(!mTiles.empty()) {
        for(size_t i = 0; i < mWidth; i++) {
            for(size_t j = 0; j < mHeight; j++) {
                mTiles[i * mHeight + j] = mTileset->GetTile(GetTileType(i, j));
            }
        }
    }
    Reposition();
    mHitBox.SetHW(mHeight * mTileHeight, mWidth * mTileWidth);
}
void TiledBackground::Reposition() {
    if(!mTiles.empty() && mTileset) {
        for(size_t i = 0; i < mWidth; i++) {
            for(size_t j = 0; j < mHeight; j++) {
                mTiles[i * mHeight + j].mTile.SetPosition(mPosition + Vector2f(i * mTileWidth, j * mTileHeight));
            }
        }
    }
    mHitBox.SetPosition(mPosition);
}
void TiledBackground::SetTileDimensions(int height, int width) {
    mTileHeight = height;
    mTileWidth = width;
    Reposition();
}
void TiledBackground::SetPosition(Vector2f pos) {
    Vector2f diff = pos - mPosition;
    mPosition = pos;
    for(size_t i = 0; i < mTiles.size(); i++) {
        mTiles[i].mTile.GetHitBox2().AddXY(diff);
    }
    mHitBox.AddXY(diff);
}
BackgroundTilesetType TiledBackground::GetTileType(size_t x, size_t y) {
    if(!mHeight || !mWidth) {
        return BGTILE_CENTER;
    }
    size_t yMax = mHeight - 1;
    size_t xMax = mWidth - 1;
    if(!x) {
        if(!y) {
            return BGTILE_BOTLEFT;
        }
        else if(y == yMax) {
            return BGTILE_TOPLEFT;
        }
        else {
            return BGTILE_LEFT;
        }
    }
    else if(x == xMax) {
        if(!y) {
            return BGTILE_BOTRIGHT;
        }
        else if(y == yMax) {
            return BGTILE_TOPRIGHT;
        }
        else {
            return BGTILE_RIGHT;
        }
    }
    else {
        if(!y) {
            return BGTILE_BOTTOM;
        }
        else if(y == yMax) {
            return BGTILE_TOP;
        }
        else {
            return BGTILE_CENTER;
        }
    }
}
void TiledBackground::GenerateCollisionBoxes(int size) {
    int xBoxes = mHitBox.GetWidth() / size;
    int yBoxes = mHitBox.GetHeight() / size;
    int numBoxes = xBoxes * 2 + yBoxes * 2 - 4;
    mCollisionBoxes = std::vector<HitBoxf>(numBoxes);
    size_t start = 0;
    for(start; start < xBoxes; start++) {
        Vector2f origin = mPosition + Vector2f(start * size, 0);
        Rectanglef rect(origin, size, size);
        origin.Print();
        mCollisionBoxes[start].AddRect(rect);
    }
    for(start; start < xBoxes * 2; start++) {
        Vector2f origin = mPosition + Vector2f((start - xBoxes) * size, (yBoxes - 1) * size);
        Rectanglef rect(origin, size, size);
        origin.Print();
        mCollisionBoxes[start].AddRect(rect);
    }
    for(start; start < xBoxes * 2 + yBoxes - 2; start++) {
        size_t offset = start - xBoxes * 2;
        Vector2f origin = mPosition + Vector2f(0, size + offset * size);
        Rectanglef rect(origin, size, size);
        mCollisionBoxes[start].AddRect(rect);
    }
    for(start; start < numBoxes; start++) {
        size_t offset = start - xBoxes * 2 + yBoxes - 2;
        Vector2f origin = mPosition + Vector2f((xBoxes - 1) * size, offset * size - size);
        Rectanglef rect(origin, size, size);
        mCollisionBoxes[start].AddRect(rect);
    }
}
void TiledBackground::CreateModel(std::vector<Model>& models) {

}

void TiledBackground::Draw(ShaderProgram& sh, TextureArray& textures) {

}
void TiledBackground::Draw(std::vector<Model>& models, ShaderProgram& sh, TextureArray& textures, glm::mat4 trans, glm::mat4 projection) {
    for(size_t i = 0; i < mTiles.size(); i++) {
        if(mTiles[i].mModelID >= 0) {
            Vector2f pos;
            if(mTiles[i].mTile.GetHBox()->GetType() == SHAPE_RECT) {
                const Rectanglef& rect = *(Rectanglef*)mTiles[i].mTile.GetHBox().get();
                pos = rect.GetTopCenter();
            }
            else {
                const Polygonf& poly = *(Polygonf*)mTiles[i].mTile.GetHBox().get();
                pos = poly.GetAABB().GetCenter();
                //pos = poly.GetPosition();
            }
            pos+=  mTiles[i].mPositionOffset;
            glm::vec3 pos2(pos.x + 640, pos.y + 360, 1.0);
            models[mTiles[i].mModelID].GetTransform().SetTranslation(UnProjectPoint(pos2, trans, projection, glm::vec4(0, 0, 1280, 720), glm::vec2(2.0 / 1280.0, 2.0 / 720.0)));
            glm::vec3 scale = models[mTiles[i].mModelID].GetTransform().GetScale();
            /*
            if(mTiles[i].mFlipsModel) {
                scale.x = -fabs(scale.x);
            }
            else {
                scale.x = fabs(scale.x);
            }
            */
            models[mTiles[i].mModelID].GetTransform().SetScale(scale);
            glm::mat4 translation = models[mTiles[i].mModelID].GetTransform().GetTransform();
            models[mTiles[i].mModelID].Draw(sh, textures, translation);
        }
    }
}
void TiledBackground::DrawTiles(SceneRenderer& rend) {
    Color c = COL_BLUE;
    c.alpha = 0.1;
    for(size_t i = 0; i < mTiles.size(); i++) {
        if(mTiles[i].mTile.GetHBox()) {
            Rectanglef rect = mTiles[i].mTile.GetHitBox2().GetRect(0);
            rend.DrawRectPoly(rect, mTiles[i].mTile.GetHitBox2().mColor);
        }
        //rend.DrawHitBox(mTiles[i]..mTileGetHitBox2().GetRect(0), mTiles[i]..mTileGetHitBox2().mColor);
    }
    for(size_t i = 0; i < mCollisionBoxes.size(); i++) {
        rend.DrawHitBox(mCollisionBoxes[i], Color(1.0, 0.0, 0.0, 0.25));
    }
}
bool TiledBackground::PointInside(Vector2f pt) {
    Rectanglef ptRect(pt, 5, 5);
    return solstice::CheckCollision(&ptRect, &mHitBox);
}

}

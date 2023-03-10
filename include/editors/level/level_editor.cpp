#include "level_editor.h"

namespace solstice {
LevelEditor::LevelEditor() {
    mMouseRect.Init(0, 5, 5, DRAW_CENTER);
    mLeftClickDown = false;
    mShiftHeld = false;
    mQHeld = false;
    mEHeld = false;
    mHeightAdd = 0;
    mWidthAdd = 0;
    mMode = MODE_EDIT;
    mDrawHitboxes = true;
    mSelectedObject = nullptr;
    mSelectionType = SELECTION_NONE;
    mDrawGrid = true;
    mLeftClicked = false;
}
void LevelEditor::CheckInput(SDL_Event& event) {
    switch(event.type) {
        case SDL_KEYUP:
            if(mPlayer && mMode == MODE_PLAYTEST) {
                mPlayer->CheckKeyUp(event.key.keysym.sym);
            }
            if(event.key.keysym.sym == SDLK_LSHIFT) {
                mShiftHeld = false;
            }
            else if(event.key.keysym.sym == SDLK_q) {
                mQHeld = false;
            }
            else if(event.key.keysym.sym == SDLK_e) {
                mEHeld = false;
            }
            break;
        case SDL_KEYDOWN:
            if(mPlayer && mMode == MODE_PLAYTEST) {
                mPlayer->CheckKeyDown(event.key.keysym.sym);
            }
            if(event.key.keysym.sym == SDLK_LSHIFT) {
                mShiftHeld = true;
            }
            else if(event.key.keysym.sym == SDLK_q) {
                mQHeld = true;
            }
            else if(event.key.keysym.sym == SDLK_e) {
                mEHeld = true;
            }
            else if(event.key.keysym.sym == SDLK_1) {
                AddTileFromPalette(0);
            }
            else if(event.key.keysym.sym == SDLK_2) {
                AddTileFromPalette(1);
            }
            else if(event.key.keysym.sym == SDLK_3) {
                AddTileFromPalette(2);
            }
            else if(event.key.keysym.sym == SDLK_4) {
                AddTileFromPalette(3);
            }
            else if(event.key.keysym.sym == SDLK_5) {
                AddTileFromPalette(4);
            }
            else if(event.key.keysym.sym == SDLK_6) {
                AddTileFromPalette(5);
            }
            else if(event.key.keysym.sym == SDLK_7) {
                AddTiledBackground();
            }
            else if(event.key.keysym.sym == SDLK_0) {
                mWaterBodies.push_back(WaterBody(0, mSnapGrid.GetCellSize() * 5, mSnapGrid.GetCellSize() * 5, 0, 0));
            }
            else if(event.key.keysym.sym == SDLK_w) {
                if(mMode == MODE_EDIT || mMode == MODE_MOVE || mMode == MODE_RESIZE) {
                    mHeightAdd = mSnapGrid.GetCellSize();
                }
                else if(mMode == MODE_ROTATE) {
                    mHeightAdd = -15;
                }
            }
            else if(event.key.keysym.sym == SDLK_a) {
                if(mMode == MODE_EDIT || mMode == MODE_MOVE || mMode == MODE_RESIZE) {
                    mWidthAdd = -mSnapGrid.GetCellSize();
                }
                else if(mMode == MODE_ROTATE) {
                    mWidthAdd = 15;
                }
            }
            else if(event.key.keysym.sym == SDLK_s) {
                if(mMode == MODE_EDIT || mMode == MODE_MOVE || mMode == MODE_RESIZE) {
                    mHeightAdd = -mSnapGrid.GetCellSize();
                }
                else if(mMode == MODE_ROTATE) {
                    mHeightAdd = 15;
                }
            }
            else if(event.key.keysym.sym == SDLK_d) {
                if(mMode == MODE_EDIT || mMode == MODE_MOVE || mMode == MODE_RESIZE) {
                    mWidthAdd = mSnapGrid.GetCellSize();
                }
                else if(mMode == MODE_ROTATE) {
                    mWidthAdd = -15;
                }
            }
            else if(event.key.keysym.sym == SDLK_DELETE) {
                if(mSelectedObject) {
                    RemoveTile(mSelectedObjectIndex);
                    mSelectedObject = nullptr;
                }
            }
            else if(event.key.keysym.sym == SDLK_KP_1) {
                if(mMode == MODE_PLAYTEST) {
                    mMode = MODE_EDIT;
                }
                else {
                    mMode = MODE_PLAYTEST;
                    if(mPlayer) {
                        mPlayer->SetPosition(0);
                    }
                    GenerateCollisionBoxes(mSnapGrid.GetCellSize() * 2);
                }
                mSelectedObject = nullptr;
            }
            else if(event.key.keysym.sym == SDLK_KP_2) {
                mDrawHitboxes = !mDrawHitboxes;
            }
            else if(event.key.keysym.sym == SDLK_KP_3) {
                mDrawGrid = !mDrawGrid;
            }
            else if(event.key.keysym.sym == SDLK_F2) {
                if(mMode == MODE_PLAYTEST && mPlayer) {
                    mPlayer->Respawn();
                }
            }
            else if(event.key.keysym.sym == SDLK_KP_7) {
                mMode = MODE_MOVE;
            }
            else if(event.key.keysym.sym == SDLK_KP_8) {
                mMode = MODE_RESIZE;
            }
            else if(event.key.keysym.sym == SDLK_KP_9) {
                mMode = MODE_ROTATE;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_RIGHT) {
                Deselect();
            }
            else if(event.button.button == SDL_BUTTON_LEFT) {
                mLeftClickDown = true;
                mLeftClicked = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT) {
                mLeftClickDown = false;
            }
            break;
        default:
            break;
    }
}
void LevelEditor::DrawGrid(SceneRenderer& rend) {
    if(mDrawGrid) {
        mSnapGrid.DrawGrid(rend);
    }
}
void LevelEditor::Draw(SceneRenderer& rend) {
    if(mMode == MODE_EDIT) {
        DrawGrid(rend);
    }
    DrawHitBoxes(rend);
}
void LevelEditor::Deselect() {
    mSelectedObject = nullptr;
    mSelectedObjectIndex = -1;
    mSelectionType = SELECTION_NONE;
}
void LevelEditor::DrawHitBoxes(SceneRenderer& rend) {
    if(mDrawHitboxes) {
        for(size_t i = 0; i < mTiles.size(); i++) {
            rend.DrawShape(mTiles[i].mTile.GetHBox().get(), mHitBoxColor);
        }
        for(size_t i = 0; i < mTiledBackgrounds.size(); i++) {
            mTiledBackgrounds[i].DrawTiles(rend);
        }
        for(size_t i = 0; i < mWaterBodies.size(); i++) {
            rend.DrawRectPoly(mWaterBodies[i].GetHitBox(), Color(0.0, 0.5, 1.0, 0.5));
        }
        for(size_t i = 0; i < mTiledBackgrounds.size(); i++) {
            mTiledBackgrounds[i].DrawTiles(rend);
        }
        for(size_t i = 0; i < mInteractables.size(); i++) {
            rend.DrawRectPoly(mInteractables[i]->mHitBox, mHitBoxColor);
        }
        if(mSelectedObject) {
            rend.DrawShape(mSelectedObject->GetShape(), mHitBoxColor);
        }
    }
    if(mPlayer && mMode == MODE_PLAYTEST) {
        rend.DrawRectPoly(mPlayer->GetHitBox(), COL_BLUE);
    }
}

void LevelEditor::Update(double dt, Vector2f mousePosition) {
    mToolBar.Update();
    mLastMousePos = mMousePos;
    mMousePos = mousePosition;
    mMouseRect.SetPosition(mMousePos);
    switch(mMode) {
        case MODE_PLAYTEST:
            UpdatePlayer(dt);
            break;
        case MODE_EDIT:
        default:
            UpdateEditMode(dt);
            break;
    }
    mWidthAdd = 0;
    mHeightAdd = 0;
    mLeftClicked = false;
}
void LevelEditor::UpdatePlayer(double dt) {
    if(mPlayer && mMode == MODE_PLAYTEST) {
        mPlayer->Update(dt);
        mPlayer->SetGrounded(false);
        for(size_t i = 0; i < mWaterBodies.size(); i++) {
            mPlayer->CheckCollision(mWaterBodies[i]);
        }
        for(size_t i = 0; i < mTiles.size(); i++) {
            mPlayer->CheckAndResolveCollision(mTiles[i].mTile);
        }
        for(size_t i = 0; i < mTiledBackgrounds.size(); i++) {
            //for(size_t i = 0; i < mTiledBackgrounds[i].)
            //mPlayer->CheckAndResolveCollision(mTiles[i].mTile);
        }
    }
    for(size_t i = 0; i < mTiles.size(); i++) {
        mTiles[i].mTile.Update(dt);
    }
}
void LevelEditor::UpdateTiles(double dt) {
    if(mMode == MODE_EDIT || mMode == MODE_MOVE) {
        Move();
    }
    if(mMode == MODE_EDIT || mMode == MODE_RESIZE) {
        Resize();
    }
    if(mMode == MODE_ROTATE) {
        Rotate();
    }
}
void LevelEditor::UpdateWater(double dt) {
    if(mMode == MODE_EDIT || mMode == MODE_MOVE) {
        Move();
    }
}
void LevelEditor::UpdateEnemies(double dt) {
}
void LevelEditor::UpdateObjects(double dt) {
}
void LevelEditor::Snap(IShapef* shape) {
    if(mShiftHeld) {
        if(mQHeld) {
            mSnapGrid.SnapX(mSelectedObject);
        }
        else if(mEHeld) {
            mSnapGrid.SnapY(mSelectedObject);
        }
        else {
            mSnapGrid.Snap(mSelectedObject);
        }
    }
}
void LevelEditor::UpdateEditMode(double dt) {
    bool found = false;
    if(mLeftClickDown) {
        if(!HasSelection()) {
            for(size_t i = 0; i < mTiles.size() && !found; i++) {
                if(CheckCollision(&mMouseRect, mTiles[i].mTile.GetHBox().get())) {
                    mSelectedObject = &mTiles[i];
                    mSelectedObjectIndex = i;
                    mSelectionType = SELECTION_TILE;
                    found = true;
                }
            }
            for(size_t i = 0; i < mWaterBodies.size() && !found; i++) {
                if(CheckCollision(&mMouseRect, &mWaterBodies[i].GetHitBox())) {
                    mSelectedObject = &mWaterBodies[i];
                    mSelectedObjectIndex = i;
                    mSelectionType = SELECTION_WATER;
                    found = true;
                }
            }
            for(size_t i = 0; i < mTiledBackgrounds.size() && !found; i++) {
                if(mTiledBackgrounds[i].PointInside(mMousePos)) {
                    mSelectedObject = &mTiledBackgrounds[i];
                    mSelectedObjectIndex = i;
                    mSelectionType = SELECTION_TILESET;
                    found = true;
                }
            }
            for(size_t i = 0; i < mInteractables.size() && !found; i++) {
                if(CheckCollision(&mMouseRect, &mInteractables[i]->mHitBox)) {
                    mSelectedObject = mInteractables[i].get();
                    mSelectedObjectIndex = i;
                    mSelectionType = SELECTION_OBJECT;
                    found = true;
                }
            }
        }
    }
    if(!found && !HasSelection()) {
        mSelectionType = SELECTION_NONE;
    }
    if(mMode == MODE_EDIT || mMode == MODE_MOVE) {
        Move();
    }
    if(mMode == MODE_EDIT || mMode == MODE_RESIZE) {
        Resize();
    }
    if(mMode == MODE_ROTATE) {
        Rotate();
    }
    switch(mSelectionType) {
        /*
        case SELECTION_TILE:
            UpdateTiles(dt);
            break;
        case SELECTION_ENEMY:
            UpdateEnemies(dt);
            break;
        case SELECTION_OBJECT:
            UpdateObjects(dt);
            break;
        case SELECTION_WATER:
            UpdateWater(dt);
            break;
        case SELECTION_NONE:
        default:
            break;
        */
    }
}
void LevelEditor::Resize() {
    if(mSelectedObject) {
        if(mSelectionType == SELECTION_TILESET) {
            mWidthAdd/= mSnapGrid.GetCellSize();
            mHeightAdd/= mSnapGrid.GetCellSize();
        }
        mSelectedObject->IncreaseSize(mHeightAdd, mWidthAdd);
    }
    mWidthAdd = 0;
    mHeightAdd = 0;
}
void LevelEditor::Rotate() {
    if(mSelectedObject) {
        Vector2f diff = mMousePos - mLastMousePos;
        float angle = mSelectedObject->GetAngle();
        if(mLeftClickDown) {
            angle+= diff.Length() * RADIAN;
        }
        angle-= mWidthAdd * RADIAN - mHeightAdd * RADIAN;
        mSelectedObject->SetAngle(angle);
    }
}
void LevelEditor::Move() {
    if(mSelectedObject) {
        Vector2f pos;
        if(mLeftClickDown) {
            if(!mShiftHeld) {
                pos = mSelectedObject->GetPosition() + mMousePos - mLastMousePos;
            }
            else {
                pos = mMousePos;
            }
        }
        else {
            pos = mSelectedObject->GetPosition();
        }
        if(mMode == MODE_MOVE) {
            pos.x+= mWidthAdd;
            pos.y+= mHeightAdd;
        }
        mSelectedObject->SetXY(pos);
        Snap(mSelectedObject->GetShape());
    }
}
bool LevelEditor::RemoveTile(size_t i) {
    if(mSelectionType == SELECTION_TILE) {
        mTiles.erase(mTiles.begin() + i);
        if(i == mSelectedObjectIndex) {
            mSelectedObject = nullptr;
            mSelectedObjectIndex = -1;
        }
        else if(mSelectedObject) {
            mSelectedObject = &mTiles[i];
        }
    }
    else if(mSelectionType == SELECTION_WATER) {
        mWaterBodies.erase(mWaterBodies.begin() + i);
        if(i == mSelectedObjectIndex) {
            mSelectedObject = nullptr;
            mSelectedObjectIndex = -1;
        }
        else if(mSelectedObject) {
            mSelectedObject = &mWaterBodies[i];
        }
    }
}

void LevelEditor::AddTile(std::shared_ptr<IShapef> tile) {
    mTiles.push_back(RenderTile(tile));
    if(mSelectedObject) {
        mSelectedObject = &mTiles[mSelectedObjectIndex];
    }
}
void LevelEditor::AddTileFromPalette(size_t i) {
    mTiles.push_back(mTilePalette[i]);
    if(mSelectedObject) {
        mSelectedObject = &mTiles[mSelectedObjectIndex];
    }
}
void LevelEditor::AddTiledBackground() {
    mTiledBackgrounds.push_back(TiledBackground(&mTileSet, 3, 3, mSnapGrid.GetCellSize(), mSnapGrid.GetCellSize()));
}
void LevelEditor::AddInteractable(std::shared_ptr<Interactable> interactable) {
    mInteractables.push_back(interactable);
}
void LevelEditor::AddIntercatableFromPallette(size_t i) {
    mInteractables.push_back(mInteractablePalette[i]);
}
void LevelEditor::SetColors(Color hitbox, Color highlight, Color hover) {
    mHitBoxColor = hitbox;
    mHighlightColor = highlight;
    mHoverColor = hover;
}
void LevelEditor::DrawModels(ShaderProgram& sh, TextureArray& textures, std::vector<Model>& models, glm::mat4& trans, glm::mat4& projection) {
    for(size_t i = 0; i < mTiles.size(); i++) {
        Vector2f pos;
        if(mTiles[i].mTile.GetHBox()->GetType() == SHAPE_RECT) {
            const Rectanglef& rect = *(Rectanglef*)mTiles[i].mTile.GetHBox().get();
            pos = rect.GetTopCenter();
        }
        else {
            const Polygonf& poly = *(Polygonf*)mTiles[i].mTile.GetHBox().get();
            pos = poly.GetAABB().GetCenter();
        }
        pos+=  mTiles[i].mPositionOffset;
        mTiles[i].Draw(sh, textures, models, pos);
    }
}
void LevelEditor::OnNotify(const GUIEntity& entity, SolsticeGUIEvent event) {
    if(entity.IsType(GUI_ENTITY_BUTTON)) {

    }
}
void LevelEditor::GenerateCollisionBoxes(int size) {
    for(size_t i = 0; i < mTiledBackgrounds.size(); i++) {
        mTiledBackgrounds[i].GenerateCollisionBoxes(size);
    }
}
string LevelEditor::GetModeString() {
    switch(mMode) {
    case MODE_EDIT:
        return "Edit Mode";
    case MODE_PLAYTEST:
        return "Playtest Mode";
    case MODE_MOVE:
        return "Move Mode";
    case MODE_RESIZE:
        return "Resize Mode";
    case MODE_ROTATE:
        return "Rotate Mode";
    case MODE_TILE:
        return "Tile Mode";
    }
}
}

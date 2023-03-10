#ifndef LEVEL_EDITOR_H_INCLUDED
#define LEVEL_EDITOR_H_INCLUDED

#include <memory>
#include <map>
#include "environment/gamelevel.h"
#include "editors/level/editor_snapgrid.h"
#include "player/baseplayer2.h"
#include "geometry/shapecreate.h"
#include "environment/rendertile.h"
#include "environment/backgroundtileset.h"
#include "graphics/3d/model.h"
#include "gui/toolbar.h"
#include "events/eventobserver.h"
#include "interactables/interactable.h"

namespace solstice {
enum LevelEditorMode {MODE_EDIT, MODE_PLAYTEST, MODE_MOVE, MODE_TILE, MODE_ROTATE, MODE_RESIZE};
enum EditorSelectionType {SELECTION_NONE, SELECTION_TILE, SELECTION_TILESET, SELECTION_OBJECT, SELECTION_ENEMY, SELECTION_WATER};
enum LevelEditorButton {
                LE_BUTTON_PLAYER, LE_BUTTON_CHECKPOINT, LE_BUTTON_SAVE, LE_BUTTON_LOAD,
                LE_BUTTON_PLAYTEST, LE_BUTTON_STOPPLAY, LE_BUTTON_UNDO, LE_BUTTON_REDO,
                LE_BUTTON_MOVE, LE_BUTTON_SCALE, LE_BUTTON_TILE, LE_BUTTON_ROTATE,
                LE_BUTTON_ADD_TILE, LE_BUTTON_ADD_PLATFORM, LE_BUTTON_ADD_SLOPE, LE_BUTTON_ADD_WATER,
                LE_BUTTON_ADD_ENEMY, LE_BUTTON_ADD_PROP, LE_BUTTON_ADD_DOOR, LE_BUTTON_ADD_BOX,
                LE_BUTTON_ADD_PLANE, LE_BUTTON_ADD_WETSPOT, LE_BUTTON_ADD_ROOM};
class LevelEditor : public GUIEventObserver {
private:
    std::shared_ptr<GameLevel> mLevel;
    int mSelectedObjectIndex;
    std::shared_ptr<LevelRoom> mCurrentRoom;
    int mMode;
    Rectanglef mMouseRect;
    Vector2f mLastMousePos, mMousePos;
    bool mLeftClickDown, mShiftHeld, mQHeld, mEHeld, mLeftClicked;
    int mSelectionType;
    int mHeightAdd, mWidthAdd;
    void UpdatePlayer(double dt);
    void UpdateEditMode(double dt);
    void UpdateTiles(double dt);
    void UpdateWater(double dt);
    void UpdateEnemies(double dt);
    void UpdateObjects(double dt);
    void Resize();
    void Rotate();
    void Move();
    void Reselect();
    void Snap(IShapef* shape);
    ResizableObject* mSelectedObject;
    std::vector<std::vector<RenderTile> > mUndoList;
    std::vector<std::vector<RenderTile> > mRedoList;
    std::vector<TiledBackground> mTiledBackgrounds;
    std::vector<std::shared_ptr<Interactable> > mInteractables;
public:
    LevelEditor();
    std::vector<RenderTile> mTilePalette;
    std::vector<std::shared_ptr<Interactable> > mInteractablePalette;
    std::vector<RenderTile> mTiles;
    std::vector<WaterBody> mWaterBodies;
    BackgroundTileset mTileSet;
    std::map<int, bool> mButtonPairs;
    bool mDrawHitboxes, mDrawGrid;
    SnapGrid mSnapGrid;
    Color mHitBoxColor, mHighlightColor, mHoverColor;
    std::shared_ptr<solstice::BasePlayer2> mPlayer;
    ToolBar mToolBar;
    void DrawGrid(SceneRenderer& rend);
    void Draw(SceneRenderer& rend);
    void DrawHitBoxes(SceneRenderer& rend);
    void Update(double dt, Vector2f mousePosition);
    void CheckInput(SDL_Event& event);
    void ChangeMode(int m)          {mMode = m;}
    bool IsMode(int m)              {return mMode = m;}
    bool RemoveTile(size_t i);
    void AddTile(std::shared_ptr<IShapef> tile);
    void SetColors(Color hitbox, Color highlight, Color hover);
    void AddTileFromPalette(size_t i);
    void AddInteractable(std::shared_ptr<Interactable> interactable);
    void AddIntercatableFromPallette(size_t i);
    void AddTiledBackground();
    void DrawModels(ShaderProgram& sh, TextureArray& textures, std::vector<Model>& models, glm::mat4& trans, glm::mat4& projection);
    void SaveToRoom(int roomID = -1);
    bool HasSelection() {return mSelectedObject;}
    void Deselect();
    void OnNotify(const GUIEntity& entity, SolsticeGUIEvent event);
    void GenerateCollisionBoxes(int size);
    string GetModeString();
};

}


#endif // LEVEL_EDITOR_H_INCLUDED

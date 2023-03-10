#ifndef WATERSHED_H_INCLUDED
#define WATERSHED_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <functional>
#include "GL/glew.h"
#include "SDL2/sdl.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "GL/glu.h"

#include "globals.h"
#include "gui/window.h"
#include "graphics/graphics.h"
#include "text/sol_text.h"
#include "text/sol_font.h"
#include "animation/frame.h"
#include "animation/animation.h"

#include "particle/particle.h"
#include "particle/particlesprayer.h"
#include "particle/particlerenderer.h"

#include "geometry/polygon.h"
#include "geometry/spatialgrid.h"
#include "environment/backgroundtile.h"
#include "environment/room.h"

#include "physics/physics.h"

#include "player/baseplayer2.h"
#include "player/playerwatershed.h"
#include "player/camera2d.h"

#include "gui/guicomponent.h"


namespace solstice {
enum WatershedEngineShaders{WENGINE_3D = 0, WENGINE_BONE, WENGINE_2D, WENGINE_2D_POLY, WENGINE_WATER, WENGINE_3D_OUTLINE, WENGINE_BONE_OUTLINE};
struct WatershedEngineState {
};
class WatershedEngine {
protected:
    WatershedPlayer mIona;
    WatershedEngineState mCurrentState, mInitialState;
    ParticleRenderer mOpaquePartRend, mAddPartRend;
    SceneRenderer mSceneRenderer, mHudRenderer, mHitBoxRenderer;
    std::vector<GUIComponent*> mGUIComponents;
    std::vector<LevelRoom> mRooms;
    int mCurrentRoomID;
    LevelRoom* mCurrentRoom;
    Camera mCamera;
    std::vector<ShaderProgram> mShaders;
    TextureArray mTextures;
    Texture mTextures2D;
    std::vector<Model> mModels;
    float mScreenWidth, mScreenHeight;
    float mNorm;
    float mScale, mViewAngle, mZoom;
    glm::mat4 mProjectionMatrix;
    ModelTransform mViewTransform;
    Window mWindow;
    SpatialGrid mBoxGrid;
    Vector2f mUpdateRange;
    double mDt, mTimeScale;
    bool mShiftHeld;
    Uint32 mLastTime, mCurrentTime, mFramesPerSecond, mAnimLastUpdate;
    Uint32 mCurFPS, mStartFPS, mFrameCounter;
    bool mPaused, mFinished;
    glm::mat4 mView;
    bool mDrawOutlines, mCameraLock, mShowHitboxes;
    float mGroundedCamOffset;
    float mGlobalGravity;
    float mOutlineSize;
    GLint mTextureUniforms[20];
    Light mWaterLight;
    SDL_GameController* mController;
    void PopulateGrids();
    void CheckCollisions();
    void UpdateEngine();
    void UpdateMenus();
    TimerEntity mPauseTimer;
public:
    Texture mParticleTextures;
    WatershedEngine(float height = 720, float width = 1280);

    WatershedPlayer& GetIona()                  {return mIona;}
    std::vector<LevelRoom>& GetRooms()          {return mRooms;}
    LevelRoom& GetRoom(size_t i)                {return mRooms[i];}
    LevelRoom* GetRoomPtr(size_t i)             {return &mRooms[i];}
    Camera& GetCamera()                         {return mCamera;}
    std::vector<ShaderProgram>& GetShaders()    {return mShaders;}
    ShaderProgram& GetShader(size_t i)          {return mShaders[i];}
    TextureArray& GetTextures()                 {return mTextures;}
    Texture& GetTextures2D()                    {return mTextures2D;}
    std::vector<Model>& GetModels()             {return mModels;}
    Model* GetModelPtr(std::string m)           {return &GetModel(m);}
    Window& GetWindow()                         {return mWindow;}
    SpatialGrid& GetBoxGrid()                   {return mBoxGrid;}
    Vector2f GetUpdateRange()                   {return mUpdateRange;}
    LevelRoom* GetCurrentRoom()                 {return mCurrentRoom;}
    Light& GetWaterLight()                      {return mWaterLight;}
    Light* GetWaterLightPtr()                   {return &mWaterLight;}
    float& GetGlobalGravity()                   {return mGlobalGravity;}
    SceneRenderer& GetSceneRenderer()           {return mSceneRenderer;}
    ModelTransform& GetCameraOffset()           {return mViewTransform;}
    Model* GetLastModel()                       {return &mModels.back();}
    void SetHowHitboxes(bool s)                 {mShowHitboxes = s;}
    bool GetShowHitboxes()                      {return mShowHitboxes;}
    void SetCameraStepSize(float step)          {mCamera.stepSize = step;}
    void Set2DTextureBase(int width, int height, int depth);
    void SetupModels();
    ParticleRenderer& GetOpaquePartRend()       {return mOpaquePartRend;}
    ParticleRenderer& GetAddPartRend()          {return mAddPartRend;}

    void AddRoom(LevelRoom r)                   {mRooms.push_back(r);}
    void AddModel(Model m)                      {mModels.push_back(m);}
    void AddShader(ShaderProgram p)             {mShaders.push_back(p);}
    void LoadTexture(std::string t, std::string ref = "")            {mTextures.LoadImg(t, ref);}
    void LoadTexture2D(std::string t, std::string ref ="")            {mTextures2D.LoadToArray(t.c_str(), ref.c_str());}
    void LoadShader(std::string vt, std::string fg);
    void LoadRoom(std::string room);

    int GetRoomID(std::string name);
    int GetModelID(std::string name);
    int GetTextureID(std::string name)                  {return mTextures.GetTextureID(name);}
    int GetTextureID2D(std::string name)                {return mTextures2D.GetArrayID(name);}
    LevelRoom& GetRoom(std::string name);
    Model& GetModel(std::string name);

    void InitScreen(float height, float width, float viewAngle, float near, float far);
    size_t GetScreenHeight()                    {return mScreenHeight;}
    size_t GetScreenWidth()                     {return mScreenWidth;}
    float GetViewAngle()                        {return mViewAngle;}
    void SetScale(float z)                      {mScale = z; mSceneRenderer.GetTransform().SetScale(glm::vec3(z));}
    void SetViewScale(float x, float y, float z) {mViewTransform.SetScale(x, y, z);}
    float GetScale()                            {return mScale;}
    void SetZoom(float z)                       {mZoom = z;}
    float GetZoom()                             {return mZoom;}
    glm::mat4 GetProjectionMatrix()             {return mProjectionMatrix;}
    float GetNorm()                             {return mNorm;}
    size_t GetFrameCounter()                    {return mFrameCounter;}
    float GetGroundedCamOffset()                {return mGroundedCamOffset;}
    bool GetDrawOutlines()                      {return mDrawOutlines;}
    void SetDrawOutlines(bool o)                {mDrawOutlines = o;}
    void SetOutlineSize(float o);
    void SetGroundedCamOffset(float o)          {mGroundedCamOffset = o;}
    void DrawParticles(ShaderProgram& sh);
    void BufferParticleRenderers();
    void ClearParticleRenderers();
    glm::vec3 GetCameraPos();
    glm::mat4 GetViewTransform()                {return mViewTransform.GetTransform();}
    glm::vec3 GetIonaWorldPos(float xScale = 1.0, float yScale = 1.0);

    void LoadDefaultShaders();
    void SetFramesPerSecond(Uint32 i);
    void SetPaused(bool p)                      {mPaused = p;}
    Uint32 GetFramesPerSecond()                 {return mFramesPerSecond;}
    float GetPaused()                           {return mPaused;}
    double GetDt()                              {return mDt;}
    double GetLastTime()                        {return mLastTime;}
    double GetCurrentTime()                     {return mCurrentTime;}
    bool Finished()                             {return mFinished;}
    void UseShader(size_t i)                    {mShaders[i].UseProgram();}
    float GetTimeScale()                        {return mTimeScale;}
    void ResetTimeScale()                       {mTimeScale = 1.0;}
    void SetShowHitBoxes(bool hb)               {mShowHitboxes = true;}
    void Pause(float dur)                       {mPauseTimer.mDuration = dur; mPauseTimer.Reset();}
    void Unpause()                              {mPauseTimer.Finish();}

    void ChangeRoom(size_t i);

    void Reset();
    void Start();
    bool Update();
    bool Update(SDL_Event& event);
    void UpdateEvents(SDL_Event& event);
    void Draw();
    void DrawModels();
    void DrawBackgrounds();
    void DrawOutlines();
    void DrawWater();
    void DrawHitBoxes();
    void DrawSprites();
    void DrawHitBox(Rectanglef rect);
    void DrawHUDOBject(GUIComponent* obj);
    void DrawQuad(QuadData& q);
    void AddGUICOmponent(GUIComponent* component);

    void ClearModels();
    void ClearTextures();
};

}


#endif // WATERSHED_H_INCLUDED

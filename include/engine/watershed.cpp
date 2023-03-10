#include "watershed.h"

namespace solstice {
WatershedEngine::WatershedEngine(float height, float width):
mSceneRenderer(height, width, 1000), mHudRenderer(height, width, 1000), mHitBoxRenderer(height, width, 1000)
{
    mScreenHeight = height;
    mScreenWidth = width;
    mCurrentRoom = NULL;
    mCurrentRoomID = -1;
    InitScreen(height, width, 30.0f, 0.1f, 100.0f);
    SetFramesPerSecond(60);
    mPaused = false;
    mFinished = false;
    mScale = 1.0f;
    mZoom = -3.5f;
    mViewAngle = 30.0f;
    mCurFPS = 0;
    mStartFPS = 0;
    mFrameCounter = 0;
    mDrawOutlines = false;
    mCameraLock = false;
    mShowHitboxes = false;
    mGroundedCamOffset = 100;
    mOutlineSize = 0.1f;
    mTimeScale = 1.0;
    mShiftHeld = false;
    for(GLint i = 0; i < 20; i++) {
        mTextureUniforms[i] = i;
    }
    mController = SDL_GameControllerOpen(0);
    mPauseTimer.Finish();
}
void WatershedEngine::InitScreen(float height, float width, float viewAngle, float near, float far) {
    mScreenHeight = height;
    mScreenWidth = width;
    float aspectRatio = width / height;
    mNorm = 2.0 / width;
    mProjectionMatrix = glm::perspective(glm::radians(viewAngle), aspectRatio, near, far);
    for(auto& shader : mShaders) {
        shader.UseProgram();
        shader.SetUniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    }
    mSceneRenderer.SetResolution(Vector2f(width, height));
    mSceneRenderer.SetProjection(mProjectionMatrix);
    glUseProgram(0);
}
void WatershedEngine::LoadDefaultShaders() {
    mShaders.clear();
    mShaders.push_back(ShaderProgram("shaders/vertex_3d.glsl",
                                     "shaders/frag_cel.glsl"));
    mShaders.push_back(ShaderProgram("shaders/vertex_3d_bone.glsl",
                                     "shaders/frag_cel.glsl"));
    mShaders.push_back(ShaderProgram("shaders/vt_2d.glsl",
                                     "shaders/fg_2d.glsl"));
    mShaders.push_back(ShaderProgram("shaders\\vt_2d_old.glsl",
                                     "shaders\\fg_2d.glsl"));
    mShaders.push_back(ShaderProgram("shaders/water_vt_light.glsl",
                                     "shaders/water_frag_light.glsl"));
    mShaders.push_back(ShaderProgram("shaders/vt_outline.glsl",
                                     "shaders/frag_outline.glsl"));
    mShaders.push_back(ShaderProgram("shaders/vt_outline_bones.glsl",
                                     "shaders/frag_outline.glsl"));
    for(auto& shader : mShaders) {
        shader.UseProgram();
        shader.SetUniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    }
    glUseProgram(0);
}
void WatershedEngine::ChangeRoom(size_t i) {
    if(i < mRooms.size()) {
        mCurrentRoom = &mRooms[i];
        mCurrentRoomID = i;
        mCurrentRoom->BindCamera(mCamera);
    }
    else {
        mCurrentRoom = NULL;
        mCurrentRoomID = -1;
    }

}
void WatershedEngine::SetFramesPerSecond(Uint32 i) {
    mDt = 1.0 / (double)i;
    mFramesPerSecond = i;
}
void WatershedEngine::PopulateGrids() {
    mBoxGrid.ResetGrid();
    mBoxGrid.PopulateNodes(&mIona);
    if(mCurrentRoom) {
        mBoxGrid.PopulateNodes(mCurrentRoom->GetBodyPointers());
    }
}
void WatershedEngine::CheckCollisions() {
    mIona.SetGrounded(false);
    mIona.SetTouchingWall(false);
    mBoxGrid.CheckCollisions();
    if(mCurrentRoom) {
        mCurrentRoom->CheckCollisions(&mIona);
        mCurrentRoom->CheckSelfCollisions();
        mIona.CheckBulletCollisions(*mCurrentRoom);
    }
}
void WatershedEngine::Reset() {
    mCurrentState = mInitialState;
}
void WatershedEngine::Start() {
    mCurrentTime = SDL_GetTicks();
    mAnimLastUpdate = SDL_GetTicks();
    mStartFPS = SDL_GetTicks();
    if(mCurrentRoom == NULL) {
        ChangeRoom(0);
    }
    SetOutlineSize(mOutlineSize);
    Update();
}
void WatershedEngine::SetOutlineSize(float o) {
    mOutlineSize = o;
    for(auto& model : mModels) {
        model.SetOutlineSize(mOutlineSize);
    }
    mIona.GetModel().SetOutlineSize(mOutlineSize);
}
bool WatershedEngine::Update(SDL_Event& event) {
    UpdateEvents(event);
    return Update();
}
bool WatershedEngine::Update() {
    if(mCurrentRoomID > -1) {
        mCurrentRoom = &mRooms[mCurrentRoomID];
    }
    Uint32 newTime = SDL_GetTicks();
    Uint32 deltaTime = newTime - mCurrentTime;
    double frameTime = deltaTime * 0.001;
    bool updated = false;
    if(frameTime >= (mDt - 0.0001) && !mPaused) {
        ClearParticleRenderers();
        mSceneRenderer.ClearBuffers();
        updated = true;
        int deltaFPS = newTime - mStartFPS;
        mCurFPS++;
        if(deltaFPS >= 1000) {
            mFrameCounter = mCurFPS / (deltaFPS / 1000);
            mStartFPS = SDL_GetTicks();
            mCurFPS = 0;
        }
        mCurrentTime = newTime;
        mIona.Update(mDt * mTimeScale, mCurrentRoom->GetBodyPointers());
        if(mCurrentRoom) {
            mCurrentRoom->UpdateBoxes(mDt * mTimeScale);
        }
        PopulateGrids();
        CheckCollisions();
        if(mIona.GetSwimming()) {
            mCamera.yOffset = 0;
            mCurrentRoom->BindWaterCamera(mCamera);
        }
        else {
            mCamera.yOffset = mGroundedCamOffset;
            mCurrentRoom->BindCamera(mCamera);
        }
        mCurrentRoom->Update(0.5f);
        for(auto& chain : mCurrentRoom->GetChains()) {
            if(chain.GetAttachedObject() == mIona.GetHeldObject() && !chain.CheckInbounds(chain.GetAttachedObject())) {
               mIona.DropHeldObject(true);
            }
        }
        if(!mCameraLock) {
            mCamera.MoveTo(0, mIona.GetHitBox().GetCenter());
        }
        DrawHitBoxes();
        DrawSprites();
    }
    frameTime = newTime - mAnimLastUpdate;
    mIona.UpdateModel(frameTime * 0.001 * mTimeScale, mScale * mNorm);
    mAnimLastUpdate = newTime;
    return updated;
}
void WatershedEngine::UpdateEngine() {
}
void WatershedEngine::UpdateMenus() {
}
void WatershedEngine::ClearModels() {
    for(auto& model : mModels) {
        model.clear();
    }
    mModels.clear();
}
void WatershedEngine::ClearTextures() {
    mTextures.clear();
    mTextures2D.Clean();
}
void WatershedEngine::UpdateEvents(SDL_Event& event) {
    while(SDL_PollEvent(&event)) {
        mIona.UpdateKeys(event, mController);
        switch(event.type) {
            case SDL_QUIT:
                mFinished = true;
                break;
            case SDL_KEYUP:
                //mIona.CheckKeyUp(event.key.keysym.sym);
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    mFinished = true;
                }
                if(event.key.keysym.sym == SDLK_F2) {
                    mIona.Respawn();
                }
                if(event.key.keysym.sym == SDLK_RETURN) {
                    mPaused = !mPaused;
                }
                break;
            case SDL_KEYDOWN:
                //mIona.CheckKeyDown(event.key.keysym.sym);
                break;
            default:
                break;
        }
    }
}
void WatershedEngine::LoadShader(std::string vt, std::string fg) {
    mShaders.push_back(ShaderProgram(vt, fg));
}
void WatershedEngine::LoadRoom(std::string room) {
    LevelRoom r;
    r.Read(room);
    mRooms.push_back(r);
}
int WatershedEngine::GetRoomID(std::string name) {
    for(size_t i = 0; i < mRooms.size(); i++) {
        if(mRooms[i].GetName() == name) {
            return i;
        }
    }
    return -1;
}
int WatershedEngine::GetModelID(std::string name) {
    for(size_t i = 0; i < mModels.size(); i++) {
        if(mModels[i].GetName() == name) {
            return i;
        }
    }
    return -1;
}
LevelRoom& WatershedEngine::GetRoom(std::string name) {
    int i = GetRoomID(name);
    if(i >= 0) {
        return mRooms[i];
    }
    return mRooms.front();
}
Model& WatershedEngine::GetModel(std::string name) {
    int i = GetModelID(name);
    if(i >= 0) {
        return mModels[i];
    }
    return mModels.front();
}
void WatershedEngine::Draw() {
    mViewTransform.SetTranslation(GetCameraPos());
    mView = mViewTransform.GetTransform();
    DrawModels();
    if(mDrawOutlines) {
        DrawOutlines();
    }
    mParticleTextures.Bind();
    if(mCurrentRoom) {
        mCurrentRoom->Draw2DGraphics(mSceneRenderer);
    }
    mSceneRenderer.Buffer();

    glDisable(GL_DEPTH_TEST);
    mShaders[WENGINE_2D].UseProgram();
    mShaders[WENGINE_2D].SetUniform1i("textures", 0);
    mSceneRenderer.Render(mShaders[WENGINE_2D]);

    mShaders[WENGINE_2D_POLY].UseProgram();
    mShaders[WENGINE_2D_POLY].SetUniform1i("textures", 0);
    mSceneRenderer.RenderPolies(mShaders[WENGINE_2D_POLY]);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);

    DrawWater();
}
glm::vec3 WatershedEngine::GetCameraPos() {
    return glm::vec3(mCamera.position.x * mNorm * mScale, mCamera.position.y * mNorm * mScale, mZoom);
}
glm::vec3 WatershedEngine::GetIonaWorldPos(float xScale, float yScale) {
    return mIona.GetWorldPos();
}
void WatershedEngine::Set2DTextureBase(int width, int height, int depth) {
    mTextures2D.CreateArrayTexture(width, height, depth);
}
void WatershedEngine::DrawModels() {
    mShaders[WENGINE_3D].UseProgram();
    mShaders[WENGINE_3D].SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    mShaders[WENGINE_3D].SetUniform1i("tex", 0);
    if(mCurrentRoom) {
        mShaders[WENGINE_3D].SetUniformLights("lights", mCurrentRoom->GetLights());
        mShaders[WENGINE_3D].SetUniform1i("numLights", mCurrentRoom->GetLights().size());
    }
    if(mCurrentRoom) {
        int modelID = mCurrentRoom->GetModelID();
        if(modelID >= 0 && modelID < mModels.size()) {
            Model& m = mModels[modelID];
            m.Draw(mShaders[WENGINE_3D], mTextures);
        }
        if(mDrawOutlines) {
            glEnable(GL_STENCIL_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }
        if(!mCurrentRoom->GetBoxes().empty()) {
            modelID = mCurrentRoom->GetBoxes().back().mModelID;
            if(modelID >= 0 && modelID < mModels.size()) {
                Model& m = mModels[modelID];
                float baseZ = m.GetTransform().GetTranslation().z;
                ModelTransform transform;
                for(auto& box : mCurrentRoom->GetBoxes()) {
                    float z = baseZ + box.GetZOffset();
                    m.SetTranslation(glm::vec3(box.GetPosition().x * mScale * mNorm,
                                               box.GetPosition().y * mScale * mNorm,
                                               z));
                    m.SetRotation(0.0f, 0.0f, box.GetHitBox().GetAngle());
                    m.Draw(mShaders[WENGINE_3D], mTextures);
                }
                m.SetTranslation(glm::vec3(0, 0, baseZ));
            }

            modelID = mCurrentRoom->GetBarrels().back()->mModelID;
            if(modelID >= 0 && modelID < mModels.size()) {
                Model& m = mModels[modelID];
                float baseZ = m.GetTransform().GetTranslation().z;
                glm::vec3 rot = m.GetTransform().GetRotation();
                ModelTransform transform;
                for(auto& barrel : mCurrentRoom->GetBarrels()) {
                    float z = baseZ + barrel->GetZOffset();
                    m.SetTranslation(glm::vec3((barrel->GetPosition().x + barrel->mModelOffset.x) * mScale * mNorm,
                                               (barrel->GetPosition().y + barrel->mModelOffset.y) * mScale * mNorm,
                                               z));
                    float xAngle = 0;
                    if(barrel->GetKnockedOver()) {
                        //xAngle = PI * 0.5;
                    }
                    m.SetRotation(0.0, xAngle, barrel->GetHitBox().GetAngle());
                    m.Draw(mShaders[WENGINE_3D], mTextures);
                }
                m.SetTranslation(glm::vec3(0, 0, baseZ));
                m.SetRotation(rot);
            }
        }
        for(auto platform : mCurrentRoom->GetGroundTiles()) {
            if(platform.GetType() == TILE_PLATFORM) {
                Vector2f pos = platform.GetHitBox().GetCenter();
                //std::cout << mTextures[mModels[1].GetMesh(0).GetTextureID()].GetNameRef() << std::endl;
                //std::cout << mModels[1].GetMesh(0).GetTextureID()<< std::endl;
                //mModels[1].SetTextureID(0);
                mModels[1].SetTranslation(glm::vec3(pos.x * mScale * mNorm,
                                                    pos.y * mScale * mNorm - 0.1f,
                                                    -1.0f));
                mModels[1].Draw(mShaders[WENGINE_3D], mTextures);
            }
        }
    }
    mShaders[WENGINE_BONE].UseProgram();
    mShaders[WENGINE_BONE].SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    if(mCurrentRoom) {
        mShaders[WENGINE_BONE].SetUniformLights("lights", mCurrentRoom->GetLights());
        mShaders[WENGINE_BONE].SetUniform1i("numLights", mCurrentRoom->GetLights().size());
    }
    if(mIona.GetFlipped()) {
        glFrontFace(GL_CW);
    }
    else {
        glFrontFace(GL_CCW);
    }
    mIona.Draw(mShaders[WENGINE_BONE], mTextures);
    glFrontFace(GL_CCW);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void WatershedEngine::DrawSprites() {
    mIona.DrawBullets(mSceneRenderer);
}
void WatershedEngine::DrawWater() {
    float waveStrength = 0.1f;
    float waveTimer = 0.001f;
    mShaders[WENGINE_WATER].UseProgram();
    mShaders[WENGINE_WATER].SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    mShaders[WENGINE_WATER].SetUniform1f("timer", SDL_GetTicks() * waveTimer);
    mShaders[WENGINE_WATER].SetUniform1f("strength", waveStrength);
    mShaders[WENGINE_WATER].SetUniform1i("noTex", false);
    mShaders[WENGINE_WATER].SetUniformLight("light", mWaterLight);
    mTextures.BindTexture("water");
    for(auto& w : mCurrentRoom->GetWaterBodies()) {
        w.Draw(mShaders[WENGINE_WATER]);
    }
}

void WatershedEngine::DrawBackgrounds() {
}
void WatershedEngine::DrawParticles(ShaderProgram& sh) {
    mAddPartRend.Render(sh);
    mOpaquePartRend.Render(sh);
}
void WatershedEngine::BufferParticleRenderers() {
    mAddPartRend.BufferData();
    mOpaquePartRend.BufferData();
}
void WatershedEngine::ClearParticleRenderers() {
    mAddPartRend.Reset();
    mOpaquePartRend.Reset();
}
void WatershedEngine::DrawOutlines() {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);

    mShaders[WENGINE_3D_OUTLINE].UseProgram();
    mShaders[WENGINE_3D_OUTLINE].SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    if(mCurrentRoom) {
        if(!mCurrentRoom->GetBoxes().empty()) {
            int modelID = mCurrentRoom->GetBoxes().back().mModelID;
            if(modelID >= 0 && modelID < mModels.size()) {
                Model& m = mModels[modelID];
                float baseZ = m.GetTransform().GetTranslation().z;
                for(auto& box : mCurrentRoom->GetBoxes()) {
                    float z = baseZ + box.GetZOffset();
                    m.SetTranslation(glm::vec3(box.GetPosition().x * mScale * mNorm,
                                               box.GetPosition().y * mScale * mNorm,
                                               z));
                    m.SetRotation(0.0f, 0.0f, box.GetHitBox().GetAngle());
                    m.DrawOutline(mShaders[WENGINE_3D_OUTLINE], mTextures);
                }
                m.SetTranslation(glm::vec3(0, 0, baseZ));
            }
        }
        for(auto platform : mCurrentRoom->GetGroundTiles()) {
            if(platform.GetType() == TILE_PLATFORM) {
                Vector2f pos = platform.GetHitBox().GetCenter();
                mModels[1].SetTranslation(glm::vec3(pos.x * mScale * mNorm,
                                                    pos.y * mScale * mNorm - 0.1f,
                                                    -1.0f));
                mModels[1].DrawOutline(mShaders[WENGINE_3D_OUTLINE], mTextures);
            }
        }
    }
    mShaders[WENGINE_BONE_OUTLINE].UseProgram();
    mShaders[WENGINE_BONE_OUTLINE].SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    mIona.GetModel().DrawOutline(mShaders[WENGINE_BONE_OUTLINE], mTextures);

    glBindTexture(GL_TEXTURE_2D, 0);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0x00);
}
void WatershedEngine::DrawHitBoxes() {
    mSceneRenderer.GetTransform().SetTranslation(glm::vec3(mCamera.position.x * mNorm * mScale,
                                                              mCamera.position.y * mNorm * mScale,
                                                              mZoom - 1.0f));
    mSceneRenderer.GetTransform().SetScale(glm::vec3(mScale));
    if(mShowHitboxes) {
        mSceneRenderer.DrawRect(mIona.GetHitBox(), Color(0.0f, 0.0f, 1.0f, 0.25f));
        //mSceneRenderer.DrawRect(Rectanglef(mIona.GetShellPosition(mNorm * mScale), 10, 10, DRAW_CENTER), Color(1.0, 0.0, 1.0, 0.5));
        mSceneRenderer.DrawRect(Rectanglef(mIona.GetMagicTarget(), 30, 30), Color(0.0, 1.0, 0.5, 0.5));
        for(auto& link : mIona.GetLassoChain().GetLinks()) {
            mSceneRenderer.DrawRect(link.GetHitBox(), Color(1.0f, 0.0f, 1.0f, 0.25f));
        }
        if(mCurrentRoom) {
            mCurrentRoom->DrawHitBoxes(mSceneRenderer);
        }
        mIona.DrawBulletHitBoxes(mSceneRenderer, COL_WHITE);
    }
}
void WatershedEngine::DrawHitBox(Rectanglef rect) {
}
void WatershedEngine::DrawHUDOBject(GUIComponent* component) {
}
void WatershedEngine::DrawQuad(QuadData& q) {
}
void WatershedEngine::AddGUICOmponent(GUIComponent* component) {
}
void WatershedEngine::SetupModels() {
    for(auto& m : mModels) {
        m.SetupModel();
    }
}
}

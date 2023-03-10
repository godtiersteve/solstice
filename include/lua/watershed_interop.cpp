#include "watershed_interop.h"
namespace solstice {
WatershedLuaBind::WatershedLuaBind(float height, float width, std::string script) {
    InitScreen(height, width, mViewAngle, 0.1f, 100.0f);
}
void WatershedLuaBind::BindLua(kaguya::State& state) {
    state["WatershedEngine"].setClass(kaguya::UserdataMetatable<WatershedEngine>()
                                     .addFunction("AddRoom", &WatershedEngine::AddRoom)
                                     .addFunction("AddModel", &WatershedEngine::AddModel)
                                     .addFunction("LoadShader", &WatershedEngine::LoadShader)
                                     .addFunction("LoadTexture", &WatershedEngine::LoadTexture)
                                     .addFunction("LoadTexture2D", &WatershedEngine::LoadTexture2D)
                                     .addFunction("LoadRoom", &WatershedEngine::LoadRoom)
                                     .addFunction("SetFramesPerSecond", &WatershedEngine::SetFramesPerSecond)
                                     .addFunction("SetZoom", &WatershedEngine::SetZoom)
                                     .addFunction("SetScale", &WatershedEngine::SetScale)
                                     .addFunction("InitScreen", &WatershedEngine::InitScreen)
                                     .addFunction("GetModel", &WatershedEngine::GetModelPtr)
                                     .addFunction("GetZoom", &WatershedEngine::GetZoom)
                                     .addFunction("GetScale", &WatershedEngine::GetScale)
                                     .addFunction("GetNorm", &WatershedEngine::GetNorm)
                                     .addFunction("ChangeRoom", &WatershedEngine::ChangeRoom)
                                     .addFunction("GetBoxGrid", &WatershedEngine::GetBoxGrid)
                                     .addFunction("GetLastModel", &WatershedEngine::GetLastModel)
                                     .addFunction("SetupModels", &WatershedEngine::SetupModels)
                                     .addFunction("GetModelID", &WatershedEngine::GetModelID)
                                     .addFunction("GetTextures", &WatershedEngine::GetTextures)
                                     .addFunction("GetTextures2D", &WatershedEngine::GetTextures2D)
                                     .addFunction("GetTextureID", &WatershedEngine::GetTextureID)
                                     .addFunction("GetTextureID2D", &WatershedEngine::GetTextureID2D)
                                     .addFunction("GetWaterLight", &WatershedEngine::GetWaterLightPtr)
                                     .addFunction("SetCameraStepSize", &WatershedEngine::SetCameraStepSize)
                                     .addFunction("SetDrawOutlines", &WatershedEngine::SetDrawOutlines)
                                     .addFunction("SetGroundedCamOffset", &WatershedEngine::SetGroundedCamOffset)
                                     .addFunction("SetOutlineSize", &WatershedEngine::SetOutlineSize)
                                     .addFunction("Set2DTextureBase", &WatershedEngine::Set2DTextureBase)
                                     .addFunction("SetViewScale", &WatershedEngine::SetViewScale)
                                     .addFunction("GetCurrentRoom", &WatershedEngine::GetCurrentRoom)
                                );
    state["Watershed"].setClass(kaguya::UserdataMetatable<WatershedLuaBind, WatershedEngine>()
                                .addFunction("LoadBoneModel", &WatershedLuaBind::LoadBoneModel)
                                .addFunction("LoadModel", &WatershedLuaBind::LoadModel)
                                .addFunction("GetTextureID", &WatershedLuaBind::GetTextureID)
                                .addFunction("LoadModelAndAnimations", &WatershedLuaBind::LoadModelAndAnimations)
                                .addFunction("LoadScript", &WatershedLuaBind::LoadScript)
                                );
    state["engine"] = this;
    state["iona"] = &mIona;
    state["SCREEN_HEIGHT"] = mScreenHeight;
    state["SCREEN_WIDTH"] = mScreenWidth;
    state["state"] = &state;
}
SkeletalModel WatershedLuaBind::LoadBoneModel(std::string file, std::string ref) {
    BoneLoader loader;
    loader.LoadModel(file, mTextures);
    loader.GetModel(0).SetName(ref);
    return loader.GetModel(0);
}
SkeletalModel WatershedLuaBind::LoadModelAndAnimations(std::string prefix, std::string path, std::string ext) {
    BoneLoader loader;
    std::string baseModel = path + "\\meshes\\" + prefix + ext;
    loader.LoadModel(baseModel, mTextures);
    loader.GetModel(0).SetName(prefix);
    std::string animPath = path + "\\animations\\";
    loader.LoadAnimatedModels(animPath, prefix, ext, mTextures);
    return loader.GetModel(0);
}
void WatershedLuaBind::LoadModel(std::string file, std::string ref) {
    mModels.push_back(solstice::LoadModel(file, mTextures));
    mModels.back().SetName(ref);
}
void WatershedLuaBind::UpdateEvents(SDL_Event& event) {
    while(SDL_PollEvent(&event)) {
        mIona.UpdateKeys(event, mController);
        switch(event.type) {
            case SDL_QUIT:
                mFinished = true;
                break;
            case SDL_KEYUP:
                mIona.CheckKeyUp(event.key.keysym.sym);
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    mFinished = true;
                }
                if(event.key.keysym.sym == SDLK_F2) {
                    mIona.Respawn();
                }
                if(event.key.keysym.sym == SDLK_F5) {
                    //mIona.ClearBullets();
                    //std::cout << "Reloading Iona scripts...\n";
                    //mState.dofile("resources\\scripts\\iona.lua");
                    //mState.dofile("resources\\scripts\\iona_guns.lua");
                }
                if(event.key.keysym.sym == SDLK_F6) {
                    //mIona.ClearBullets();
                    //std::cout << "Reloading gun scripts...\n";
                    //mState.dofile("resources\\scripts\\iona_guns.lua");
                }
                if(event.key.keysym.sym == SDLK_F3) {
                    mState.dofile("resources\\scripts\\room_objects.lua");
                    mCurrentRoom->PopulateBodyPointers();
                }
                if(event.key.keysym.sym == SDLK_RETURN) {
                    mPaused = !mPaused;
                }
                if(event.key.keysym.sym == SDLK_KP_1) {
                    mCameraLock = !mCameraLock;
                }
                if(event.key.keysym.sym == SDLK_KP_2) {
                    mShowHitboxes = !mShowHitboxes;
                }
                break;
            case SDL_KEYDOWN:
                //mIona.UpdateKeys(event);
                mIona.CheckKeyDown(event.key.keysym.sym);
                break;
            default:
                break;
        }
    }
}
void WatershedLuaBind::UpdateEvent(SDL_Event& event) {
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
            else if(event.key.keysym.sym == SDLK_F2) {
                mIona.Respawn();
            }
            if(event.key.keysym.sym == SDLK_F5) {
                //mIona.ClearBullets();
                //std::cout << "Reloading Iona scripts...\n";
                mState.dofile("resources\\scripts\\iona.lua");
                //mState.dofile("resources\\scripts\\iona_guns.lua");
            }
            if(event.key.keysym.sym == SDLK_F6) {
                //mIona.ClearBullets();
                //std::cout << "Reloading gun scripts...\n";
                //mState.dofile("resources\\scripts\\iona_guns.lua");
                //mIona.InitBullets(&mState, &mAddPartRend);
            }
            if(event.key.keysym.sym == SDLK_F3) {
                mState.dofile("resources\\scripts\\room_objects.lua");
                mCurrentRoom->PopulateBodyPointers();
            }
            else if(event.key.keysym.sym == SDLK_RETURN) {
                mPaused = !mPaused;
            }
            else if(event.key.keysym.sym == SDLK_KP_1) {
                mCameraLock = !mCameraLock;
            }
            else if(event.key.keysym.sym == SDLK_KP_2) {
                mShowHitboxes = !mShowHitboxes;
            }
            else if(event.key.keysym.sym == SDLK_a) {
                mIona.ToggleSpellLeft();
            }
            else if(event.key.keysym.sym == SDLK_s) {
                mIona.ToggleSpellRight();
            }
            else if(event.key.keysym.sym == SDLK_w) {
                //mViewTransform.angle+= RADIAN * 5;
                //mViewTransform.rotation = glm::vec3(1.0f, 0.0, 0.0f);
            }
            else if(event.key.keysym.sym == SDLK_LSHIFT) {
                mShiftHeld = false;
            }
            if(mShiftHeld) {
                if(event.key.keysym.sym == SDLK_PLUS) {
                    mIona.mInterpModifier+= 0.5;
                }
                else if(event.key.keysym.sym == SDLK_PLUS) {
                    if(mIona.mInterpModifier > 0.5) {
                        mIona.mInterpModifier-= 0.5;
                    }
                }
                else if(event.key.keysym.sym == SDLK_0) {
                    mIona.mInterpModifier = 1.0;
                }
            }
            else {
                if(event.key.keysym.sym == SDLK_PLUS) {
                    if(mTimeScale < 2.0) {
                        mTimeScale += 0.05;
                    }
                }
                else if(event.key.keysym.sym == SDLK_PLUS) {
                    if(mTimeScale > 0.05) {
                        mTimeScale-= 0.05;
                    }
                }
                else if(event.key.keysym.sym == SDLK_0) {
                    ResetTimeScale();
                }
            }
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_LSHIFT) {
                mShiftHeld = true;
            }
            //mIona.UpdateKeys(event);
            //mIona.CheckKeyDown(event.key.keysym.sym);
            break;
        default:
            break;
    }
}
void WatershedLuaBind::LoadScript(std::string script) {
    mState.dofile(script);
}
void WatershedLuaBind::RunScripts() {
    mRooms.clear();
    ClearModels();
    ClearTextures();
    mState.dofile("resources\\scripts\\engine.lua");
    mState.dofile("resources\\scripts\\graphics.lua");
    mState.dofile("resources\\scripts\\room.lua");
    mCurrentRoom = &mRooms.front();
    mState.dofile("resources\\scripts\\room_objects.lua");
    mState.dofile("resources\\scripts\\iona.lua");
    //mState.dofile("resources\\scripts\\iona_guns.lua");
    mState.dofile("resources\\scripts\\iona_model.lua");
    Start();
}
}

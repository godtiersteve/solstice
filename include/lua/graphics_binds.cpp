#include "graphics_binds.h"

namespace solstice {
    void SetupAnimationBinds(kaguya::State& state) {
        state["TextureData"].setClass(kaguya::UserdataMetatable<TextureData>()
                                      .setConstructors<TextureData()>()
                                      .addOverloadedFunctions("Init", static_cast<void (TextureData::*)(Vector2f, float, float)>(&TextureData::Init),
                                                                      static_cast<void (TextureData::*)(float, float, float, float, float, float)>(&TextureData::Init),
                                                                      static_cast<void (TextureData::*)(Vector2f, float, float, float, float)>(&TextureData::Init),
                                                                      static_cast<void (TextureData::*)(Vector2f, Vector2f, Vector2f, Vector2f)>(&TextureData::Init))
                                      );
        state["FrameData"].setClass(kaguya::UserdataMetatable<FrameData, TextureData>());
        state["Frame"].setClass(kaguya::UserdataMetatable<Frame, FrameData>()
                                .setConstructors<Frame()>()
                                );
        state["AnimationBase"].setClass(kaguya::UserdataMetatable<Animator<Frame> >()
                                    .addFunction("Init", &Animator<Frame>::Init)
                                    .addFunction("SetLoops", &Animator<Frame>::SetLoops)
                                    .addFunction("SetUpdateInterval", &Animator<Frame>::SetUpdateInterval)
                                    .addFunction("SetLoopFrame", &Animator<Frame>::SetLoopFrame)
                                    .addFunction("SetName", &Animator<Frame>::SetName)
                                    .addOverloadedFunctions("AddFrame", static_cast<void (Animator<Frame>::*)(Frame)>(&Animator<Frame>::AddFrame),
                                                                       static_cast<void (Animator<Frame>::*)(Frame, size_t)>(&Animator<Frame>::AddFrame))
                                        );
        state["Animation"].setClass(kaguya::UserdataMetatable<Animation, Animator<Frame> >()
                                    .setConstructors<Animation()>()
                                    .addFunction("SetHeight", &Animation::SetHeight)
                                    .addFunction("SetWidth", &Animation::SetWidth)
                                    .addFunction("SetLoop", &Animation::SetLoop)
                                    .addFunction("AddMotion", &Animation::AddMotion)
                                    .addFunction("AddRotation", &Animation::AddRotation)
                                    .addFunction("AddScale", &Animation::AddScale)
                                    .addFunction("AddFade", &Animation::AddFade)
                                    .addFunction("SetTextureID", &Animation::SetTextureID)
                                    .addFunction("SetAngle", &Animation::SetAngle)
                                    .addFunction("GetAngle", &Animation::GetAngle)
                                    .addFunction("SetFrameDur", &Animation::SetFrameDur)
                                    .addFunction("Size", &Animation::size)
                                    .addFunction("GetFrame", &Animation::GetFrame)
                                    .addFunction("AutoAddFramesSep", &Animation::AutoAddFramesSep)
                                    .addFunction("SetDuration", &Animation::SetDuration)
                                    .addFunction("SetColor", &Animation::SetColor)
                                    .addFunction("SetOffset", &Animation::SetOffset)
                                    .addFunction("DuplicateFrame", &Animation::DuplicateFrame)
                                    .addFunction("GetCurFrameNum", &Animation::GetCurFrameNum)
                                    .addFunction("GetLoopFrame", &Animation::GetLoopFrame)
                                    .addFunction("Done", &Animation::Done)
                                    .addFunction("Loops", &Animation::Loops)
                                    .addFunction("SetTexHW", &Animation::SetTexHW)
                                    .addFunction("GetPosition", &Animation::GetPosition)
                                    .addOverloadedFunctions("SetClip", static_cast<void (Animation::*)(Vector2f)>(&Animation::SetClip),
                                                                       static_cast<void (Animation::*)(float, float)>(&Animation::SetClip))
                                    .addOverloadedFunctions("AutoAddFramesSheet", static_cast<void (Animation::*)(int, float, float, float, float)>(&Animation::AutoAddFramesSheet))
                                    .addFunction("Start", &Animation::Start)
                                    .addFunction("Play", &Animation::Play)
                                    .addFunction("Pause", &Animation::Pause)
                                    .addFunction("Stop", &Animation::Stop)
                                    .addFunction("Reset", &Animation::Reset)
                                    .addFunction("Reverse", &Animation::Reverse)
                                    .addFunction("Clear", &Animation::Clear)
                                    .addFunction("SetDrawFrom", &Animation::SetDrawFrom)
                                    );
                                    //.addOverloadedFunctions("SetRotation", static_cast<void (ModelBase<Mesh>::*)(float, float, float)>(&ModelBase<Mesh>::SetRotation))
    }
    void SetupGraphicsBinds(kaguya::State& state) {
        state["PI"] = PI;
        state["TWO_PI"] = TWO_PI;
        state["RADIAN"] = RADIAN;
        state["Color"].setClass(kaguya::UserdataMetatable<Color>()
                                .setConstructors<Color(), Color(ColorCode), Color(float, float, float), Color(float, float, float, float)>()
                                .addProperty("red", &Color::red)
                                .addProperty("green", &Color::green)
                                .addProperty("blue", &Color::blue));
        state["Mesh"].setClass(kaguya::UserdataMetatable<Mesh>()
                               .setConstructors<Mesh()>()
                               .addFunction("SetBackfaceCull", &Mesh::SetBackfaceCull)
                               .addFunction("SetHidden", &Mesh::SetHidden)
                               .addFunction("SetOutlineSize", &Mesh::SetOutlineSize)
                               .addFunction("SetTextureID", &Mesh::SetTextureID)
                               .addFunction("SetTranslation", &Mesh::SetTranslation)
                               .addFunction("SetScale", &Mesh::SetScale)
                               );
        state["Meshf"].setClass(kaguya::UserdataMetatable<MeshBase<glm::vec3> >()
                               .setConstructors<MeshBase<glm::vec3>()>()
                               .addFunction("SetBackfaceCull", &MeshBase<glm::vec3>::SetBackfaceCull)
                               .addFunction("SetHidden", &MeshBase<glm::vec3>::SetHidden)
                               .addFunction("SetOutlineSize", &MeshBase<glm::vec3>::SetOutlineSize)
                               .addFunction("SetTextureID", &MeshBase<glm::vec3>::SetTextureID)
                               .addFunction("SetTranslation", &MeshBase<glm::vec3>::SetTranslation)
                               .addFunction("SetScale", &MeshBase<glm::vec3>::SetScale)
                               );
        state["ModelBase"].setClass(kaguya::UserdataMetatable<ModelBase<Mesh> >()
                                .addOverloadedFunctions("SetAngle", static_cast<void (ModelBase<Mesh>::*)(float)>(&ModelBase<Mesh>::SetAngle))
                                .addFunction("SetOutline", &ModelBase<Mesh>::SetOutlineColor)
                                .addFunction("SortMeshes", &ModelBase<Mesh>::SortMeshes)
                                .addFunction("SetBackfaceCull", &ModelBase<Mesh>::SetBackfaceCull)
                                .addOverloadedFunctions("SetScale", static_cast<void (ModelBase<Mesh>::*)(float, float, float)>(&ModelBase<Mesh>::SetScale))
                                .addOverloadedFunctions("SetTranslation", static_cast<void (ModelBase<Mesh>::*)(float, float, float)>(&ModelBase<Mesh>::SetTranslation))
                                .addOverloadedFunctions("SetRotation", static_cast<void (ModelBase<Mesh>::*)(float, float, float)>(&ModelBase<Mesh>::SetRotation))
                                );
        state["Model"].setClass(kaguya::UserdataMetatable<Model, ModelBase<Mesh> >()
                                .addOverloadedFunctions("GetMesh", static_cast<Mesh* (Model::*)(size_t)>(&Model::GetMeshPtr),
                                                                   static_cast<Mesh* (Model::*)(string)>(&Model::GetMeshPtr)));
        state["AnimationBase"].setClass(kaguya::UserdataMetatable<Animator<AnimKeyFrame> >()
                                        .addFunction("SetLoopFrame", &Animator<AnimKeyFrame>::SetLoopFrame)
                                        .addFunction("SetLoops", &Animator<AnimKeyFrame>::SetLoops)
                                        .addFunction("GetNumFrames", &Animator<AnimKeyFrame>::GetNumFrames)
                                        .addFunction("SetFlags", &Animator<AnimKeyFrame>::SetFlags)
                                        .addFunction("AddFlags", &Animator<AnimKeyFrame>::AddFlags)
                                        .addFunction("HasFlag", &Animator<AnimKeyFrame>::HasFlag)
                                        .addFunction("RemoveFlags", &Animator<AnimKeyFrame>::RemoveFlags)
                                        .addFunction("GetFlags", &Animator<AnimKeyFrame>::GetFlags)
                                        );
        state["ModelAnimation"].setClass(kaguya::UserdataMetatable<ModelAnimation, Animator<AnimKeyFrame> >()
                                    );
        state["Light"].setClass(kaguya::UserdataMetatable<Light>()
                                .setConstructors<Light()>()
                                .addFunction("SetAmbient", &Light::SetAmbient)
                                .addFunction("SetDiffuse", &Light::SetDiffuse)
                                .addFunction("SetSpecular", &Light::SetSpecular)
                                .addFunction("SetPosition", static_cast<void (Light::*)(float, float, float)>(&Light::SetPosition))
                                .addFunction("SetDirection", &Light::SetDirection)
                                .addFunction("SetDirectionAxis", &Light::SetDirectionAxis)
                                .addFunction("SetDirectionAxisDeg", &Light::SetDirectionAxisDeg)
                                .addFunction("SetSpecExponent", &Light::SetSpecExponent)
                                .addFunction("SetSpotCutoff", &Light::SetSpotCutoff)
                                .addFunction("GetSpotCutoff", &Light::GetSpotCutoff)
                                .addFunction("SetPoint", &Light::SetPoint)
                                .addFunction("SetAttenuation", &Light::SetAttenuation)
                                );
        state["SkeletalMeshBase"].setClass(kaguya::UserdataMetatable<MeshBase<SkelVertex3f> >()
                                .addFunction("SetBackfaceCull", &MeshBase<SkelVertex3f>::SetBackfaceCull)
                                .addFunction("SetHidden", &MeshBase<SkelVertex3f>::SetHidden)
                                .addFunction("SetOutlineSize", &MeshBase<SkelVertex3f>::SetOutlineSize)
                                .addFunction("SetNoDepth", &MeshBase<SkelVertex3f>::SetNoDepth)
                                );
        state["SkeletalMesh"].setClass(kaguya::UserdataMetatable<SkeletalMesh, MeshBase<SkelVertex3f> >());
        state["SkeletalModelBase"].setClass(kaguya::UserdataMetatable<ModelBase<SkeletalMesh> >()
                            .addOverloadedFunctions("SetAngle", static_cast<void (ModelBase<SkeletalMesh>::*)(float)>(&ModelBase<SkeletalMesh>::SetAngle))
                            .addFunction("SetOutline", &ModelBase<SkeletalMesh>::SetOutlineColor)
                            .addFunction("SortMeshes", &ModelBase<SkeletalMesh>::SortMeshes)
                            .addFunction("SetOutlineSize", &ModelBase<SkeletalMesh>::SetOutlineSize)
                            .addFunction("SetBackfaceCull", &ModelBase<SkeletalMesh>::SetBackfaceCull)
                            .addOverloadedFunctions("SetScale", static_cast<void (ModelBase<SkeletalMesh>::*)(float, float, float)>(&ModelBase<SkeletalMesh>::SetScale))
                            .addOverloadedFunctions("SetTranslation", static_cast<void (ModelBase<SkeletalMesh>::*)(float, float, float)>(&ModelBase<SkeletalMesh>::SetTranslation))
                            .addOverloadedFunctions("SetRotation", static_cast<void (ModelBase<SkeletalMesh>::*)(float, float, float)>(&ModelBase<SkeletalMesh>::SetRotation))
                            );
        state["AnimatedModel"].setClass(kaguya::UserdataMetatable<SkeletalModel, ModelBase<SkeletalMesh> >()
                                       .setConstructors<SkeletalModel()>()
                                        .addOverloadedFunctions("GetAnimation",
                                                                static_cast<ModelAnimation* (SkeletalModel::*)(size_t)>(&SkeletalModel::GetAnimation),
                                                                static_cast<ModelAnimation* (SkeletalModel::*)(std::string)>(&SkeletalModel::GetAnimation))
                                        .addOverloadedFunctions("GetMesh", static_cast<SkeletalMesh* (SkeletalModel::*)(size_t)>(&SkeletalModel::GetMeshPtr),
                                                                   static_cast<SkeletalMesh* (SkeletalModel::*)(string)>(&SkeletalModel::GetMeshPtr))
                                      );
        state["SplashMesh"].setClass(kaguya::UserdataMetatable<SplashMesh, MeshBase<glm::vec3> >()
                                     .setConstructors<SplashMesh(), SplashMesh(float, size_t, size_t), SplashMesh(float, float, size_t, size_t)>()
                                     .addProperty("bottom", &SplashMesh::bottom)
                                     .addProperty("spread", &SplashMesh::mSpread)
                                     .addProperty("damping", &SplashMesh::mDamping)
                                     .addProperty("springConstant", &SplashMesh::mSpringConstant)
                                     .addProperty("xScale", &SplashMesh::xScale)
                                     .addProperty("yScale", &SplashMesh::yScale)
                                     .addFunction("SetIterations", &SplashMesh::SetIterations)
                                     .addFunction("CreateMesh", &SplashMesh::CreateMesh)
                                     .addOverloadedFunctions("Init", static_cast<void (SplashMesh::*)(float, size_t, size_t)>(&SplashMesh::Init),
                                                                     static_cast<void (SplashMesh::*)(float, float, size_t, size_t)>(&SplashMesh::Init)
                                                             )
                                     );
        state["COL_RED"] = COL_RED;
        state["COL_BLUE"] = COL_BLUE;
        state["COL_GREEN"] = COL_GREEN;
        state["COL_BLACK"] = COL_BLACK;
        state["COL_WHITE"] = COL_WHITE;
        state["COL_PURPLE"] = COL_PURPLE;
        state["COL_PINK"] = COL_PINK;
        state["COL_GREY"] = COL_GRAY;
        state["COL_YELLOW"] = COL_YELLOW;
        state["COL_CYAN"] = COL_CYAN;
        state["COL_MAGENTA"] = COL_MAGENTA;
        state["COL_NAVY"] = COL_NAVY;
        state["COL_BROWN"] = COL_BROWN;
        state["COL_GOLD"] = COL_GOLD;
        state["COL_SILVER"] = COL_SILVER;
        state["COL_INDIGO"] = COL_INDIGO;
        state["COL_OLIVE"] = COL_OLIVE;
        state["COL_ORANGE"] = COL_ORANGE;
        state["COL_VIOLET"] = COL_VIOLET;
    }
}

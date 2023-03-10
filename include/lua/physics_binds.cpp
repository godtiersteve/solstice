#include "physics_binds.h"

namespace solstice {
void SetupPhysicsBinds(kaguya::State& state) {
    state["Vector2f"].setClass(kaguya::UserdataMetatable<Vector2f>()
                           .setConstructors<Vector2f(), Vector2f(float, float)>()
                           .addProperty("x", &Vector2f::x)
                           .addProperty("y", &Vector2f::y)
                           );
    state["vec2"].setClass(kaguya::UserdataMetatable<glm::vec2>()
                           .setConstructors<glm::vec2(), glm::vec2(float, float)>()
                           .addProperty("x", &glm::vec2::x)
                           .addProperty("y", &glm::vec2::y)
                           );
    state["Vector3f"].setClass(kaguya::UserdataMetatable<glm::vec3>()
                               .setConstructors<glm::vec3(), glm::vec3(float, float, float)>()
                               .addProperty("x", &glm::vec3::x)
                               .addProperty("y", &glm::vec3::y)
                               .addProperty("z", &glm::vec3::y)
                               );
    state["Rectangle"].setClass(kaguya::UserdataMetatable<Rectanglef>()
                                .setConstructors<Rectanglef(), Rectanglef(Vector2f, float, float)>()
                                .addFunction("SetHeight", &Rectanglef::SetHeight)
                                .addFunction("SetWidth", &Rectanglef::SetWidth)
                                .addFunction("SetHW", &Rectanglef::SetHW)
                                .addFunction("SetX", &Rectanglef::SetX)
                                .addFunction("SetY", &Rectanglef::SetY)
                                .addFunction("SetXY", &Rectanglef::SetXY)
                                .addFunction("SetPosition", &Rectanglef::SetPosition)
                                .addOverloadedFunctions("SetXYHW", static_cast<void (Rectanglef::*)(float, float, float, float)>(&Rectanglef::SetXYHW))
                                .addOverloadedFunctions("Init", static_cast<void (Rectanglef::*)(Vector2f, float, float, DrawPoint)>(&Rectanglef::Init))
                                .addFunction("SetScale", &Rectanglef::SetScale)
                                .addFunction("SetSize", &Rectanglef::SetSize)
                                .addFunction("SetBottom", &Rectanglef::SetBottom)
                                .addFunction("SetTop", &Rectanglef::SetTop)
                                .addFunction("SetLeft", &Rectanglef::SetLeft)
                                .addFunction("SetRight", &Rectanglef::SetRight)
                                .addFunction("GetBottom", &Rectanglef::GetBottom)
                                .addFunction("GetTop", &Rectanglef::GetTop)
                                .addFunction("GetLeft", &Rectanglef::GetLeft)
                                .addFunction("GetRight", &Rectanglef::GetRight)
                                .addFunction("GetHeight", &Rectanglef::GetHeight)
                                .addFunction("GetWidth", &Rectanglef::GetWidth)
                                .addFunction("SetDrawFrom", &Rectanglef::SetDrawFrom)
                                );
    state["Circle"].setClass(kaguya::UserdataMetatable<Circlef>()
                             .setConstructors<Circlef(), Circlef(Vector2f, float)>()
                             .addFunction("SetSize", &Circlef::SetSize)
                             .addFunction("SetPosition", &Circlef::SetPosition)
                             .addFunction("GetSize", &Circlef::GetRadius)
                             .addProperty("GetPosition", &Circlef::GetCenter)
                             );
    state["RigidBody"].setClass(kaguya::UserdataMetatable<RigidBody>()
                                .addFunction("SetMass", &RigidBody::SetMass)
                                .addFunction("SetInvMass", &RigidBody::SetInvMass)
                                .addFunction("SetElasticity", &RigidBody::SetElasticity)
                                .addFunction("SetFriction", &RigidBody::SetFriction)
                                .addFunction("SetBuoyancy", &RigidBody::SetBuoyancy)
                                .addFunction("SetNoTorque", &RigidBody::SetNoTorque)
                                .addFunction("SetInertia", &RigidBody::SetInertia)
                                .addFunction("SetAngleDamping", &RigidBody::SetAngleDamping)
                                .addFunction("GetHitBox", &RigidBody::GetHitBoxPtr)
                                .addFunction("InitHitBox", &RigidBody::InitHitBox)
                                .addFunction("SetAngle", &RigidBody::SetAngle)
                                .addFunction("SetIsPlatform", &RigidBody::SetIsPlatform)
                                .addFunction("SetNoPlatform", &RigidBody::SetNoPlatform)
                                .addFunction("SetNoFloat", &PhysicsBox2f::SetNoFloat)
                                );
    state["Constraint"].setClass(kaguya::UserdataMetatable<Constraint>()
                                 .setConstructors<Constraint(float, float, float)>()
                                 .addFunction("SetMinLength", &Constraint::SetMinLength)
                                 .addFunction("SetStiffness", &Constraint::SetStiffness)
                                 .addFunction("SetDamping", &Constraint::SetDamping)
                                 .addFunction("SetAttributes", &Constraint::SetAttributes)
                                 );
    state["Chain"].setClass(kaguya::UserdataMetatable<Chain>()
                            .setConstructors<Chain(), Chain(Constraint), Chain(Constraint, Constraint)>()
                            .addFunction("Attach", &Chain::Attach)
                            .addFunction("SetEndConst", &Chain::SetEndConst)
                            .addFunction("SetLinkConst", &Chain::SetLinkConst)
                            .addFunction("SetNumIter", &Chain::SetNumIter)
                            .addFunction("SetAnchorPoint", &Chain::SetAnchorPoint)
                            .addOverloadedFunctions("Init", static_cast<void (Chain::*)(int, float)>(&Chain::Init),
                                                            static_cast<void (Chain::*)(int, float, Constraint)>(&Chain::Init),
                                                            static_cast<void (Chain::*)(int, PhysicsBox2f, Constraint)>(&Chain::Init))
                            );
    state["PhysicsBox"].setClass(kaguya::UserdataMetatable<PhysicsBox2f, RigidBody>()
                                 .setConstructors<PhysicsBox2f()>()
                                 .addFunction("SetAcceleration", &PhysicsBox2f::SetAcceleration)
                                 .addFunction("SetAccelerationX", &PhysicsBox2f::SetAccelerationX)
                                 .addFunction("SetAccelerationY", &PhysicsBox2f::SetAccelerationX)
                                 .addFunction("SetDeceleration", &PhysicsBox2f::SetDeceleration)
                                 .addFunction("SetDecelerationX", &PhysicsBox2f::SetDecelerationX)
                                 .addFunction("SetDecelerationY", &PhysicsBox2f::SetDecelerationX)
                                 .addFunction("SetMaxVelocity", &PhysicsBox2f::SetMaxVelocity)
                                 .addFunction("SetGravityAccel", &PhysicsBox2f::SetGravityAccel)
                                 .addFunction("SetDepthModifier", &PhysicsBox2f::SetDepthModifier)
                                 .addFunction("SetBuoyancyDamping", &PhysicsBox2f::SetBuoyancyDamping)
                                 .addFunction("SetNoGravity", &PhysicsBox2f::SetNoGravity)
                                 .addFunction("SetIgnorePhysics", &PhysicsBox2f::SetIgnorePhysics)
                                 .addFunction("SetWaterDamping", &PhysicsBox2f::SetWaterDamping)
                                 .addFunction("SetNoCollision", &PhysicsBox2f::SetNoCollision)
                                 .addFunction("SetPosition", &PhysicsBox2f::SetPosition)
                                 );
    state["Player"].setClass(kaguya::UserdataMetatable<BasePlayer2, PhysicsBox2f>()
                             .addFunction("SetHitStun", &BasePlayer2::SetHitStun)
                             .addOverloadedFunctions("SetJumpStrength", static_cast<void (BasePlayer2::*)(float)>(&BasePlayer2::SetJumpStrength),
                                                                        static_cast<void (BasePlayer2::*)(float, float)>(&BasePlayer2::SetJumpStrength))
                             .addFunction("SetHoldJumpStrength", &BasePlayer2::SetHoldJumpStrength)
                             .addFunction("SetSpawnPosition", &BasePlayer2::SetSpawnPosition)
                             .addFunction("SetLandStop", &BasePlayer2::SetLandStop)
                             .addFunction("SetAirAccels", &BasePlayer2::SetAirAccels)
                             .addFunction("SetHoldJump", &BasePlayer2::SetHoldJump)
                             .addOverloadedFunctions("SetWalkSpeed", static_cast<void (BasePlayer2::*)(float)>(&BasePlayer2::SetWalkSpeed))
                             .addFunction("SetWalkAcceleration", &BasePlayer2::SetWalkAcceleration)
                             .addFunction("SetWaterJumpStrength", &BasePlayer2::SetWaterJumpStrength)
                             .addFunction("SetIgnoreInput", &BasePlayer2::SetIgnoreInput)
                             .addFunction("SetModelOffset", &BasePlayer2::SetModelOffset)
                             .addFunction("GetModel", &BasePlayer2::GetModelPtr)
                             .addFunction("SetupModel", &BasePlayer2::SetupModel)
                             .addFunction("SetModel", &BasePlayer2::SetModel)
                             );
    state["WatershedPlayer"].setClass(kaguya::UserdataMetatable<WatershedPlayer, BasePlayer2>()
                                      .addFunction("SetSwimMaxVel", &WatershedPlayer::SetSwimMaxVel)
                                      .addFunction("SetSwimAccel", &WatershedPlayer::SetSwimAccel)
                                      .addFunction("SetSwimDecel", &WatershedPlayer::SetSwimDecel)
                                      .addFunction("SetFocusModifier", &WatershedPlayer::SetFocusModifier)
                                      .addFunction("SetThrowStrength", &WatershedPlayer::SetThrowStrength)
                                      .addFunction("GetLassoChain", &WatershedPlayer::GetLassoChainPtr)
                                      .addFunction("SetHoldPoint", &WatershedPlayer::SetHoldPoint)
                                      .addFunction("SetDiveVel", &WatershedPlayer::SetDiveVel)
                                      .addFunction("SetSurfaceVel", &WatershedPlayer::SetSurfaceVel)
                                      .addFunction("SetSkidStop", &WatershedPlayer::SetSkidStop)
                                      .addFunction("SetFocusAccels", &WatershedPlayer::SetFocusAccels)
                                      .addFunction("GetHairBandModel", &WatershedPlayer::GetHairbandPtr)
                                      .addFunction("GetHoloRodModel", &WatershedPlayer::GetHoloRodPtr)
                                      .addFunction("SetHairband", &WatershedPlayer::SetHairband)
                                      .addFunction("SetHoloRod", &WatershedPlayer::SetHoloRod)
                                      .addFunction("SetShellModel", &WatershedPlayer::SetShellModel)
                                      .addFunction("SetupModels", &WatershedPlayer::SetupModels)
                                      .addFunction("SetGroundFocusModifier", &WatershedPlayer::SetGroundFocusModifier)
                                      .addFunction("SetGrabRange", &WatershedPlayer::SetGrabRange)
                                      .addFunction("SetShotOrigin", &WatershedPlayer::SetShotOrigin)
                                      .addFunction("GetHurlHitbox", &WatershedPlayer::GetHurlHitboxPtr)
                                     );
    state["Crate"].setClass(kaguya::UserdataMetatable<Crate, PhysicsBox2f>()
                            .setConstructors<Crate()>()
                            .addFunction("SetModelID", &Crate::SetModelID)
                            );
    state["Spring"].setClass(kaguya::UserdataMetatable<Spring>()
                             .setConstructors<Spring(), Spring(Vector2f, float, float)>()
                             .addProperty("height", &Spring::height)
                             .addProperty("speed", &Spring::speed)
                             .addProperty("k", &Spring::k)
                             .addProperty("damping", &Spring::dampening)
                             );
    state["GroundTile"].setClass(kaguya::UserdataMetatable<BackgroundTile2>()
                                 .setConstructors<BackgroundTile2(), BackgroundTile2(Rectanglef, TileType)>()
                                 .addFunction("GetHitBox", &BackgroundTile2::GetHitBoxPtr)
                                 .addFunction("SetPosition", &BackgroundTile2::SetPosition)
                                 .addFunction("SetFriction", &BackgroundTile2::SetFriction)
                                 .addFunction("SetElasticity", &BackgroundTile2::SetElasticity)
                                 .addFunction("SetSprings", &BackgroundTile2::SetSprings)
                                 .addFunction("SetType", &BackgroundTile2::SetType)
                                 .addFunction("GetSpring", &BackgroundTile2::GetSpringPtr)
                                 );
    state["WaterBody"].setClass(kaguya::UserdataMetatable<WaterBody>()
                                .setConstructors<WaterBody(), WaterBody(Vector2f, float, float, float, float)>()
                                .addFunction("GetWaterMesh", &WaterBody::GetWaterMeshPtr)
                                .addFunction("SetPosition", &WaterBody::SetPosition)
                                .addFunction("GetHitBox", &WaterBody::GetHitBox)
                                .addFunction("SetSplashMesh", &WaterBody::SetSplashMesh)
                                .addFunction("SetCamBoundary", &WaterBody::SetCamBoundary)
                                .addFunction("SetZSplashPos", &WaterBody::SetZSplashPos)
                                .addOverloadedFunctions("Init", static_cast<void (WaterBody::*)(Vector2f, float, float, float, float)>(&WaterBody::Init))
                                );
    state["Room"].setClass(kaguya::UserdataMetatable<LevelRoom>()
                           .setConstructors<LevelRoom(), LevelRoom(string)>()
                           .addFunction("AddCrate", &LevelRoom::AddCrate)
                           .addFunction("AddLight", &LevelRoom::AddLight)
                           .addFunction("AddTile", &LevelRoom::AddTile)
                           .addFunction("AddWaterBody", &LevelRoom::AddWaterBody)
                           .addFunction("AddChain", &LevelRoom::AddChain)
                           .addFunction("GetCamBoundary", &LevelRoom::GetCamBoundary)
                           .addFunction("SetCamBoundary", &LevelRoom::SetCamBoundary)
                           .addFunction("SetWaterCamBoundary", &LevelRoom::SetWaterCamBoundary)
                           .addFunction("SetModelID", &LevelRoom::SetModelID)
                           .addFunction("Read", &LevelRoom::Read)
                           .addFunction("GetTile", &LevelRoom::GetTile)
                           .addFunction("SetName", &LevelRoom::SetName)
                           .addFunction("AttachBoxToChain", &LevelRoom::AttachBoxToChain)
                           .addFunction("GetCrate", &LevelRoom::GetCratePtr)
                           .addFunction("ClearCrates", &LevelRoom::ClearCrates)
                           );
    state["CameraBoundary"].setClass(kaguya::UserdataMetatable<CameraBoundary, Rectanglef>()
                                    .setConstructors<CameraBoundary(), CameraBoundary(Rectanglef)>()
                                    .addProperty("offsetMax", &CameraBoundary::offsetMax)
                                    .addProperty("offsetMin", &CameraBoundary::offsetMin)
                                    .addProperty("offsetStep", &CameraBoundary::offsetStep)
                                    .addProperty("velocityRange", &CameraBoundary::velocityRange)
                                    .addProperty("offsetSlowStep", &CameraBoundary::offsetSlowStep)
                                    .addProperty("horiLimit", &CameraBoundary::horiLimit)
                                    .addProperty("vertLimit", &CameraBoundary::vertLimit)
                                    .addFunction("Init", &CameraBoundary::Init)
                                     );
    state["Camera"].setClass(kaguya::UserdataMetatable<Camera>()
                             .setConstructors<Camera()>()
                             .addFunction("SetPosition", &Camera::SetPosition)
                             .addProperty("origin", &Camera::origin)
                             .addProperty("stepSize", &Camera::stepSize)
                             .addProperty("xOffset", &Camera::xOffset)
                             .addProperty("yOffset", &Camera::yOffset)
                             .addProperty("mBoundary", &Camera::mBoundary)
                             );
    state["SpatialGrid"].setClass(kaguya::UserdataMetatable<SpatialGrid>()
                                  .setConstructors<SpatialGrid(), SpatialGrid(int, int, int, size_t)>()
                                  .addFunction("Init", &SpatialGrid::Init)
                                  );
    state["DRAW_CENTER"] = DRAW_CENTER;
    state["DRAW_TOPLEFT"] = DRAW_TOPLEFT;
    state["DRAW_TOPRIGHT"] = DRAW_TOPRIGHT;
    state["DRAW_TOPCENTER"] = DRAW_TOPCENTER;
    state["DRAW_BOTCENTER"] = DRAW_BOTCENTER;
    state["DRAW_LEFTCENTER"] = DRAW_LEFTCENTER;
    state["DRAW_RIGHTCENTER"] = DRAW_RIGHTCENTER;
    state["DRAW_BOTLEFT"] = DRAW_BOTLEFT;
    state["DRAW_BOTRIGHT"] = DRAW_BOTRIGHT;
    state["TILE_GROUND"] = TILE_GROUND;
    state["TILE_MOVING"] = TILE_MOVING;
    state["TILE_PLATFORM"] = TILE_PLATFORM;
    state["TILE_WATER"] = TILE_WATER ;
}

}

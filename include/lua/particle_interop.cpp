#include "particle_interop.h"
namespace solstice {
LuaParticleSprayer::LuaParticleSprayer() {
    mActiveEffect = 0;
}
void LuaParticleSprayer::SetupBinds(kaguya::State& state) {
    state["LuaParticleSprayer"].setClass(kaguya::UserdataMetatable<LuaParticleSprayer>()
                                         .setConstructors<LuaParticleSprayer()>()
                                         .addFunction("AddSprayer", &LuaParticleSprayer::AddSprayer)
                                         .addFunction("GetSprayer", &LuaParticleSprayer::GetSprayerPtr)
                                         .addFunction("GetTexture", &LuaParticleSprayer::GetTexturePtr)
                                         .addFunction("GetTextureID", &LuaParticleSprayer::GetTextureID)
                                         .addFunction("LoadTexture", &LuaParticleSprayer::LoadTexture)
                                         .addFunction("AddEffect", &LuaParticleSprayer::AddEffect)
                                         );
}
void LuaParticleSprayer::Update(double dt) {
    if(mEffects.empty()) {
        return;
    }
    mNormalRenderer.Reset();
    mAddRenderer.Reset();
    /*
    for(auto& effect : mEffects) {
        effect.Update(dt);
    }
    */
    mEffects[mActiveEffect].Update(dt);
}
void LuaParticleSprayer::Draw(ShaderProgram& sh) {
    mNormalRenderer.Render(sh);
    mAddRenderer.Render(sh);
}
ParticleSprayer* LuaParticleSprayer::GetSprayerPtr(size_t i) {
    if(mEffects.empty() || mEffects[0].GetSprayers().empty()) {
        return NULL;
    }
    else {
        return &mEffects[0].GetSprayer(i);
    }
}
ParticleSprayer& LuaParticleSprayer::GetSprayer(size_t i) {
    return mEffects[0].GetSprayer(i);
}
void LuaParticleSprayer::AddSprayer(ParticleSprayer s) {
    if(mEffects.empty()) {
        mEffects.push_back(ParticleEffect());
    }
    mEffects[0].AddSprayer(s);
}
void LuaParticleSprayer::NextActiveEffect() {
    mActiveEffect++;
    if(mActiveEffect >= mEffects.size()) {
        mActiveEffect = 0;
    }
}
void LuaParticleSprayer::LastActiveEffect() {
    mActiveEffect--;
    if(mActiveEffect < 0) {
        mActiveEffect = mEffects.size() - 1;
    }
}
size_t LuaParticleSprayer::GetNumParticles() {
    size_t total = 0;
    for(auto emitter : mEffects) {
        total+= emitter.GetNumParticles();
    }
    return total;
}
void LuaParticleSprayer::SetupVBOs(int height, int width, int buffer) {
    mAddRenderer.SetNorm(Vector2f(2.0 / width, 2.0 / height));
    mAddRenderer.SetSize(buffer);
    mNormalRenderer.SetNorm(Vector2f(2.0 / width, 2.0 / height));
    mNormalRenderer.SetSize(buffer);
    mAddRenderer.CreateBuffers();
    mNormalRenderer.CreateBuffers();
    for(size_t i = 0; i < mEffects.size(); i++) {
        for(size_t j = 0; j < mEffects[i].GetSprayers().size(); j++) {
            if(mEffects[i].GetSprayer(j).GetBlendDest() == GL_ONE) {
                mEffects[i].GetSprayer(j).SetRenderer(&mAddRenderer);
            }
            else {
                mEffects[i].GetSprayer(j).SetRenderer(&mNormalRenderer);
            }
        }
    }
}
void LuaParticleSprayer::SetNorms(Vector2f vt) {
    mAddRenderer.SetNorm(vt);
    mNormalRenderer.SetNorm(vt);
}
}

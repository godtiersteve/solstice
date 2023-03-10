#include "particleeffect.h"

namespace solstice {
    ParticleEffect::ParticleEffect() {
    }
    ParticleEffect::ParticleEffect(std::vector<ParticleSprayer> sprayers) {
        mSprayers = sprayers;
    }
    ParticleEffect::ParticleEffect(std::vector<ParticleSprayer> sprayers, glm::vec3 pos) {
        mSprayers = sprayers;
        mPosition = pos;
    }
    void ParticleEffect::Update(double dt, glm::vec3 pos) {
        for(auto& sprayer : mSprayers) {
            sprayer.SetParticleSpawnPos(pos);
            sprayer.Update(dt, mPosition);
        }
    }
    void ParticleEffect::SetSpawnPositions(glm::vec3 pos) {
        for(auto& sprayer : mSprayers) {
            sprayer.SetParticleSpawnPos(pos);
        }
    }
    void ParticleEffect::AddVelocity(glm::vec3 vel) {
        for(auto& sprayer : mSprayers) {
            sprayer.AddVelocity(vel);
        }
    }
    size_t ParticleEffect::GetNumParticles() {
        size_t total = 0;
        for(auto& sprayer : mSprayers) {
            total+= sprayer.GetNumParticles();
        }
        return total;
    }
    void ParticleEffect::Reset() {
        for(auto& sprayer : mSprayers) {
            sprayer.Reset();
        }
    }
    bool ParticleEffect::Finished() {
        int numFinished = 0;
        for(auto& sprayer : mSprayers) {
            if(sprayer.Finished()) {
                numFinished++;
            }
        }
        if(numFinished >= mSprayers.size()) {
            return true;
        }
        return false;
    }
    void ParticleEffect::SetInfluenceAngle(Vector2f a) {
        for(auto& sprayer : mSprayers) {
            sprayer.SetInfluenceAngle(a);
        }
    }
    void ParticleEffect::SetRenderer(ParticleRenderer* r) {
        for(size_t i = 0; i < mSprayers.size(); i++) {
            mSprayers[i].SetRenderer(r);
        }
    }
    void ParticleEffect::DeactivateSprayers() {
        for(size_t i = 0; i < mSprayers.size(); i++) {
            mSprayers[i].mActive = false;
        }
    }
    void ParticleEffect::ActivateSprayers() {
        for(size_t i = 0; i < mSprayers.size(); i++) {
            mSprayers[i].mActive = true;
        }
    }
}

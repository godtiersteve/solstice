#ifndef PARTICLEEFFECT_H_INCLUDED
#define PARTICLEEFFECT_H_INCLUDED

#include "particle.h"
#include "particlesprayer.h"
#include <vector>

namespace solstice {
class ParticleEffect {
private:
    std::vector<ParticleSprayer> mSprayers;
    glm::vec3 mPosition;
public:
    ParticleEffect();
    ParticleEffect(std::vector<ParticleSprayer> sprayers);
    ParticleEffect(std::vector<ParticleSprayer> sprayers, glm::vec3 pos);

    std::vector<ParticleSprayer>& GetSprayers()         {return mSprayers;}
    ParticleSprayer& GetSprayer(size_t i)               {return mSprayers[i];}
    void AddSprayer(ParticleSprayer p)                  {mSprayers.push_back(p);}

    glm::vec3 GetPosition()                             {return mPosition;}
    void SetPosition(glm::vec3 pos)                     {mPosition = pos;}
    void SetInfluenceAngle(Vector2f a);
    void Update(double dt = 1.0, glm::vec3 pos = glm::vec3(0));
    void AddVelocity(glm::vec3 vel);
    void SetSpawnPositions(glm::vec3 pos);
    size_t GetNumParticles();
    void Reset();
    bool Finished();
    void SetRenderer(ParticleRenderer* r);
    void DeactivateSprayers();
    void ActivateSprayers();
};

}


#endif // PARTICLEEFFECT_H_INCLUDED

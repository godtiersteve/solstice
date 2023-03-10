#ifndef PARTICLE_INTEROP_H_INCLUDED
#define PARTICLE_INTEROP_H_INCLUDED

#include "kaguya/kaguya.hpp"
#include "particle/particlesprayer.h"
#include "particle/particleeffect.h"
#include "particle/particlerenderer.h"
#include "graphics/texarray.h"
#include <vector>
namespace solstice {
class LuaParticleSprayer {
    std::vector<ParticleEffect> mEffects;
    int mActiveEffect;
    TextureArray mTextures;
    std::vector<float> mSizes, mAlphas;
    std::vector<glm::vec3> mPositions;
    std::vector<glm::vec2> mTextureOffsets;
    std::vector<glm::vec4> mColors;
    GLuint mSizeBuffer, mAlphaBuffer, mPositionBuffer, mClipBuffer, mVao;
    ParticleRenderer mAddRenderer, mNormalRenderer;
public:
    LuaParticleSprayer();
    void SetupBinds(kaguya::State& state);
    void AddSprayer(ParticleSprayer s);
    void AddEffect(ParticleEffect e)                                {mEffects.push_back(e);}
    void NextActiveEffect();
    void LastActiveEffect();
    ParticleSprayer* GetSprayerPtr(size_t i);
    ParticleSprayer& GetSprayer(size_t i);
    Texture* GetTexturePtr(size_t i)                                {return &mTextures.GetTexture(i);}
    Texture& GetTexture(size_t i)                                   {return mTextures.GetTexture(i);}
    int GetTextureID(std::string name)                              {return mTextures.GetTextureID(name);}
    void LoadTexture(std::string str, std::string name)             {mTextures.LoadImg(str);}
    void Update(double dt);
    void Draw(ShaderProgram& sh);
    void SetupVBOs(int height, int width, int buffer);
    ParticleEffect& GetEmitter(size_t i)                            {return mEffects[i];}
    TextureArray& GetTextures()                                     {return mTextures;}
    std::vector<ParticleEffect>& GetEmitters()                      {return mEffects;}
    void SetNorms(Vector2f vt);
    size_t GetNumParticles();
};
}
#endif // PARTICLE_INTEROP_H_INCLUDED

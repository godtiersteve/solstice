#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "particle.h"
#include "graphics/graphicglobals.h"
#include "geometry/rectangle.h"
#include "geometry/polygon.h"
#include "geometry/hitbox.h"
#include "graphics/shaderprogram.h"
#include "graphics/quaddata.h"
#include "graphics/multiquad.h"
#include "graphics/vertex2.h"
#include "utility/util.h"

namespace solstice {
class ParticleRenderer {
    public:
        ParticleRenderer(int bufferSize = 10000);

        void Reset();
        void CreateBuffers();
        void BufferData();
        void ResizeBuffer(int size);
        void Render(ShaderProgram& sh);
        void Render(ShaderProgram& sh, Texture& texture);
        void DrawParticle(Particle& part, glm::vec3 offset = glm::vec3(0));

        GLuint GetVao()             {return mVao;}
        GLuint GetSizeBuffer()      {return mSizeBuffer;}
        GLuint GetAlphaBuffer()     {return mAlphaBuffer;}
        GLuint GetPositionBuffer()  {return mPositionBuffer;}
        GLuint GetCLipBuffer()      {return mClipBuffer;}
        std::vector<glm::vec3> GetParticlePositions() { return mParticlePositions; }
        std::vector<float> GetAlphas() { return mAlphas; }
        std::vector<float> GetSizes() { return mSizes; }
        std::vector<glm::vec3> GetPositions() { return mPositions; }
        std::vector<glm::vec2> GetTextureOffsets() { return mTextureOffsets; }
        int GetUsedParticles() { return mUsedParticles; }
        void SetNorm(float n)           {mNorm = n;}
        float GetNorm()                 {return mNorm;}

    protected:

    private:
        int mBufferSize, mUsedParticles;
        GLuint mSizeBuffer, mAlphaBuffer, mPositionBuffer, mClipBuffer, mVao;
        std::vector<glm::vec3> mParticlePositions;
        std::vector<float> mSizes, mAlphas;
        std::vector<glm::vec3> mPositions;
        std::vector<glm::vec2> mTextureOffsets;
        float mNorm;
};
}

#endif // PARTICLERENDERER_H

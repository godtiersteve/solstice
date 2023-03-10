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
        ParticleRenderer(int bufferSize = 1000);

        void Reset();
        void CreateBuffers();
        void BufferData();
        void ResizeBuffer(int size);
        void Render(ShaderProgram& sh);
        void Render(ShaderProgram& sh, Texture& texture);
        void DrawParticle(Particle& part, glm::vec4 color, glm::vec3 offset, int id);

        GLuint GetVao()             {return mVao;}
        GLuint GetSizeBuffer()      {return mSizeBuffer;}
        GLuint GetColorBuffer()     {return mColorBuffer;}
        GLuint GetPositionBuffer()  {return mPositionBuffer;}
        GLuint GetCLipBuffer()      {return mClipBuffer;}
        std::vector<glm::vec3> GetParticlePositions() { return mParticlePositions; }
        std::vector<glm::vec4> GetColors() { return mColors; }
        std::vector<float> GetSizes() { return mSizes; }
        std::vector<glm::vec3> GetPositions() { return mPositions; }
        std::vector<glm::vec2> GetTextureOffsets() { return mTextureOffsets; }
        int GetUsedParticles()          { return mUsedParticles; }
        void SetNorm(Vector2f n)           {mNorm = n;}
        Vector2f GetNorm()                 {return mNorm;}
        void SetBlend(GLenum src, GLenum dest) {mBlendDest = dest; mBlendSrc = src;}
        GLenum GetBlendSrc()            {return mBlendSrc;}
        GLenum GetBlendDest()           {return mBlendDest;}
        void SetClip(Vector2f clip)     {mClip = clip;}
        Vector2f GetClip()              {return mClip;}
        void SetSize(int size)                  {mBufferSize = size;}

    protected:

    private:
        int mBufferSize, mUsedParticles;
        GLenum mBlendSrc, mBlendDest;
        GLuint mSizeBuffer, mPositionBuffer, mClipBuffer, mColorBuffer, mTextureBuffer, mVao;
        std::vector<glm::vec3> mParticlePositions;
        std::vector<glm::vec4> mColors;
        std::vector<GLfloat> mSizes;
        std::vector<glm::vec3> mPositions;
        std::vector<glm::vec2> mTextureOffsets;
        std::vector<GLint> mTextureIDs;
        Vector2f mNorm, mClip;
};
}

#endif // PARTICLERENDERER_H

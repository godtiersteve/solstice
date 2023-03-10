#include "particlerenderer.h"
namespace solstice {
ParticleRenderer::ParticleRenderer(int bufferSize)
{
    ResizeBuffer(bufferSize);
    mUsedParticles = 0;
    mNorm = 1.0;
}
void ParticleRenderer::Reset() {
    mUsedParticles = 0;
}
void ParticleRenderer::CreateBuffers() {
    if(!glIsBuffer(mSizeBuffer)) {
        glGenBuffers(1, &mSizeBuffer);
    }
    if(!glIsBuffer(mAlphaBuffer)) {
        glGenBuffers(1, &mAlphaBuffer);
    }
    if(!glIsBuffer(mPositionBuffer)) {
        glGenBuffers(1, &mPositionBuffer);
    }
    if(!glIsBuffer(mClipBuffer)) {
        glGenBuffers(1, &mClipBuffer);
    }
    if(!glIsVertexArray(mVao)) {
        glGenVertexArrays(1, &mVao);
    }
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * mPositions.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * mSizes.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mAlphaBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * mAlphas.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec2) * mTextureOffsets.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, mAlphaBuffer);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ParticleRenderer::BufferData() {
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec3) * mUsedParticles,
                    &mPositions[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(float) * mUsedParticles,
                    &mSizes[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mAlphaBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(float) * mUsedParticles,
                    &mAlphas[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec2) * mUsedParticles,
                    &mTextureOffsets[0]);
}
void ParticleRenderer::ResizeBuffer(int size) {
    mParticlePositions.resize(size);
    mSizes.resize(size);
    mAlphas.resize(size);
    mPositions.resize(size);
    mTextureOffsets.resize(size);
}
void ParticleRenderer::DrawParticle(Particle& part, glm::vec3 offset) {
    if(mUsedParticles >= mPositions.size()) {
        ResizeBuffer(mPositions.size() * 2);
    }
    mAlphas[mUsedParticles] = part.GetAlpha();
    mAlphas[mUsedParticles] = part.GetAlpha();
    mPositions[mUsedParticles] = glm::vec3((offset + part.GetPosition()));
    mPositions[mUsedParticles].x*= mNorm;
    mPositions[mUsedParticles].y*= mNorm;
    mSizes[mUsedParticles] = part.GetSize();
    mTextureOffsets[mUsedParticles] = part.GetTextureOffsets();
    mUsedParticles++;
}
void ParticleRenderer::Render(ShaderProgram& sh) {
    if(!glIsVertexArray(mVao) || !mUsedParticles) {
        return;
    }
    //glBlendFunc(mBlendSrc, mBlendDest);
    sh.SetUniform1i("tex", 0);
    //sh.SetUniform2f("clipSize", mTextureClip.x, mTextureClip.y);
    BufferData();
    glDrawArrays(GL_POINTS, 0, mUsedParticles);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(0);
}
void ParticleRenderer::Render(ShaderProgram& sh, Texture& texture) {
    texture.Bind();
    Render(sh);
}
}

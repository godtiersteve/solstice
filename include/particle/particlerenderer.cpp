#include "particlerenderer.h"
namespace solstice {
ParticleRenderer::ParticleRenderer(int bufferSize) {
    mUsedParticles = 0;
    mNorm = 1.0;
    mBlendSrc = GL_SRC_ALPHA;
    mBlendDest = GL_ONE;
    mClip = 1;
    mBufferSize = bufferSize;
}
void ParticleRenderer::Reset() {
    mUsedParticles = 0;
}
void ParticleRenderer::CreateBuffers() {
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);
    glGenBuffers(1, &mPositionBuffer);
    glGenBuffers(1, &mColorBuffer);
    glGenBuffers(1, &mSizeBuffer);
    glGenBuffers(1, &mClipBuffer);
    glGenBuffers(1, &mTextureBuffer);
    ResizeBuffer(mBufferSize);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
    glVertexAttribIPointer(4, 1, GL_INT, 0, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}
void ParticleRenderer::BufferData() {
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec3) * mUsedParticles,
                    &mPositions[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec4) * mUsedParticles,
                    &mColors[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(GLfloat) * mUsedParticles,
                    &mSizes[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec2) * mUsedParticles,
                    &mTextureOffsets[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(GLint) * mUsedParticles,
                    &mTextureIDs[0]);
}
void ParticleRenderer::ResizeBuffer(int size) {
    mParticlePositions.resize(size, glm::vec3(0));
    mSizes.resize(size, 0);
    mColors.resize(size, glm::vec4(0));
    mPositions.resize(size, glm::vec3(0));
    mTextureOffsets.resize(size, glm::vec2(0));
    mTextureIDs.resize(size, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * mPositions.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * mSizes.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec4) * mColors.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec2) * mTextureOffsets.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLint) * mTextureIDs.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
}
void ParticleRenderer::DrawParticle(Particle& part, glm::vec4 color, glm::vec3 offset, int id) {
    if(mUsedParticles >= mBufferSize) {
        ResizeBuffer(mBufferSize * 2);
        mBufferSize*= 2;
    }
    float alpha = part.GetAlpha();
    if(alpha < 0.01) {
        return;
    }
    color.a = part.GetAlpha();
    mColors[mUsedParticles] = color;
    mPositions[mUsedParticles] = offset + part.GetPosition();
    mPositions[mUsedParticles].x*= mNorm.x;
    mPositions[mUsedParticles].y*= mNorm.y;
    glm::vec3 pos = mPositions[mUsedParticles];
    mSizes[mUsedParticles] = part.GetSize();
    glm::vec2 texOffsets = part.GetTextureOffsets();
    mTextureOffsets[mUsedParticles] = part.GetTextureOffsets();
    mTextureIDs[mUsedParticles] = id;
    mUsedParticles++;
}
void ParticleRenderer::Render(ShaderProgram& sh) {
    if(!glIsVertexArray(mVao) || !mUsedParticles || !mBufferSize) {
        return;
    }
    glUseProgram(0);
    //glBlendFunc(mBlendSrc, mBlendDest);
    BufferData();
    sh.UseProgram();
    sh.SetUniform1i("textures", 0);
    sh.SetUniform2f("clipSize", mClip.x, mClip.y);
    glDrawArrays(GL_POINTS, 0, mUsedParticles);
    glUseProgram(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}
void ParticleRenderer::Render(ShaderProgram& sh, Texture& texture) {
    texture.Bind();
    Render(sh);
}
}

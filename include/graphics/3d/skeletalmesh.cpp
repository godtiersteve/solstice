#include "skeletalmesh.h"
namespace solstice {
void SkeletalMesh::ClearBuffers() {
    if(glIsBuffer(mVertexBuffer.vao)) {
        glDeleteVertexArrays(1, &mVertexBuffer.vao);
    }
    if(glIsBuffer(mVertexBuffer.vbo)) {
        glDeleteBuffers(1, &mVertexBuffer.vbo);
    }
    if(glIsBuffer(mVertexBuffer.ebo)) {
        glDeleteBuffers(1, &mVertexBuffer.ebo);
    }
}
void SkeletalMesh::BindBuffers() {
    glBindVertexArray(mVertexBuffer.vao);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffer.ebo);
    //glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer.vbo);
}
void SkeletalMesh::SetupMesh() {
    if(mVertices.empty()) {
        return;
    }
    glGenBuffers(1, &mVertexBuffer.vbo);
    glGenBuffers(1, &mVertexBuffer.ebo);
    glGenVertexArrays(1, &mVertexBuffer.vao);

    glBindVertexArray(mVertexBuffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(SkelVertex3f), &mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffer.ebo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkelVertex3f), (GLvoid*)offsetof(SkelVertex3f, position));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SkelVertex3f), (GLvoid*)offsetof(SkelVertex3f, color));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SkelVertex3f), (GLvoid*)offsetof(SkelVertex3f, normal));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SkelVertex3f), (GLvoid*)offsetof(SkelVertex3f, texCoord));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SkelVertex3f), (GLvoid*)offsetof(SkelVertex3f, jointIDs));
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(SkelVertex3f), (GLvoid*)offsetof(SkelVertex3f, jointWeights));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void SkeletalMesh::DrawVao() {
    if(mBackFaceCull) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    if(mNoDepth) {
        glDisable(GL_DEPTH_TEST);
    }
    glBindVertexArray(mVertexBuffer.vao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    if(mBackFaceCull) {
        glDisable(GL_CULL_FACE);
    }
    if(mNoDepth) {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
}
void SkeletalMesh::DrawOutline(ShaderProgram& sh, TextureArray& textures) {
    if(mHidden || mOutlineSize <= 0.001f) {
        return;
    }
    textures.BindTexture(mTextureID);
    sh.SetUniform1i("tex", 0);
    sh.SetUniform1f("outlineWidth", mOutlineSize);
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    sh.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
    glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glBindVertexArray(mVertexBuffer.vao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void SkeletalMesh::DrawOutline(ShaderProgram& shader) {
    shader.SetUniform1f("outlineWidth", mOutlineSize);
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    shader.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glBindVertexArray(mVertexBuffer.vao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
}

#include "vertexbuffer.h"

namespace solstice {
size_t VertexBuffer::mBoundBuffer = -1;
size_t VertexBuffer::mNumBuffers = 0;
VertexBuffer::VertexBuffer(size_t bufferID) {
    numAttribs = 0;
    timer = 0;
    bufferMode = GL_STATIC_DRAW;
    vbo = 0;
    vao = 0;
    ebo = 0;
}
/*
VertexBuffer::~VertexBuffer() {
    //mNumBuffers--;
}
*/
/*
VertexBuffer::VertexBuffer(const VertexBuffer& buffer) {
    vbo = buffer.vbo;
    ebo = buffer.ebo;
    vao = buffer.vao;
    mBufferID = buffer.mBufferID;
    mIndices = buffer.mIndices;

}
*/
void VertexBuffer::Clear() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    numAttribs = 0;
    mIndices.clear();
}
void VertexBuffer::SetAttrib2D(GLfloat* vertices,  int vtAmt, GLfloat* texcoords, int txAmt, const GLuint* indices, int ixAmt) {
    mIndices.clear();
    for(int i = 0; i < ixAmt; i++) {
        mIndices.push_back(indices[i]);
    }
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], bufferMode);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vtAmt + sizeof(texcoords) * txAmt, NULL, bufferMode);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(vertices) * vtAmt,
                    vertices);
    glBufferSubData(GL_ARRAY_BUFFER,
                    sizeof(vertices) * vtAmt,
                    sizeof(texcoords) * txAmt,
                    texcoords);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(vertices) * vtAmt));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    numAttribs = 2;
}
void VertexBuffer::Buffer3D(const std::vector<Vertex3f>& vertices, const std::vector<GLuint>& indices) {
    glBindVertexArray(vao);
    mIndices = indices;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3f), &vertices[0], bufferMode);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], bufferMode);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    numAttribs = 4;
}
void VertexBuffer::Buffer3D(const std::vector<Vertex3f>& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3f), &vertices[0], bufferMode);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VertexBuffer::Buffer3D(const std::vector<GLuint>& indices) {
    mIndices = indices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], bufferMode);
}
void VertexBuffer::SetAttrib2D(const std::vector<Vertex2f>& vertices, const std::vector<GLuint>& indices) {
    mIndices = indices;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], bufferMode);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex2f) * vertices.size(),
                 NULL,
                 bufferMode);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offsetof(Vertex2f, texCoord));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    numAttribs = 2;
}

void VertexBuffer::SetAttrib3D(const std::vector<Vertex3f>& vertices, const std::vector<GLuint>& indices) {
    mIndices = indices;
    if(!glIsVertexArray(vao)) {
        glGenVertexArrays(1, &vao);
    }
    if(!glIsBuffer(vbo)) {
        glGenBuffers(1, &vbo);
    }
    if(!glIsBuffer(ebo)) {
        glGenBuffers(1, &ebo);
    }
    numAttribs = 4;
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3f), &vertices[0], bufferMode);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], bufferMode);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3f), (GLvoid*)offsetof(Vertex3f, position));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3f), (GLvoid*)offsetof(Vertex3f, color));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3f), (GLvoid*)offsetof(Vertex3f, normal));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3f), (GLvoid*)(offsetof(Vertex3f, texCoord)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void VertexBuffer::BindBuffers() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
}
void VertexBuffer::UnbindBuffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void VertexBuffer::Draw(GLenum type) {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void VertexBuffer::DrawInstanced(int count) {
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}
void VertexBuffer::Draw2(GLenum type) {
    for(int i = 0; i < numAttribs; i++) {
        glEnableVertexAttribArray(i);
    }
    if(numAttribs == 2) {
        glDrawElements(type, mIndices.size(), GL_UNSIGNED_INT, &mIndices[0]);
    }
    else {
        glDrawElements(type, mIndices.size(), GL_UNSIGNED_INT, 0);
    }
}
}

#ifndef VERTEXBUFFER_H_INCLUDED
#define VERTEXBUFFER_H_INCLUDED

#include "graphicglobals.h"
#include "vertex2.h"
#include "vertex3.h"
#include <iostream>
#include <vector>
#include <iterator>
namespace solstice {
class VertexBuffer {
private:
    std::vector<GLuint> mIndices;
    static size_t mBoundBuffer;
    static size_t mNumBuffers;
    size_t mBufferID;
public:
    GLuint vbo, ebo, vao;
    float timer;
    GLenum bufferMode;
    int numAttribs;
    VertexBuffer(size_t bufferID = -1);
    //~VertexBuffer();
    //VertexBuffer(const VertexBuffer& buffer);
    void SetAttrib2D(GLfloat* vertices,  int vtAmt, GLfloat* texcoords, int txAmt, const GLuint* indices = SOLSTICE_RENDER_TRIANGLES, int ixAmt = 6);
    void SetAttrib2D(const std::vector<Vertex2f>& vertices, const std::vector<GLuint>& indices);
    void SetAttrib3D(const std::vector<Vertex3f>& vertices, const std::vector<GLuint>& indices);
    void BindBuffers();
    void UnbindBuffers();
    void Draw(GLenum type = GL_TRIANGLES);
    void Draw2(GLenum type = GL_TRIANGLES);
    void DrawInstanced(int count);
    void DrawElementsInstanced(int count) {glDrawElementsInstanced(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0, count);}
    void Clear();
    bool IsVertexBuffer() {return glIsBuffer(vbo) && glIsBuffer(ebo) && glIsVertexArray(vao);}
    void Buffer3D(const std::vector<Vertex3f>& vertices, const std::vector<GLuint>& indices);
    void Buffer3D(const std::vector<Vertex3f>& vertices);
    void Buffer3D(const std::vector<GLuint>& indices);
    std::vector<GLuint>& GetIndices()               {return mIndices;}
    void BindVAO()                                  {glBindVertexArray(vao);}
    size_t GetBufferID()                            {return mBufferID;}
};

}

#endif // VERTEXBUFFER_H_INCLUDED

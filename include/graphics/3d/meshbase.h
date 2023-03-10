#ifndef MESHBASE_H_INCLUDED
#define MESHBASE_H_INCLUDED

#include "graphics/vector2.h"
#include "graphics/vertex3.h"
#include "graphics/shaderprogram.h"
#include "graphics/texarray.h"
#include "graphics/color.h"
#include "graphics/vertexbuffer.h"
#include "geometry/vertex3.h"
#include "geometry/polygon.h"

#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <map>
namespace solstice {
template<class T>
class MeshBase {
protected:
    VertexBuffer mVertexBuffer;
    void CalculateNormals(size_t i);
    vector<T> mVertices;
    vector<GLuint> mIndices;
    GLuint mLastIndex;
    ModelTransform mTransform;
    glm::mat4 mInverseTransform;
    string mName;
    bool mHidden;
    float mOutlineSize;
    int mTextureID;
    bool mBackFaceCull, mNoDepth;
public:
    MeshBase();
    MeshBase(vector<T>& vt);
    MeshBase(vector<T> vt, vector<GLuint> id);

    void AddVertices(vector<Vertex3f> verts);
    virtual void Bind();
    virtual void DrawVao();
    virtual void Draw(ShaderProgram& shader, glm::mat4& modelTrans);
    virtual void DrawBatch(ShaderProgram& shader, TextureArray& textures, std::vector<glm::mat4> transforms);
    virtual void Draw(ShaderProgram& shader);
    virtual void Draw(ShaderProgram& shader, TextureArray& textures);
    void DrawOutline(ShaderProgram& shader);
    void DrawOutline(ShaderProgram& shader, TextureArray& textures);
    void SetupMesh();
    void ClearMesh();
    void GenIndices();
    void CalculateNormals();
    void SetHidden(bool h)                  {mHidden = h;}
    void SetOutlineSize(float s)            {mOutlineSize = s;}
    void SetName(string n)                  {mName = n;}
    void SetTextureID(int i)                {mTextureID = i;}
    bool GetHidden()                        {return mHidden;}
    float GetOutlineSize()                  {return mOutlineSize;}
    string GetName()                        {return mName;}
    size_t GetTextureID()                   {return mTextureID;}
    void UpdateMeshVerts()                  {mVertexBuffer.Buffer3D(mVertices);}
    void UpdateMeshIndices()                {mVertexBuffer.Buffer3D(mIndices);}
    VertexBuffer& GetVertexBuffer()         {return mVertexBuffer;}
    void UpdateMesh()                       {UpdateMeshVerts(); UpdateMeshIndices();}
    ModelTransform& GetTransform()          {return mTransform;}
    std::vector<T>& GetVertices()           {return mVertices;}
    std::vector<GLuint>& GetIndices()       {return mIndices;}
    T& GetVertex(size_t i)                  {return mVertices[i];}
    GLuint& GetIndex(size_t i)              {return mIndices[i];}
    void AddVertex(T vt)                    {mVertices.push_back(vt);}
    GLuint GetLastIndex()                   {return mLastIndex;}
    glm::mat4& GetInverseTrans()            {return mInverseTransform;}
    bool GetBackfaceCull()                  {return mBackFaceCull;}
    void SetBackfaceCull(bool f)            {mBackFaceCull = f;}
    void SetNoDepth(bool f)                 {mNoDepth = f;}
    bool GetNoDepth()                       {return mNoDepth;}
    virtual bool HasBones()                 {return false;}
    bool operator>(MeshBase<T>& m)          {return mName > m.GetName();}
    bool operator<(MeshBase<T>& m)          {return mName < m.GetName();}
    void SetTranslation(float x, float y, float z) {mTransform.SetTranslation(glm::vec3(x, y, z));}
    void SetScale(float x, float y, float z) {mTransform.SetScale(glm::vec3(x, y, z));}
    void AddIndex(GLuint i);
    virtual void Write(std::ofstream& file);
    virtual void Read(std::ifstream& file);
    virtual void SetUniforms(ShaderProgram& sh);
    virtual void SetUniforms(ShaderProgram& sh, glm::mat4 modelTrans);
};
template<class T>
MeshBase<T>::MeshBase() {
    mLastIndex = 0;
    mHidden = false;
    mBackFaceCull = false;
    mNoDepth = false;
    mOutlineSize = 0.0f;
    mTextureID = -1;
}
template<class T>
MeshBase<T>::MeshBase(vector<T>& vt) {
    mVertices = vt;
    mLastIndex = 0;
    mOutlineSize = 0.0f;
    mHidden = false;
    mBackFaceCull = false;
    mNoDepth = false;
    GenIndices();
    mTextureID = -1;
}
template<class T>
MeshBase<T>::MeshBase(vector<T> vt, vector<GLuint> id) {
    mVertices = vt;
    mIndices = id;
    mOutlineSize = 0.0f;
    mHidden = false;
    mBackFaceCull = false;
    mNoDepth = false;
    mLastIndex = 0;
    mTextureID = -1;
}
template<class T>
void MeshBase<T>::AddVertices(vector<Vertex3f> verts) {
    for(auto vt : verts) {
        mVertices.push_back(vt);
    }
}
template<class T>
void MeshBase<T>::AddIndex(GLuint i) {
    if(i > mLastIndex) {
        mLastIndex = i;
    }
    mIndices.push_back(i);
}
template<class T>
void MeshBase<T>::Bind() {
    mVertexBuffer.BindBuffers();
}
template<class T>
void MeshBase<T>::DrawVao() {
    if(mBackFaceCull) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    if(mNoDepth) {
        glDisable(GL_DEPTH_TEST);
    }
    mVertexBuffer.Draw();
    if(mBackFaceCull) {
        glDisable(GL_CULL_FACE);
    }
    if(mNoDepth) {
        glEnable(GL_DEPTH_TEST);
    }
}
template<class T>
void MeshBase<T>::Draw(ShaderProgram& shader) {
    if(mHidden) {
        return;
    }
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    shader.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
    DrawVao();
}
template<class T>
void MeshBase<T>::Draw(ShaderProgram& shader, TextureArray& textures) {
    if(mHidden) {
        return;
    }
    textures.BindTexture(mTextureID);
    SetUniforms(shader);
    DrawVao();
}
template<class T>
void MeshBase<T>::Draw(ShaderProgram& shader, glm::mat4& modelTrans) {
    if(mHidden) {
        return;
    }
    SetUniforms(shader, modelTrans);
    DrawVao();
}
template<class T>
void MeshBase<T>::SetUniforms(ShaderProgram& sh) {
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    sh.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
}
template<class T>
void MeshBase<T>::SetUniforms(ShaderProgram& sh, glm::mat4 modelTrans) {
    sh.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(modelTrans * mInverseTransform));
}
template<class T>
void MeshBase<T>::DrawBatch(ShaderProgram& shader, TextureArray& textures, std::vector<glm::mat4> transforms) {
}
template<class T>
void MeshBase<T>::DrawOutline(ShaderProgram& shader) {
    if(mHidden || mOutlineSize < 0.001f) {
        return;
    }
    shader.SetUniform1f("outlineWidth", mOutlineSize);
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    shader.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
    mVertexBuffer.Draw();
}
template<class T>
void MeshBase<T>::DrawOutline(ShaderProgram& shader, TextureArray& textures) {
    if(mHidden || mOutlineSize < 0.001f) {
        return;
    }
    //textures.BindTexture(mTextureID);
    shader.SetUniform1i("tex", 0);
    shader.SetUniform1f("outlineWidth", mOutlineSize);
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    shader.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
    mVertexBuffer.Draw();
}
template<class T>
void MeshBase<T>::SetupMesh() {
    if(mVertices.empty()) {
        return;
    }
    mVertexBuffer.SetAttrib3D(mVertices, mIndices);
}
template<class T>
void MeshBase<T>::ClearMesh() {
    mVertexBuffer.Clear();
    mVertices.clear();
    mIndices.clear();
}
template<class T>
void MeshBase<T>::GenIndices() {
    mIndices.clear();
    GLuint index = 0;
    for(size_t i = 0; i < mVertices.size(); i++) {
        Vertex3f& vt = mVertices[i];
        auto it = std::find(mVertices.begin(), mVertices.end(), vt);
        if(it == mVertices.end()) {
            mIndices.push_back(index++);
            mLastIndex = index;
        }
        else {
            mIndices.push_back(it->index);
        }
    }
}
template<class T>
void MeshBase<T>::CalculateNormals() {
    for(size_t i = 0; i < mIndices.size(); i+=3) {
        T& vert = mVertices[mIndices[i]];
        glm::vec3 current = vert.position;
        glm::vec3 next = mVertices[mIndices[i + 1]].position;
        vert.normal.x = current.y - next.y * current.z + next.z;
        vert.normal.y = current.z - next.z * current.x + next.x;
        vert.normal.z = current.x - next.x * current.y * next.y;
    }
}
template<class T>
void MeshBase<T>::Write(std::ofstream& file) {
    BinaryWriteString(file, mName);
    BinaryWrite(file, mHidden);
    BinaryWrite(file, mOutlineSize);
    BinaryWrite(file, mTextureID);
    BinaryWrite(file, mBackFaceCull);
    BinaryWrite(file, mNoDepth);
    BinaryWrite(file, mVertices.size());
    std::cout << "Num Verts: " << mVertices.size() << std::endl;
    for(size_t i = 0; i < mVertices.size(); i++) {
        if(i >= 95) {
            //std::cout << "whatever" << std::endl;
        }
        //mVertices[i].Write(file);
    }
    //T vt = mVertices.back();
    BinaryWrite(file, mIndices.size());
    std::cout << "Num indices: " << mIndices.size() << std::endl;
    for(auto id : mIndices) {
        BinaryWrite(file, id);
    }
    BinaryWriteMatrix(file, mInverseTransform);
    mTransform.Write(file);
}
template<class T>
void MeshBase<T>::Read(std::ifstream& file) {
    BinaryReadString(file, mName);
    std::cout << mName << std::endl;
    BinaryRead(file, mHidden);
    BinaryRead(file, mOutlineSize);
    BinaryRead(file, mTextureID);
    BinaryRead(file, mBackFaceCull);
    BinaryRead(file, mNoDepth);
    size_t numVertices;
    BinaryRead(file, numVertices);
    mVertices.reserve(numVertices);
    for(size_t i = 0; i < numVertices; i++) {
        T vt;
        //vt.Read(file);
        if(i >= 90) {
            std::cout << "whatever" << std::endl;
        }
        mVertices.push_back(vt);
    }
    std::cout << "Num Verts: " << mVertices.size() << std::endl;
    //T vt = mVertices.back();
    size_t numIndices;
    BinaryRead(file, numIndices);
    mIndices.clear();
    std::cout << "Num indices: " << mIndices.size() << std::endl;
    mIndices.reserve(numIndices);
    for(size_t i = 0; i < numIndices; i++) {
        GLuint index;
        BinaryRead(file, index);
        mIndices.push_back(index);
    }
    BinaryReadMatrix(file, mInverseTransform);
    mTransform.Read(file);
}
}

#endif // MESHBASE_H_INCLUDED

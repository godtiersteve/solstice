#ifndef MODELBASE_H_INCLUDED
#define MODELBASE_H_INCLUDED

#include <vector>
#include <string>
#include <algorithm>

#include "graphics/vector2.h"
#include "graphics/vertex3.h"
#include "graphics/shaderprogram.h"
#include "graphics/texarray.h"
#include "graphics/color.h"
#include "graphics/vertexbuffer.h"
#include "geometry/vertex3.h"
#include "geometry/polygon.h"

namespace solstice {
template<class T>
class ModelBase {
protected:
    vector<T> mMeshes;
    Color mOutlineColor;
    ModelTransform mTransform;
    std::string mName;
    bool mHidden;
public:
    ModelBase();
    virtual void Draw(ShaderProgram& shader);
    virtual void Draw(ShaderProgram& shader, TextureArray& textures);
    virtual void DrawInternalTransform(ShaderProgram& shader, TextureArray& textures);
    virtual void Draw(ShaderProgram& shader, TextureArray& texArr, glm::mat4& trans);
    virtual void Draw(ShaderProgram& shader, glm::mat4& trans);
    virtual void DrawOutline(ShaderProgram& shader);
    virtual void DrawOutline(ShaderProgram& shader, TextureArray& textures);
    //virtual void DrawInstanced(ShaderProgram& shader, std::vector<glm::vec3> positions);

    T& GetMesh(string s);
    virtual void SetAngle(float angle);
    virtual void SetScale(glm::vec3 scale);
    virtual void SetAngle(float x, float y, float z);
    virtual void SetRotation(glm::vec3 rotation);
    virtual void SetRotation(float xrot, float yrot, float zrot);
    virtual void SetTranslation(glm::vec3 translation);
    virtual void SetTranslation(float x, float y, float z)          {SetTranslation(glm::vec3(x, y, z));}
    virtual void SetScale(float x, float y, float z)                {SetScale(glm::vec3(x, y, z));}
    virtual void SetScale(float x)                                  {SetScale(glm::vec3(x, x, x));}
    virtual void SetTransform(ModelTransform t);

    void SetName(std::string s)                             {mName = s;}
    std::string GetName()                                   {return mName;}

    void AddMesh(T mesh)                                    {mMeshes.push_back(mesh);}
    void SetOutlineSize(float s)                            {for(auto& mesh : mMeshes) {mesh.SetOutlineSize(s);}}
    vector<T>& GetMeshes()                                  {return mMeshes;}
    ModelTransform& GetTransform()                          {return mTransform;}
    T& GetMesh(size_t i)                                    {return mMeshes[i];}
    T* GetMeshPtr(size_t i)                                 {return i >= 0 && i < mMeshes.size() ? &mMeshes[i] : NULL;}
    T* GetMeshPtr(std::string name);
    void SetTextureID(size_t i)                             {for(auto& mesh : mMeshes) {mesh.SetTextureID(i);}}
    ModelBase& SetupModel();
    void SetOutlineColor(float r, float g, float b)         {mOutlineColor = Color(r, g, b, 1.0f);}
    Color& GetOutlineColor()                                {return mOutlineColor;}
    void SetHidden(float h)                                 {mHidden = h;}
    void SetBackfaceCull(bool cull);
    bool GetHidden()                                        {return mHidden;}
    bool Empty()                                            {return mMeshes.empty();}
    void SortMeshes();
    void AddVertices(T mesh);
    void clear();
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
    virtual void Unproject(glm::vec3 pos, glm::mat4& trans, glm::mat4 projection, glm::vec4 viewport);
};
template<class T>
ModelBase<T>::ModelBase() {
    mHidden = false;
}
template<class T>
void ModelBase<T>::Draw(ShaderProgram& shader) {
    if(mHidden) {
        return;
    }
    for(auto& mesh : mMeshes) {
        mesh.Draw(shader);
    }
}
template<class T>
void ModelBase<T>::Draw(ShaderProgram& shader, TextureArray& textures) {
    if(mHidden) {
        return;
    }
    for(auto& mesh : mMeshes) {
        mesh.Draw(shader, textures);
    }
}
template<class T>
void ModelBase<T>::DrawInternalTransform(ShaderProgram& shader, TextureArray& textures) {
    if(mHidden) {
        return;
    }
    glm::mat4 trans = mTransform.GetTransform();
    for(auto& mesh : mMeshes) {
        textures.BindTexture(mesh.GetTextureID());
        mesh.Draw(shader, trans);
    }
}
template<class T>
void ModelBase<T>::Draw(ShaderProgram& shader, TextureArray& texArr, glm::mat4& trans) {
    if(mHidden) {
        return;
    }
    for(auto& mesh : mMeshes) {
        texArr.BindTexture(mesh.GetTextureID());
        mesh.Draw(shader, trans);
    }
}
template<class T>
void ModelBase<T>::Draw(ShaderProgram& shader, glm::mat4& trans) {
    if(mHidden) {
        return;
    }
    for(auto& mesh : mMeshes) {
        mesh.Draw(shader, trans);
    }
}
template<class T>
void ModelBase<T>::DrawOutline(ShaderProgram& shader) {
    if(mHidden) {
        return;
    }
    for(auto& mesh : mMeshes) {
        mesh.DrawOutline(shader);
    }
}
template<class T>
void ModelBase<T>::DrawOutline(ShaderProgram& shader, TextureArray& textures) {
    if(mHidden) {
        return;
    }
    for(auto& mesh : mMeshes) {
        mesh.DrawOutline(shader, textures);
    }
}
template<class T>
T& ModelBase<T>::GetMesh(string s) {
    for(auto& mesh : mMeshes) {
        if(mesh.GetName() == s) {
            return mesh;
        }
    }
    return mMeshes.front();
}
template<class T>
T* ModelBase<T>::GetMeshPtr(string s) {
    for(size_t i = 0; i < mMeshes.size(); i++) {
        if(mMeshes[i].GetName() == s) {
            return &mMeshes[i];
        }
    }
    return NULL;
}
template<class T>
void ModelBase<T>::SetAngle(float angle) {
    mTransform.SetRotation(angle, 0, 0);
    for(auto& mesh : mMeshes) {
        mesh.GetTransform().SetRotation(angle, 0, 0);
    }
}
template<class T>
void ModelBase<T>::SetAngle(float x, float y, float z) {
    mTransform.SetAngles(x, y, z);
    for(auto& mesh : mMeshes) {
        mesh.GetTransform().SetAngles(x, y, z);
    }
}
template<class T>
void ModelBase<T>::SetScale(glm::vec3 scale) {
    mTransform.SetScale(scale);
    for(auto& mesh : mMeshes) {
        mesh.GetTransform().SetScale(scale);
    }
}
template<class T>
void ModelBase<T>::SetRotation(glm::vec3 rotation) {
    mTransform.SetRotation(rotation);
    for(auto& mesh : mMeshes) {
        mesh.GetTransform().SetRotation(rotation);
    }
}
template<class T>
void ModelBase<T>::SetRotation(float xrot, float yrot, float zrot) {
    mTransform.SetRotation(xrot, yrot, zrot);
    for(auto& m : mMeshes) {
        m.GetTransform().SetRotation(xrot, yrot, zrot);
    }
}
template<class T>
void ModelBase<T>::SetTranslation(glm::vec3 translation) {
    mTransform.SetTranslation(translation);
    for(auto& mesh : mMeshes) {
        mesh.GetTransform().SetTranslation(translation);
    }
}
template<class T>
void ModelBase<T>::SetTransform(ModelTransform t) {
    mTransform = t;
    for(auto& m : mMeshes) {
        m.GetTransform() = t;
    }
}
template<class T>
void ModelBase<T>::SetBackfaceCull(bool cull) {
    for(auto& mesh : mMeshes) {
        mesh.SetBackfaceCull(cull);
    }
}
template<class T>
void ModelBase<T>::clear() {
    if(!mMeshes.empty()) {
        for(auto& m : mMeshes) {
            m.ClearMesh();
        }
    }
    mMeshes.clear();
}
template<class T>
void ModelBase<T>::Unproject(glm::vec3 pos, glm::mat4& trans, glm::mat4 projection, glm::vec4 viewport) {
    pos = glm::unProject(pos, trans, projection, viewport);
    mTransform.SetTranslation(pos);
}
template<class T>
void ModelBase<T>::SortMeshes() {
    std::sort(mMeshes.begin(), mMeshes.end());
}
template<class T>
ModelBase<T>& ModelBase<T>::SetupModel() {
    for(auto& mesh : mMeshes) {
        mesh.SetupMesh();
    }
    return *this;
}
template<class T>
void ModelBase<T>::AddVertices(T mesh) {
    if(mMeshes.empty()) {
        AddMesh(mesh);
    }
    else {
        T& m = mMeshes[0];
        for(auto vt : mesh.GetVertices()) {
            m.AddVertex(vt);
        }
        int ind = m.GetLastIndex() + 1;
        for(auto i : mesh.GetIndices()) {
            m.AddIndex(i + ind);
        }
    }
}
template<class T>
void ModelBase<T>::Write(std::ofstream& file) {
    BinaryWrite(file, mMeshes.size());
    for(auto& mesh : mMeshes) {
        mesh.Write(file);
    }
    BinaryWrite(file, mOutlineColor.red);
    BinaryWrite(file, mOutlineColor.green);
    BinaryWrite(file, mOutlineColor.blue);
    BinaryWrite(file, mOutlineColor.alpha);
    mTransform.Write(file);
    BinaryWriteString(file, mName);
    BinaryWrite(file, mHidden);
}
template<class T>
void ModelBase<T>::Read(std::ifstream& file) {
    size_t numMeshes;
    BinaryRead(file, numMeshes);
    mMeshes.resize(numMeshes);
    for(auto& mesh : mMeshes) {
        mesh.Read(file);
    }
    BinaryRead(file, mOutlineColor.red);
    BinaryRead(file, mOutlineColor.green);
    BinaryRead(file, mOutlineColor.blue);
    BinaryRead(file, mOutlineColor.alpha);
    mTransform.Read(file);
    BinaryReadString(file, mName);
    BinaryRead(file, mHidden);
}
}

#endif // MODELBASE_H_INCLUDED

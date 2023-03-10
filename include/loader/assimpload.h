#ifndef ASSIMPLOAD_H_INCLUDED
#define ASSIMPLOAD_H_INCLUDED

#include "utility/sol_math.h"
#include "graphics/3d/mesh.h"
#include "graphics/3d/model.h"
#include "graphics/3d/modelanim.h"
#include "graphics/3d/skeletalmodel.h"

#include "assimp/importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace solstice {
template<class T>
void LoadModel(ModelBase<T>* model, string file, TextureArray& textures, bool singleMesh = false);
template<class T>
void ProcessNode(aiNode* node, const aiScene* scene, ModelBase<T>* model, TextureArray& textures, bool singleMesh = false);
template<class T>
void LoadBones(const aiMesh* mesh, T& bmesh);
template<class T>
void LoadAnimation(const aiScene* scene, T& mesh);
template<class TMesh, class TVertex>
void ProcessMesh(aiMesh* mesh, const aiScene* scene, TextureArray& textures, TMesh& m);
template<class T>
void ReadNodeHierarchy(float animationTime, const aiScene* scene, const aiNode* node, const glm::mat4& parentTransform, T& mesh);
Model LoadModel(string file, TextureArray& textures);
SkeletalModel LoadSkelModel(string file, TextureArray& textures);
void CalcInterpolatedRotation(aiQuaternion& out, float animTime, const aiNodeAnim* nodeAnim);
void FindNodeAnim(const aiAnimation* animation, string nodeName);
glm::mat4 aiToGLMat4(aiMatrix4x4 mat);

template<class T>
void LoadModel(ModelBase<T>* model, string file, TextureArray& textures, bool singleMesh) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file,
                                            aiProcess_CalcTangentSpace       |
                                            aiProcess_Triangulate            |
                                            aiProcess_JoinIdenticalVertices  |
                                            aiProcess_SortByPType            |
                                            aiProcess_GenSmoothNormals);
    if(!scene) {
        std::cout << "Unable to read file " << file << std::endl;
        return;
    }
    else {
        std::cout << "Loading model: " << file << std::endl;
    }
    std::string path;
    size_t last = file.find_last_of("\\");
    if(last == string::npos) {
        last = file.find_last_of("/");
    }
    if(last != string::npos) {
        path = file.substr(0, last + 1);
    }
    std::string oldPath = textures.GetPath();
    textures.SetPath(path);
    ProcessNode(scene->mRootNode, scene, model, textures, false);
    textures.SetPath(oldPath);
}
template<class T>
void ProcessNode(aiNode* node, const aiScene* scene, ModelBase<T>* model, TextureArray& textures, bool singleMesh) {
    for(size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        T nMesh;
        nMesh.SetName(mesh->mName.data);
        ProcessMesh(mesh, scene, textures, nMesh);
        if(mesh->HasBones()) {
            LoadBones(mesh, nMesh);
        }
        nMesh.GetInverseTrans() = glm::inverse(glm::transpose(aiToGLMat4(scene->mRootNode->mTransformation)))
                                * aiToGLMat4(node->mTransformation);
        if(!singleMesh) {
            model->AddMesh(nMesh);
        }
        else {
            model->AddVertices(nMesh);
        }
        //std::cout << "Loading mesh: " << mesh->mName.data << std::endl;
    }
    for(size_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, model, textures);
    }
}
template<class TMesh>
void ProcessMesh(aiMesh* mesh, const aiScene* scene, TextureArray& textures, TMesh& m) {
    aiString texRef;
    int textureID = -1;
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        material->Get(AI_MATKEY_NAME, texRef);
        for(size_t i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, i, &str);
            textureID = textures.LoadImg(str.C_Str());
        }
    }
    m.SetTextureID(textureID);
    for(size_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex3f vt;
        vt.position.x = mesh->mVertices[i].x;
        vt.position.y = mesh->mVertices[i].y;
        vt.position.z = mesh->mVertices[i].z;
        vt.normal.x = mesh->mNormals[i].x;
        vt.normal.y = mesh->mNormals[i].y;
        vt.normal.z = mesh->mNormals[i].z;
        if(mesh->HasVertexColors(i)) {
            vt.color.red = mesh->mColors[i]->r;
            vt.color.green = mesh->mColors[i]->g;
            vt.color.blue = mesh->mColors[i]->b;
            vt.color.alpha = mesh->mColors[i]->a;
        }
        if(mesh->mTextureCoords[0]) {
            vt.texCoord.x = mesh->mTextureCoords[0][i].x;
            vt.texCoord.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            vt.texCoord.x = -1000;
            vt.texCoord.y = -1000;
        }
        m.AddVertex(vt);
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++) {
            m.AddIndex(face.mIndices[j]);
        }
    }
}

template<class T>
void LoadBones(const aiMesh* mesh, T& bmesh) {
    /*
    if(!bmesh.HasBones()) {
        return;
    }
    for(size_t i = 0; i < mesh->mNumBones; i++) {
        unsigned int boneIndex = 0;
        string boneName = mesh->mBones[i]->mName.data;
        std::map<string, unsigned int>& mapping = bmesh.GetBoneMap();
        auto mapID = mapping.find(boneName);
        if(mapID == mapping.end()) {
            boneIndex = mapping.size();
            mapping.insert(std::pair<string, unsigned int>(boneName, mapping.size()));
        }
        else {
            boneIndex = mapping[boneName];
        }
        mapping[boneName] = boneIndex;
        for(size_t j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
            size_t vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            Vertex3f* vt = &bmesh.GetVertex(vertexID);
            //std::cout << "vertex: " << vertexID << " bone index: " << boneIndex << " : " << weight << std::endl;
            vt->AddWeight(boneIndex, weight);
        }
        //aiBone* bone = mesh->mBones[i];
        //Joint bone;
        //bone.transform = mesh->mBones[i].mOffsetMatrix;
        //bmesh.AddJointsToArray(bone, )
    }
    */
}
template<class T>
void LoadAnimation(const aiScene* scene, T& mesh) {
    glm::mat4 identity(1.0f);
    float ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ?
                           scene->mAnimations[0]->mTicksPerSecond : 25.0f;
    float timeInTicks = ticksPerSecond;
    float animationTime = fmod(timeInTicks, scene->mAnimations[0]->mDuration);

    ReadNodeHierarchy(animationTime, scene, scene->mRootNode, identity, mesh);
    for(size_t i = 0; i < mesh.GetNumBones(); i++) {
        //mesh.GetTransform(i) =
    }
}

template<class T>
void ReadNodeHierarchy(float animationTime, const aiScene* scene, const aiNode* node, const glm::mat4& parentTransform, T& mesh) {
    string nodeName = node->mName.data;
    const aiAnimation* animation = scene->mAnimations[0];
    glm::mat4 nodeTransformation = aiToGLMat4(node->mTransformation);
    const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);
    if(nodeAnim) {
        aiVector3D scaling;
        //CalcInterpoaltedScaling(scaling, animationTime, nodeAnim);
        glm::mat4 scalingMat = glm::scale(glm::vec3(scaling.x, scaling.y, scaling.z));

        aiQuaternion rotationQuat;
        //CalcInterpolatedRotation(rotationQuat, animationTime, nodeAnim);
        glm::mat4 rotationMat;
        //aiToGLMat4(rotationQuat.GetMatrix());

        aiVector3D translation;
        //CalcInterpolatedPosition(translation, animationTime, nodeAnim);
        glm::mat4 transMat = glm::translate(glm::vec3(translation.x, translation.y, translation.z));

        nodeTransformation = transMat * rotationMat * scalingMat;
    }
    glm::mat4 globalTransform = parentTransform * nodeTransformation;
    if(mesh.GetBoneMap().find(nodeName) != mesh.GetBoneMap().end()) {
        size_t boneIndex = mesh.GetBoneMap[nodeName];
        mesh.GetBone(boneIndex).finalTransform = mesh.GetGlobalInvTrans() * globalTransform;
    }
    for(size_t i = 0; i < node->mNumChildren; i++) {
        //ReadNodeHierarchy(animationTime, scene, node->mChildren[i], globalTransform);
    }

}
inline void FindNodeAnim(const aiAnimation* animation, string nodeName) {
}
inline void CalcInterpolatedRotation(aiQuaternion& out, float animTime, const aiNodeAnim* nodeAnim) {
    if(nodeAnim->mNumRotationKeys == 1) {
        out = nodeAnim->mRotationKeys[0].mValue;
    }
    //size_t rotIndex = FindRotation(animTime, nodeAnim);
}
inline Model LoadModel(string file, TextureArray& textures) {
    Model m;
    LoadModel(&m, file, textures);
    return m;
}
inline SkeletalModel LoadSkelModel(string file, TextureArray& textures) {
    SkeletalModel m;
    LoadModel(&m, file, textures);
    return m;
}
inline glm::mat4 aiToGLMat4(aiMatrix4x4 mat) {
    glm::mat4 tmp;
    tmp[0][0] = mat.a1;
    tmp[1][0] = mat.a2;
    tmp[2][0] = mat.a3;
    tmp[3][0] = mat.a4;

    tmp[0][1] = mat.b1;
    tmp[1][1] = mat.b2;
    tmp[2][1] = mat.b3;
    tmp[3][1] = mat.b4;

    tmp[0][2] = mat.c1;
    tmp[1][2] = mat.c2;
    tmp[2][2] = mat.c3;
    tmp[3][2] = mat.c4;

    tmp[0][3] = mat.d1;
    tmp[1][3] = mat.d2;
    tmp[2][3] = mat.d3;
    tmp[3][3] = mat.d4;
    return tmp;
}

}

#endif // ASSIMPLOAD_H_INCLUDED

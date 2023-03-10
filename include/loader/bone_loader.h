#ifndef BONE_LOADER_H_INCLUDED
#define BONE_LOADER_H_INCLUDED

#include <windows.h>

#include "assimp/importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/gtx/quaternion.hpp"

#include "assimpload.h"
#include "graphics/3d/skeletalmodel.h"
#include "utility/util.h"

namespace solstice {
class BoneLoader {
    std::vector<SkeletalModel> mModels;
    std::vector<aiNode*> mNodes;
    const aiScene* mCurrentScene;
    SkeletalModel* mCurrentModel;
    void ProcessNode(aiNode* node, TextureArray& textures, bool singleMesh = true);
    void ProcessMesh(const aiMesh* mesh, SkeletalMesh& m, TextureArray& textures);
    void LoadBonesModel(const aiMesh* mesh, SkeletalMesh& bmesh);
    void LoadAnimationModel(const aiNode* node, size_t frame);
    void LoadAnimation(ModelAnimation& anim, const aiNode* node, size_t frame);
    aiNode* GetNode(string str);
    aiBone* GetBone(const aiNode* node, string str);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* anim, string name);
    void GenNodes(aiNode* node);
    std::vector<Bone> mBones;
public:
    BoneLoader();
    SkeletalModel LoadModel(string file, TextureArray& textures, bool singleMesh = true, bool setup = false);
    void LoadAnimatedModels(std::string dir, std::string prefix, std::string ext, TextureArray& textures);
    void LoadAnimation(string file, string modelName);
    void AddAnimation(std::string file, std::string model, std::string animName);
    SkeletalModel& GetModel(size_t i) {return mModels[i];}
    SkeletalModel* GetModelPtr(int i) {return i >= 0 && i < mModels.size() && !mModels.empty() ? &mModels[i] : NULL;}
    SkeletalModel* GetModelPtr(std::string n);
    void ExportModels(std::string filename);
    void LoadBinary(std::string path);
    std::vector<SkeletalModel>& GetModels() {return mModels;}
    void AddModel(SkeletalModel m)          {mModels.push_back(m);}
    void clear();
};
void TrimNames(std::string str, std::string& name, std::string& group);
bool LoadModelAndAnimations(SkeletalModel& model, std::string path, std::string prefix, TextureArray& textures);
}

#endif // BONE_LOADER_H_INCLUDED

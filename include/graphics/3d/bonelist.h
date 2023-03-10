#ifndef BONELIST_H_INCLUDED
#define BONELIST_H_INCLUDED

#include "bone.h"

namespace solstice {
class BoneList {
    std::vector<Bone> mBones;
    std::vector<size_t> mRootBones;
    void InterpNextFrame();
    void UpdateBone(size_t i, glm::mat4& parent);
public:
    void Update(double t = 1.0f);
    int GetBoneID(string s);
    void AddBone(Bone b);
    void AddDeformBone(Bone b);
    void GenBones();
    void SetBoneTransforms(std::vector<Bone>& bones);
    std::vector<Bone>& GetBones()                       {return mBones;}
    Bone& GetBone(size_t i)                             {return mBones[i];}
    Bone* GetBone(string s);
    Bone& FindBone(string s);
    void MapBone(int i, int k, BoneList& list);
    void MapBoneTransRot(int i, int k, BoneList& list);
    void MapBones(BoneGroup& group, BoneList& list);
};

}

#endif // BONELIST_H_INCLUDED

#include "bonelist.h"

namespace solstice {
    void BoneList::Update(double t) {
    }
    void BoneList::InterpNextFrame() {
    }
    void BoneList::UpdateBone(size_t i, glm::mat4& parent) {
    }
    int BoneList::GetBoneID(string s) {
    }
    void BoneList::AddBone(Bone b) {
    }
    void BoneList::AddDeformBone(Bone b) {
    }
    void BoneList::GenBones() {
    }
    void BoneList::SetBoneTransforms(std::vector<Bone>& bones) {
    }
    Bone* BoneList::GetBone(string s) {
    }
    Bone& BoneList::FindBone(string s) {
    }
    void BoneList::MapBone(int i, int k, BoneList& list) {
    }
    void BoneList::MapBoneTransRot(int i, int k, BoneList& list) {
    }
    void BoneList::MapBones(BoneGroup& group, BoneList& list) {
    }
}



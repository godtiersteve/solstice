#include "animkeyframe.h"
namespace solstice {
    AnimKeyFrame::AnimKeyFrame() {
        timestamp = 0.0f;
    }
    AnimKeyFrame::AnimeKeyFrame(std::vector<Bone> b) {
        bones = b;
        timestamp = 0.0f;
    }
    AnimKeyFrame& AnimKeyFrame::operator=(const std::vector<Bone>& b) {
        bones = b;
        return *this;
    }
    Bone* AnimKeyFrame::GetBone(std::string str) {
        for(auto& bone : bones) {
            if(bone.name == str) {
                return &bone;
            }
        }
        return NULL;
    }
    Bone* AnimKeyFrame::GetBone(size_t i) {
        if(i >= 0 && i < bones.size()) {
            return &bones[i];
        }
        return NULL;
    }
    size_t AnimKeyFrame::GetBoneID(std::string str) {
        for(size_t i = 0; i < bones.size(); i++) {
            if(bones[i].name == str) {
                return i;
            }
        }
        return std::string::npos;
    }
    void AnimKeyFrame::ResetBoneIDs() {
        for(auto& bone : bones) {
            bone.ClearChildren();
        }
    }
    void AnimKeyFrame::GenBones() {
        for(size_t i = 0; i < bones.size(); i++) {
            Bone* parent = GetBone(bones[i].parentName);
            if(parent) {
                parent->AddChild(i);
                bones[i].parentID = GetBoneID(bones[i].name);
            }
            else {
                bones[i].parentID = -1;
            }
        }
    }
    Bone& AnimKeyFrame::FindBone(std::string str) {
        return *GetBone(str);
    }
    void AnimKeyFrame::AddBone(Bone b) {
        if(!GetBone(b.name)) {
            bones.push_back(b);
        }
    }
    void AnimKeyFrame::Write(std::ofstream& file) {
        BinaryWrite(file, bones.size());
        for(auto& b: bones) {
            b.Write(file);
        }
        BinaryWrite(file, timestamp);
    }
    void AnimKeyFrame::Read(std::ifstream& file) {
        size_t numBones;
        BinaryRead(file, numBones);
        bones.reserve(numBones);
        for(size_t i = 0; i < numBones; i++) {
            Bone b;
            b.Read(file);
            bones.push_back(b);
        }
        BinaryRead(file, timestamp);
    }
}

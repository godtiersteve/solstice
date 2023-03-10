#ifndef ANIMKEYFRAME_H_INCLUDED
#define ANIMKEYFRAME_H_INCLUDED

#include "bone.h"
#include <vector>

namespace solstice {
struct AnimKeyFrame {
    std::vector<Bone> bones;
    double timestamp;
    AnimKeyFrame();
    AnimeKeyFrame(std::vector<Bone> b);
    AnimKeyFrame& operator=(const std::vector<Bone>& b);
    Bone* GetBone(std::string str);
    Bone* GetBone(size_t i);
    size_t GetBoneID(std::string str);
    void GenBones();
    Bone& FindBone(std::string str);
    void AddBone(Bone b);
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
    void ResetBoneIDs();
};
}

#endif // ANIMKEYFRAME_H_INCLUDED

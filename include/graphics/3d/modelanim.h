#ifndef MODELANIM_H_INCLUDED
#define MODELANIM_H_INCLUDED

#include "animation/animator.h"
#include "bone.h"
#include "animkeyframe.h"
#include <vector>
#include "glm/gtx/vector_angle.hpp"

namespace solstice {

class ModelAnimation : public Animator<AnimKeyFrame> {
private:
    std::vector<Bone> mBones;
    std::vector<glm::mat4>* mBoneTransforms;
    std::vector<AnimKeyFrame> mActiveAnimation;
    std::vector<size_t> mRootBones;
    glm::mat4 mParentMatrix;
    AnimKeyFrame mPrevFrame, mNextFrame;
    int mLastFrame;
    double mCurrentTime;
    double mEndTime;
    int mLoopEndFrame;
    void InterpNextFrame();
    void UpdateBone(size_t i, glm::mat4& parent);
    int mSubAnimationFrame, mSubAnimationSize;
    void GenPrevAndNextFrames();
public:
    ModelAnimation();
    void Update(float t = 1.0f);
    bool operator==(int i)                      {return mID == i;}
    bool operator!=(int i)                      {return mID != i;}
    bool IDComp(const ModelAnimation& anim)     {return mID == anim.mID;}
    bool NameComp(const ModelAnimation& anim)   {return mRef == anim.mRef;}

    AnimKeyFrame GetActiveFrame(size_t i)                   {return mActiveAnimation[i];}
    std::vector<AnimKeyFrame> GetActiveAnimation()          {return mActiveAnimation;}
    double GetPrevTime();
    double GetNextTime();
    int GetBoneID(string s);
    void AddBone(Bone b);
    void AddDeformBone(Bone b);
    void GenBones();
    void SetBoneTransforms(std::vector<Bone>& bones);
    std::vector<Bone>& GetBones()                       {return mBones;}
    Bone& GetBone(size_t i)                             {return mBones[i];}
    Bone* GetBone(string s);
    Bone& FindBone(string s);
    AnimKeyFrame& GetNextFrame();
    AnimKeyFrame& GetPrevFrame();
    AnimKeyFrame& GetNextActiveFrame();
    AnimKeyFrame& GetPrevActiveFrame();
    void ForceFrame(size_t i);
    void ForceBoneTransform(size_t i, glm::mat4 transform);
    std::vector<glm::mat4>* GetBoneTransforms() {return mBoneTransforms;}
    void BindUBO(ShaderProgram& sh, GLuint ubo, std::string name);
    void Reset();
    bool Finished();
    void SetTime(float t)                           {mCurrentTime = t;}
    float GetEndTime()                              {return mEndTime;}
    float GetCurTime()                              {return mCurrentTime;}
    int GetLoopEndFrame()                           {return mLoopEndFrame;}
    void SetEndTime(float f)                        {mEndTime = f;}
    void SetLoopEndFrame(size_t i)                  {mLoopEndFrame = i;}
    float GetTimeToNext()                           {return mNextFrame.timestamp - mCurrentTime;}
    void MapBone(int i, int k, ModelAnimation* anim);
    void MapBoneTransRot(int i, int k, ModelAnimation* anim);
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
    void PrintTransforms();
    glm::mat4& GetBoneTransform(size_t i)               {return (*mBoneTransforms)[i];}
    int GetRemainingSubFrames()                         {return mSubAnimationSize - mSubAnimationFrame;}
    int GetNumSubFrames()                               {return mSubAnimationSize;}
    int GetCurrentSubFrame()                            {return mSubAnimationFrame;}
    bool SubAnimFinished()                             {return !mSubAnimationFrame && !mSubAnimationFrame;}
    void UpdateBones();
    void SetBoneTransformsPtr(std::vector<glm::mat4>* boneTransforms) {mBoneTransforms = boneTransforms;}
    void SwapBones(const std::vector<BoneGroup>& groups, std::vector<ModelAnimation>& anims);
    void SwapBones(const std::vector<BoneGroup>& groups, std::vector<ModelAnimation>& anims, size_t subStartFrame, size_t numSubFrames);
    void SwapBones(const BoneGroup& group, ModelAnimation& anims, size_t subStartFrame, size_t numSubFrames);
    void LookAt(glm::vec3 pos);
    void SetParentMatrix(glm::mat4 mat) {mParentMatrix = mat;}

    void Reverse();
    /*
    vector<bool>& GetAnimationFlags()                       {return mAnimationFlags;}
    bool GetAnimationFlag(size_t i)                         {return mAnimationFlags[i];}
    void SetAnimationFlag(size_t i, bool f)                 {mAnimationFlags[i] = f;}
    void SetAnimationFlags(size_t amt, bool val);
    void SetAnimationFlags(std::vector<bool> flags);
    void ResetAnimationFlags(bool val);
    */
};
}


#endif // MODELANIM_H_INCLUDED

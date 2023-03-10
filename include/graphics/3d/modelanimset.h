#ifndef MODELANIMSET_H_INCLUDED
#define MODELANIMSET_H_INCLUDED

#include "modelanim.h"

namespace solstice {
class ModelAnimationset {
private:
    std::vector<ModelAnimation> mAnimations;
    std::vector<Bone> mBones;
    std::vector<BoneGroup> mBoneGroups;
    std::vector<glm::mat4> mBoneTransforms;
    ModelAnimation mTransitionAnim;
    ModelAnimation* mCurrentAnimation;
    int mCurrentAnimationNum, mTransitionFrame;
    void GenerateTransitionAnim();
public:
    float mUpdateModifier;
    ModelAnimationset();
    void Update(float t);
    void SetLoopFrame(int i);
    void ForceFrame(size_t i);
    std::vector<Bone>& GetBones()                       {return mBones;}
    Bone& GetBone(size_t i)                             {return mBones[i];}
    Bone* GetBone(string s);
    Bone& FindBone(string s);
    glm::mat4 GetBoneMatrix(size_t i)                   {return mBones[i].finalTransformation;}
    std::vector<ModelAnimation>& GetAnimations()        {return mAnimations;}
    int GetBoneID(string s);
    void AddBone(Bone b);
    void AddDeformBone(Bone b);
    void GenBones();
    void SetBoneTransforms(std::vector<Bone>& bones);
    ModelAnimation* GetCurrentAnim()                    {return mCurrentAnimation;}
    int GetCurrentAnimID()                              {return mCurrentAnimation ? mCurrentAnimation->GetID() : -1;}
    int GetCurrentAnimNum()                             {return mCurrentAnimationNum;}
    ModelAnimation* GetAnimation(size_t i)              {return &mAnimations[i];}
    glm::mat4& GetBoneTransform(size_t i)               {return mCurrentAnimation->GetBoneTransform(i);}
    glm::vec3 GetVertexPosition(size_t mesh, size_t vert, size_t boneID, glm::mat4& parentTrans, glm::mat4& trans);
    ModelAnimation* GetAnimation(std::string s);
    int GetAnimationID(std::string anim);
    void AddAnimation(ModelAnimation anim);
    void ChangeAnimation(std::string anim);
    int SetAnimID(std::string n, int id);
    void ChangeAnimation(int i, float transition = 0.05f);
    void TransitionToAnimation(int i, float transition, int sourceFrame, int destFrame);
    void ForceAnimation(std::string anim);
    void ForceAnimation(size_t i);
    void Pause();
    void Stop();
    void Reset();
    void Play();
    void NextAnim();
    void LastAnim();
    void ForceNextAnim();
    void ForceLastAnim();
    void NextFrame();
    void PrevFrame();
    void RemoveAnimation(size_t i);
    int AddBoneGroup(std::string groupName, int boneID);
    void SwapBoneGroup(int srcAnim, int boneGroup);
    void SwapBoneGroups(std::vector<std::pair<int, int> > animIDs);
    void SwapBoneGroup(std::string srcAnim, std::string boneGroup);
    int GetBoneGroupID(std::string name);
    void LookAt(glm::vec3 pos);
    void ResetBoneGroups();
    void SetAnimPointers();
    void SetAnimations(std::vector<ModelAnimation>& animations);
    void SetBones(std::vector<Bone>& bones) {mBones = bones;}
    void SetBoneGroups(std::vector<BoneGroup>& groups) {mBoneGroups = groups;}
    void SetTransitionAnim(ModelAnimation& anim)        {mTransitionAnim = anim;}
    void BindUBO(ShaderProgram& sh, GLuint ubo, std::string name);
    void UpdateTransformMatrix();
    bool CurrentAnimFinished() {return mCurrentAnimation ? mCurrentAnimation->Finished() : false;}
};
}

#endif // MODELANIMSET_H_INCLUDED

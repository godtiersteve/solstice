#ifndef SKELETALMODEL_H_INCLUDED
#define SKELETALMODEL_H_INCLUDED

#include "graphics/shader.h"
#include "graphics/shaderprogram.h"
#include "skeletalmesh.h"
#include "modelbase.h"
#include "modelanimset.h"

namespace solstice {
enum CharacterExpressionType {
    CHAR_EXPRESSION_DEFAULT,
    CHAR_EXPRESSION_SMILING,
    CHAR_EXPRESSION_FROWNING,
    CHAR_EXPRESSION_ANGRY,
    CHAR_EXPRESSION_SAD,
    CHAR_EXPRESSION_HURT
};
class SkeletalModel : public ModelBase<SkeletalMesh> {
private:
    std::vector<ModelAnimation> mAnimations;
    std::vector<Bone> mBones;
    std::vector<BoneGroup> mBoneGroups;
    std::vector<glm::mat4> mBoneTransforms;
    size_t mNumDeformBones;
    ModelAnimation mTransitionAnim;
    ModelAnimation* mCurrentAnimation;
    int mCurrentAnimationNum, mTransitionFrame;
    int mAlternateAnimID;
    GLuint mUBO;
    glm::mat4 mInverseTransform;
    int mExpression;
    void InterpNextFrame();
    void UpdateBone(size_t i, glm::mat4& parent);
    void ResetBoneGroups();
public:
    SkeletalModel();
    SkeletalModel(const SkeletalModel& m) {*this = m;}
    SkeletalModel& operator=(const SkeletalModel& m);
    void Update(float t);
    void Draw(ShaderProgram& sh, TextureArray& textures);
    void Draw(ShaderProgram& sh, TextureArray& textures, glm::mat4& trans);
    void DrawAnimset(ModelAnimationset& animset, ShaderProgram& sh, TextureArray& textures, glm::mat4& trans);
    void DrawAnimset(ModelAnimationset& animset, ShaderProgram& sh, TextureArray& textures);
    void DrawInternalTransform(ShaderProgram& sh, TextureArray& textures);
    void DrawOutline(ShaderProgram& sh, TextureArray& textures);
    void DrawAnimSet(ModelAnimationset& animset, ShaderProgram& sh, TextureArray& textures, glm::mat4& trans);
    void DrawAnimSet(ModelAnimationset& animset, ShaderProgram& sh, TextureArray& textures);
    SkeletalModel& SetupModel();
    void AddVertices(SkeletalMesh mesh);
    void SetCurrentAnimation(ModelAnimation* anim)      {mCurrentAnimation = anim;}
    void InitAnimationset(ModelAnimationset& animSet);

    void SetLoopFrame(int i);
    void ForceFrame(size_t i);
    std::vector<Bone>& GetBones()                       {return mBones;}
    glm::mat4& GetInverseTrans()                        {return mInverseTransform;}
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
    void ChangeAnimation(size_t i, float transition = 0.05f);
    void TransitionToAnimation(size_t i, float transition, int sourceFrame, int destFrame);
    void ForceAnimation(std::string anim);
    void ForceAnimation(size_t i);
    void SetAlternateAnimteID(int id)           {mAlternateAnimID = id;}
    int GetAlternateAniMID()                    {return mAlternateAnimID;}
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
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
    void PrintBoneGroups();
    void PrintBones();
    int AddBoneGroup(std::string groupName, int boneID);
    void SwapBoneGroup(int srcAnim, int boneGroup);
    void SwapBoneGroups(std::vector<std::pair<int, int> > animIDs);
    void SwapBoneGroup(std::string srcAnim, std::string boneGroup);
    int GetBoneGroupID(std::string name);
    void LookAt(glm::vec3 pos);
};
}

#endif // SKELETALMODEL_H_INCLUDED

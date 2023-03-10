#include "modelanimset.h"
namespace solstice {
ModelAnimationset::ModelAnimationset() {
    mCurrentAnimation = nullptr;
    mCurrentAnimationNum = 0;
    mTransitionFrame = 0;
    GenerateTransitionAnim();
    mUpdateModifier = 1.0;
}
void ModelAnimationset::GenerateTransitionAnim() {
    mTransitionAnim = ModelAnimation();
    mTransitionAnim.AddFrame(AnimKeyFrame());
    mTransitionAnim.AddFrame(AnimKeyFrame());
    mTransitionAnim.GetBones() = mBones;
    mTransitionAnim.GenBones();
    mTransitionAnim.SetLoops(0);
    mTransitionAnim.SetEndTime(0.1f);
    mTransitionAnim.SetName("Transitioning");
    mTransitionAnim.SetBoneTransformsPtr(&mBoneTransforms);
}
void ModelAnimationset::SetAnimPointers() {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        mAnimations[i].SetBoneTransformsPtr(&mBoneTransforms);
    }
    GenerateTransitionAnim();
}
void ModelAnimationset::Update(float t) {
    if(mCurrentAnimation) {
        if(mCurrentAnimation->Finished() && mCurrentAnimationNum < 0) {
            ForceAnimation(-mCurrentAnimationNum);
            if(mTransitionFrame >= 0) {
                mCurrentAnimation->ForceFrame(mTransitionFrame);
            }
        }
        mCurrentAnimation->Update(t * mUpdateModifier);
    }
}
void ModelAnimationset::LookAt(glm::vec3 pos) {
    mCurrentAnimation->LookAt(pos);
}
void ModelAnimationset::GenBones() {
    mBoneTransforms.resize(mBones.size());
    for(size_t i = 0; i < mBones.size(); i++) {
        Bone* parent = GetBone(mBones[i].parentName);
        if(parent) {
            parent->AddChild(i);
            mBones[i].parentID = GetBoneID(mBones[i].name);
        }
    }
    GenerateTransitionAnim();
}
void ModelAnimationset::SetAnimations(std::vector<ModelAnimation>& animations) {
    mAnimations = animations;
    mCurrentAnimation = &mAnimations.front();
    mCurrentAnimationNum = 0;
    SetAnimPointers();
}
int ModelAnimationset::AddBoneGroup(std::string groupName, int boneID) {
    int id = -1;
    for(size_t i = 0; i < mBoneGroups.size(); i++) {
        if(mBoneGroups[i].name == groupName) {
            id = i;
            break;
        }
    }
    if(id == -1) {
        mBoneGroups.push_back(BoneGroup(groupName));
        id = mBoneGroups.size() - 1;
    }
    mBoneGroups[id].boneIDs.push_back(boneID);
    return id;
}
void ModelAnimationset::SetBoneTransforms(std::vector<Bone>& bones) {
    bool regenBones = false;
    for(size_t i = 0; i < bones.size(); i++) {
        mBones[i].scale = bones[i].scale;
        mBones[i].translation = bones[i].translation;
        mBones[i].rotation = bones[i].rotation;
        if(mBones.size() - 1 < i) {
            AddBone(bones[i]);
            regenBones = true;
        }
    }
    if(regenBones) {
        GenBones();
    }
}
Bone* ModelAnimationset::GetBone(string s) {
    if(s.empty()) {
        return NULL;
    }
    for(auto& bone : mBones) {
        if(bone.name == s) {
            return &bone;
        }
    }
    return NULL;
}
void ModelAnimationset::AddBone(Bone b) {
    if(!GetBone(b.name)) {
        mBones.push_back(b);
    }
}
Bone& ModelAnimationset::FindBone(string s) {
    for(auto& bone : mBones) {
        if(bone.name == s) {
            return bone;
        }
    }
    return mBones[0];
}
int ModelAnimationset::GetBoneID(string s) {
    for(size_t i = 0; i < mBones.size(); i++) {
        if(mBones[i].name == s) {
            return i;
        }
    }
    return -1;
}
void ModelAnimationset::ChangeAnimation(std::string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == anim) {
            ChangeAnimation(i);
            return;
        }
    }
}
void ModelAnimationset::ChangeAnimation(int i, float transition) {
    TransitionToAnimation(i, transition, -1, 0);
}
void ModelAnimationset::TransitionToAnimation(int i, float transition, int sourceFrame, int destFrame) {
    if(i < 0 || i >= mAnimations.size() || mCurrentAnimationNum == i) {
        return;
    }

    if(GetCurrentAnimID() < 0) {
        transition = 0.0f;
    }
    if(transition > 0.0) {
        if(mCurrentAnimationNum != i && mCurrentAnimationNum >= 0) {
            if(mCurrentAnimation->GetBones().size() == mAnimations[i].GetBones().size()) {
                if(sourceFrame == -1) {
                    sourceFrame = mCurrentAnimation->GetCurFrameNum();
                }
                int transitionSubStart = mCurrentAnimation->GetCurrentSubFrame() + 1;
                int transitionSubNum = mCurrentAnimation->GetNumSubFrames() - transitionSubStart;
                if(transitionSubNum > 0 && transitionSubStart > 0) {
                    mAnimations[i].SwapBones(mBoneGroups, mAnimations, transitionSubStart, transitionSubNum);
                }
                mTransitionAnim.GetFrame(0) = mCurrentAnimation->GetActiveFrame(sourceFrame);
                mTransitionAnim.GetFrame(0).timestamp = 0.0f;
                if(destFrame >= mAnimations[i].Size()) {
                    destFrame = 0;
                }
                mTransitionAnim.GetFrame(1) = mAnimations[i].GetActiveFrame(destFrame);
                mTransitionFrame = destFrame;
                mTransitionAnim.GetFrame(1).timestamp = transition;
                mTransitionAnim.SetEndTime(transition);
                mTransitionAnim.SetLoops(0);
                mTransitionAnim.SetID(-mAnimations[i].GetID());
                mTransitionAnim.Reset();
                mCurrentAnimation->Reset();
                mCurrentAnimation = &mTransitionAnim;
                mCurrentAnimationNum = -i;
                ResetBoneGroups();
            }
            else {
               ForceAnimation(i);
            }
        }
    }
    else {
        ForceAnimation(i);
    }
}
void ModelAnimationset::ForceAnimation(std::string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == anim) {
            mCurrentAnimation = &mAnimations[i];
            mCurrentAnimationNum = i;
        }
    }
    mCurrentAnimation->Reset();
    ResetBoneGroups();
}
void ModelAnimationset::ForceAnimation(size_t i) {
    mCurrentAnimation = &mAnimations[i];
    mCurrentAnimationNum = i;
    mCurrentAnimation->Reset();
    ResetBoneGroups();
}
void ModelAnimationset::ResetBoneGroups() {
    int id = abs(GetCurrentAnimID());
    for(auto& b: mBoneGroups) {
        b.animID = id;
        b.animIndex = mCurrentAnimationNum;
    }
}
void ModelAnimationset::AddAnimation(ModelAnimation anim) {
    if(anim.GetID() == -1) {
        anim.SetID(mAnimations.size());
    }
    mAnimations.push_back(anim);
    if(mCurrentAnimationNum >= 0) {
        mCurrentAnimation = &mAnimations[mCurrentAnimationNum];
    }
    else {
        mCurrentAnimation = &mTransitionAnim;
    }
    mCurrentAnimation->Reset();
}
ModelAnimation* ModelAnimationset::GetAnimation(std::string s) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == s) {
            return &mAnimations[i];
        }
    }
    return NULL;
}
int ModelAnimationset::GetAnimationID(std::string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == anim) {
            return i;
        }
    }
    return -1;
}
int ModelAnimationset::SetAnimID(std::string n, int id) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == n) {
            mAnimations[i].SetID(id);
            return i;
        }
    }
    return -1;
}
void ModelAnimationset::NextFrame() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Pause();
        mCurrentAnimation->ForceFrame(mCurrentAnimation->GetCurFrameNum() + 1);
    }
}
void ModelAnimationset::PrevFrame() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Pause();
        int frame = mCurrentAnimation->GetCurFrameNum();
        mCurrentAnimation->ForceFrame(frame - 1);
    }
}
void ModelAnimationset::Pause() {
    if(mCurrentAnimation) {
        if(mCurrentAnimation->GetStatus() == ANIM_PAUSED) {
            mCurrentAnimation->Play();
        }
        else {
            mCurrentAnimation->Pause();
        }
    }
}
void ModelAnimationset::Stop() {
    if(mCurrentAnimation) {
        if(mCurrentAnimation->GetStatus() == ANIM_STOPPED) {
            mCurrentAnimation->Play();
        }
        else {
            mCurrentAnimation->Reset();
            mCurrentAnimation->Pause();
        }
    }
}
void ModelAnimationset::Reset() {
    mCurrentAnimation = &mAnimations[mCurrentAnimationNum];
    if(mCurrentAnimation) {
        mCurrentAnimation->Reset();
    }
}
void ModelAnimationset::Play() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Start();
    }
}
void ModelAnimationset::NextAnim() {
    int anim = mCurrentAnimationNum;
    if(anim < 0) {
        anim = -mCurrentAnimationNum + 1;
    }
    if(anim >= mAnimations.size() - 1) {
        anim = 0;
    }
    else {
        anim++;
    }
    ChangeAnimation(anim);
}
void ModelAnimationset::ForceNextAnim() {
    int anim = mCurrentAnimationNum;
    if(anim < 0) {
        anim = -mCurrentAnimationNum + 1;
    }
    if(anim >= mAnimations.size() - 1) {
        anim = 0;
    }
    else {
        anim++;
    }
    ForceAnimation(anim);
}
void ModelAnimationset::ForceLastAnim() {
    int anim = mCurrentAnimationNum;
    if(anim < 0) {
        anim = -mCurrentAnimationNum - 1;
    }
    if(anim <= 0) {
        anim = mAnimations.size() -1;
    }
    else {
        anim--;
    }
    ForceAnimation(anim);
}
void ModelAnimationset::LastAnim() {
    int anim = mCurrentAnimationNum;
    if(anim < 0) {
        anim = -mCurrentAnimationNum - 1;
    }
    if(anim <= 0) {
        anim = mAnimations.size() -1;
    }
    else {
        anim--;
    }
    ChangeAnimation(anim);
}
void ModelAnimationset::SwapBoneGroup(int srcAnim, int boneGroup) {
    if(srcAnim < 0 || srcAnim >= mAnimations.size()) {
        return;
    }
    mBoneGroups[boneGroup].animID = mAnimations[srcAnim].GetID();
    mBoneGroups[boneGroup].animIndex = srcAnim;
    mCurrentAnimation->SwapBones(mBoneGroups, mAnimations);
}
void ModelAnimationset::SwapBoneGroup(std::string srcAnim, std::string boneGroup) {
    SwapBoneGroup(GetAnimationID(srcAnim), GetBoneGroupID(boneGroup));
}
void ModelAnimationset::SwapBoneGroups(std::vector<std::pair<int, int> > animIDs) {
    for(auto p : animIDs) {
        if(p.second >= mBoneGroups.size() || p.first >= mAnimations.size()) {
            break;
        }
        mBoneGroups[p.second].animID = p.first;
        for(auto id : mBoneGroups[p.second].boneIDs) {
            mCurrentAnimation->GetBone(id).translation = mAnimations[p.first].GetFrame(0).bones[id].translation;
            mCurrentAnimation->GetBone(id).rotation = mAnimations[p.first].GetFrame(0).bones[id].rotation;
            mCurrentAnimation->GetBone(id).scale = mAnimations[p.first].GetFrame(0).bones[id].scale;
        }
    }
    mCurrentAnimation->SwapBones(mBoneGroups, mAnimations);
}
int ModelAnimationset::GetBoneGroupID(std::string name) {
    for(int i = 0; i < mBoneGroups.size(); i++) {
        if(mBoneGroups[i].name == name) {
            return i;
        }
    }
    return -1;
}
void ModelAnimationset::RemoveAnimation(size_t i) {
    mAnimations.erase(mAnimations.begin() + i);
    mCurrentAnimation = NULL;
}
void ModelAnimationset::BindUBO(ShaderProgram& sh, GLuint ubo, std::string name) {
    if(mCurrentAnimation) {
        mCurrentAnimation->BindUBO(sh, ubo, name);
    }
}
}

#include "skeletalmodel.h"
namespace solstice {
SkeletalModel::SkeletalModel() {
    mCurrentAnimation = NULL;
    mCurrentAnimationNum = 0;
    mTransitionFrame = 0;
    mNumDeformBones = 0;
    mAlternateAnimID = -1;
}
SkeletalModel& SkeletalModel::operator=(const SkeletalModel& m) {
    mMeshes = m.mMeshes;
    mOutlineColor = m.mOutlineColor;
    mTransform = m.mTransform;
    mName = m.mName;
    mHidden = m.mHidden;
    mAnimations = m.mAnimations;
    if(mAnimations.empty()) {
        mCurrentAnimation = NULL;
    }
    else {
        mCurrentAnimation = &mAnimations[0];
    }
    mCurrentAnimationNum = m.mCurrentAnimationNum;
    mBones = m.mBones;
    mUBO = m.mUBO;
    mTransitionAnim = m.mTransitionAnim;
    mTransitionFrame = m.mTransitionFrame;
    mBoneGroups = m.mBoneGroups;
    mBoneTransforms = m.mBoneTransforms;
    mNumDeformBones = m.mNumDeformBones;
    mAlternateAnimID = m.mAlternateAnimID;
    return *this;
}
SkeletalModel& SkeletalModel::SetupModel() {
    for(auto& mesh : mMeshes) {
        mesh.SetupMesh();
    }
    GenBones();
    mTransitionAnim.AddFrame(AnimKeyFrame());
    mTransitionAnim.AddFrame(AnimKeyFrame());
    mTransitionAnim.GetBones() = mBones;
    mTransitionAnim.GenBones();
    mTransitionAnim.SetLoops(0);
    mTransitionAnim.SetEndTime(0.1f);
    mTransitionAnim.SetName("Transitioning");
    if(!mAnimations.empty()) {
        mCurrentAnimation = &mAnimations[0];
        mCurrentAnimation->GenBones();
    }
    else {
        mCurrentAnimation = NULL;
    }
    for(auto& group : mBoneGroups) {
        group.animID = -1;
    }
    glGenBuffers(1, &mUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * mBones.size(), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return *this;
}
void SkeletalModel::Update(float t) {
    if(mCurrentAnimation) {
        /*
        if(mName == "Iona" && GetMeshPtr("Iona-LowerTail") && GetMeshPtr("Iona-LowerLegs")) {
            if(mCurrentAnimation->GetName().back() == 'B') {
                GetMeshPtr("Iona-LowerTail")->SetHidden(false);
                GetMeshPtr("Iona-LowerLegs")->SetHidden(true);
            }
            else if(mCurrentAnimation->GetName().back() == 'A') {
                GetMeshPtr("Iona-LowerTail")->SetHidden(true);
                GetMeshPtr("Iona-LowerLegs")->SetHidden(false);
            }
        }
        */
        if(mCurrentAnimation->Finished() && mCurrentAnimationNum < 0) {
            //mCurrentAnimation = &mAnimations[-mCurrentAnimationNum];
            ForceAnimation(-mCurrentAnimationNum);
            //mCurrentAnimationNum = -mCurrentAnimationNum;
            if(mTransitionFrame >= 0) {
                mCurrentAnimation->ForceFrame(mTransitionFrame);
            }
        }
        mCurrentAnimation->SetBoneTransformsPtr(&mBoneTransforms);
        mCurrentAnimation->Update(t);
    }
}
void SkeletalModel::InitAnimationset(ModelAnimationset& animSet) {
    animSet.SetAnimations(mAnimations);
    animSet.SetBones(mBones);
    animSet.GenBones();
    //animSet.SetTransitionAnim(mTransitionAnim);
    animSet.SetBoneGroups(mBoneGroups);
    animSet.Reset();
}
void SkeletalModel::LookAt(glm::vec3 pos) {
    mCurrentAnimation->LookAt(pos);
}
void SkeletalModel::Draw(ShaderProgram& sh, TextureArray& textures) {
    if(mHidden || !mCurrentAnimation) {
        return;
    }
    mCurrentAnimation->BindUBO(sh, mUBO, "BoneData");
    for(auto& mesh : mMeshes) {
        mesh.Draw(sh, textures);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
glm::vec3 SkeletalModel::GetVertexPosition(size_t mesh, size_t vert, size_t boneID, glm::mat4& parentTrans, glm::mat4& trans) {
    if(mesh >= mMeshes.size()) {
        return glm::vec3(0);
    }
    if(mMeshes[mesh].GetVertices().size() <= vert) {
        return glm::vec3(0.0);
    }
    glm::vec4 pos = glm::vec4(mMeshes[mesh].GetVertex(vert).position, 1.0f);
    return glm::vec3(trans * mMeshes[mesh].GetInverseTrans() * parentTrans * mBones[boneID].offsetMatrix * pos);
}
void SkeletalModel::Draw(ShaderProgram& sh, TextureArray& textures, glm::mat4& trans) {
    if(mHidden || !mCurrentAnimation) {
        return;
    }
    mCurrentAnimation->BindUBO(sh, mUBO, "BoneData");
    for(auto& mesh : mMeshes) {
        textures.BindTexture(mesh.GetTextureID());
        mesh.Draw(sh, trans);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void SkeletalModel::DrawInternalTransform(ShaderProgram& sh, TextureArray& textures) {
    if(mHidden || !mCurrentAnimation) {
        return;
    }
    mCurrentAnimation->BindUBO(sh, mUBO, "BoneData");
    glm::mat4 trans = mTransform.GetTransform();
    for(auto& mesh : mMeshes) {
        textures.BindTexture(mesh.GetTextureID());
        mesh.Draw(sh, trans);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void SkeletalModel::DrawAnimset(ModelAnimationset& animset, ShaderProgram& sh, TextureArray& textures) {
    glm::mat4 trans = mTransform.GetTransform();
    DrawAnimset(animset, sh, textures, trans);
}
void SkeletalModel::DrawAnimset(ModelAnimationset& animset, ShaderProgram& sh, TextureArray& textures, glm::mat4& trans) {
    if(mHidden || !mCurrentAnimation) {
        return;
    }
    animset.BindUBO(sh, mUBO, "BoneData");
    //mCurrentAnimation->BindUBO(sh, mUBO, "BoneData");
    for(auto& mesh : mMeshes) {
        textures.BindTexture(mesh.GetTextureID());
        mesh.Draw(sh, trans);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void SkeletalModel::DrawOutline(ShaderProgram& sh, TextureArray& textures) {
    if(mHidden || !mCurrentAnimation) {
        return;
    }
    mCurrentAnimation->BindUBO(sh, mUBO, "BoneData");
    for(auto& mesh : mMeshes) {
        mesh.DrawOutline(sh, textures);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void SkeletalModel::GenBones() {
    mBoneTransforms.resize(mBones.size());
    for(size_t i = 0; i < mBones.size(); i++) {
        Bone* parent = GetBone(mBones[i].parentName);
        if(parent) {
            parent->AddChild(i);
            mBones[i].parentID = GetBoneID(mBones[i].name);
        }
    }
}
void SkeletalModel::PrintBoneGroups() {
    for(auto group : mBoneGroups) {
        for(auto id : group.boneIDs) {
            printf("Group: %s, Bone: %s\n", group.name.c_str(), mBones[id].name.c_str());
        }
    }
}
int SkeletalModel::AddBoneGroup(std::string groupName, int boneID) {
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
void SkeletalModel::PrintBones() {
    for(auto& b : mBones) {
        printf("Bone: %s \t(Parent: %s)\n", b.name.c_str(), b.parentName.c_str());
    }
}
void SkeletalModel::SetBoneTransforms(std::vector<Bone>& bones) {
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
Bone* SkeletalModel::GetBone(string s) {
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
void SkeletalModel::AddBone(Bone b) {
    if(!GetBone(b.name)) {
        mBones.push_back(b);
    }
}
void SkeletalModel::AddDeformBone(Bone b) {
    if(!GetBone(b.name)) {
        mBones.push_back(b);
        mNumDeformBones++;
    }
}
Bone& SkeletalModel::FindBone(string s) {
    for(auto& bone : mBones) {
        if(bone.name == s) {
            return bone;
        }
    }
    return mBones[0];
}
int SkeletalModel::GetBoneID(string s) {
    for(size_t i = 0; i < mBones.size(); i++) {
        if(mBones[i].name == s) {
            return i;
        }
    }
    return -1;
}
void SkeletalModel::AddVertices(SkeletalMesh mesh) {
    ModelBase::AddVertices(mesh);
}
void SkeletalModel::ChangeAnimation(std::string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == anim) {
            ChangeAnimation(i);
            return;
        }
    }
}
void SkeletalModel::ChangeAnimation(size_t i, float transition) {
    TransitionToAnimation(i, transition, -1, 0);
}
void SkeletalModel::TransitionToAnimation(size_t i, float transition, int sourceFrame, int destFrame) {
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
void SkeletalModel::ForceAnimation(std::string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == anim) {
            mCurrentAnimation = &mAnimations[i];
            mCurrentAnimationNum = i;
        }
    }
    mCurrentAnimation->Reset();
    ResetBoneGroups();
}
void SkeletalModel::ForceAnimation(size_t i) {
    mCurrentAnimation = &mAnimations[i];
    mCurrentAnimationNum = i;
    mCurrentAnimation->Reset();
    ResetBoneGroups();
}
void SkeletalModel::ResetBoneGroups() {
    int id = abs(GetCurrentAnimID());
    for(auto& b: mBoneGroups) {
        b.animID = id;
        b.animIndex = mCurrentAnimationNum;
    }
}
void SkeletalModel::AddAnimation(ModelAnimation anim) {
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
ModelAnimation* SkeletalModel::GetAnimation(std::string s) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == s) {
            return &mAnimations[i];
        }
    }
    return NULL;
}
int SkeletalModel::GetAnimationID(std::string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == anim) {
            return i;
        }
    }
    return -1;
}
int SkeletalModel::SetAnimID(std::string n, int id) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetName() == n) {
            mAnimations[i].SetID(id);
            return i;
        }
    }
    return -1;
}
void SkeletalModel::NextFrame() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Pause();
        mCurrentAnimation->ForceFrame(mCurrentAnimation->GetCurFrameNum() + 1);
    }
}
void SkeletalModel::PrevFrame() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Pause();
        int frame = mCurrentAnimation->GetCurFrameNum();
        mCurrentAnimation->ForceFrame(frame - 1);
    }
}
void SkeletalModel::Pause() {
    if(mCurrentAnimation) {
        if(mCurrentAnimation->GetStatus() == ANIM_PAUSED) {
            mCurrentAnimation->Play();
        }
        else {
            mCurrentAnimation->Pause();
        }
    }
}
void SkeletalModel::Stop() {
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
void SkeletalModel::Reset() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Reset();
    }
}
void SkeletalModel::Play() {
    if(mCurrentAnimation) {
        mCurrentAnimation->Start();
    }
}
void SkeletalModel::NextAnim() {
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
void SkeletalModel::ForceNextAnim() {
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
void SkeletalModel::ForceLastAnim() {
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
void SkeletalModel::LastAnim() {
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
void SkeletalModel::Write(std::ofstream& file) {
    ModelBase<SkeletalMesh>::Write(file);
    BinaryWrite(file, mAnimations.size());
    for(auto& anim : mAnimations) {
        anim.Write(file);
    }
    BinaryWrite(file, mBones.size());
    for(auto b: mBones) {
        b.Write(file);
    }
    BinaryWriteMatrix(file, mInverseTransform);
}
void SkeletalModel::Read(std::ifstream& file) {
    ModelBase<SkeletalMesh>::Read(file);
    size_t numAnims;
    BinaryRead(file, numAnims);
    mAnimations.resize(numAnims);
    for(auto& anim : mAnimations) {
        anim.Read(file);
    }
    size_t numBones;
    BinaryRead(file, numBones);
    mBones.resize(numBones);
    for(auto& b: mBones) {
        b.Read(file);
    }
    BinaryReadMatrix(file, mInverseTransform);
}
void SkeletalModel::SwapBoneGroup(int srcAnim, int boneGroup) {
    if(srcAnim < 0 || srcAnim >= mAnimations.size()) {
        return;
    }
    mBoneGroups[boneGroup].animID = mAnimations[srcAnim].GetID();
    mBoneGroups[boneGroup].animIndex = srcAnim;
    /*
    for(auto id : mBoneGroups[boneGroup].boneIDs) {
        mCurrentAnimation->GetBone(id).translation = mAnimations[srcAnim].GetFrame(0).bones[id].translation;
        mCurrentAnimation->GetBone(id).rotation = mAnimations[srcAnim].GetFrame(0).bones[id].rotation;
        mCurrentAnimation->GetBone(id).scale = mAnimations[srcAnim].GetFrame(0).bones[id].scale;
    }
    */
    mCurrentAnimation->SwapBones(mBoneGroups, mAnimations);
}
void SkeletalModel::SwapBoneGroup(std::string srcAnim, std::string boneGroup) {
    SwapBoneGroup(GetAnimationID(srcAnim), GetBoneGroupID(boneGroup));
}
void SkeletalModel::SwapBoneGroups(std::vector<std::pair<int, int> > animIDs) {
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
int SkeletalModel::GetBoneGroupID(std::string name) {
    for(int i = 0; i < mBoneGroups.size(); i++) {
        if(mBoneGroups[i].name == name) {
            return i;
        }
    }
    return -1;
}
void SkeletalModel::RemoveAnimation(size_t i) {
    mAnimations.erase(mAnimations.begin() + i);
    mCurrentAnimation = NULL;
}
}

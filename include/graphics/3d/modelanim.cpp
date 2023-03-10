#include "modelanim.h"

namespace solstice {
    ModelAnimation::ModelAnimation() {
        mEndTime = 1.0f;
        mCurrentTime = 0.0f;
        mStatus = ANIM_PLAYING;
        mLoopEndFrame = -1;
        mBoneTransforms = NULL;
        mSubAnimationFrame = 0;
        mSubAnimationSize = 0;
        mFlags = 0;
    }
    void ModelAnimation::Update(float t) {
        if(!Finished() && !mAnimation.empty()) {
            if(mActiveAnimation.empty()) {
                mActiveAnimation = mAnimation;
            }
            if(mStatus == ANIM_PLAYING) {
                mCurrentTime+= t;
                if(mCurrentTime >= mEndTime) {
                    mLoopsPerformed++;
                    if(mLoops <= -1 || (mLoops >= 0 && mLoopsPerformed <= mLoops)) {
                        mCurFrameNum = mLoopFrame;
                        mSubAnimationFrame++;
                        if(mLoopFrame > 0) {
                            mCurrentTime = mActiveAnimation[mLoopFrame].timestamp;;
                        }
                        else {
                            mCurrentTime = 0.0f;
                        }
                    }
                }
                float nextTime = GetNextTime();
                if(nextTime < mCurrentTime) {
                    mCurFrameNum++;
                    mSubAnimationFrame++;
                    if(mCurFrameNum >= mActiveAnimation.size()) {
                        mCurFrameNum = mLoopFrame;
                        mCurrentTime = mActiveAnimation[mLoopFrame].timestamp;
                    }
                }
                if(mSubAnimationFrame >= mSubAnimationSize) {
                    mSubAnimationFrame = 0;
                    if(mSubAnimationSize > 0) {
                        mSubAnimationSize = 0;
                        mActiveAnimation = mAnimation;
                    }
                }

            }
        }
        GenPrevAndNextFrames();
        InterpNextFrame();
        UpdateBones();
    }
    void ModelAnimation::LookAt(glm::vec3 pos) {
        int nextFrame = mCurFrameNum + 1;
        if(nextFrame >= mAnimation.size()) {
            nextFrame = 0;
        }
        int boneID = GetBoneID("Head_Head");
        glm::vec3 origin = glm::normalize(glm::vec3(0.0f, 2.0f, 0.5f));
        pos = glm::normalize(pos);
        glm::vec3 diff = pos - origin;
        //diff.z = 0.0f;
        float angle = acos(glm::dot(pos, origin));
        glm::vec3 axis = glm::normalize(glm::vec3(sin(angle), cos(angle), tan(angle)));
        std::cout << solstice::RadiansToDegrees(angle) << std::endl;
        printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
        if(boneID > -1) {
            mActiveAnimation[nextFrame].GetBone(boneID)->rotation = glm::angleAxis(angle, axis);
            mActiveAnimation[mCurFrameNum].GetBone(boneID)->rotation = glm::angleAxis(angle, axis);
        }
    }
    void ModelAnimation::SwapBones(const std::vector<BoneGroup>& groups, std::vector<ModelAnimation>& anims) {
        if(mActiveAnimation.empty()) {
            mActiveAnimation = mAnimation;
        }
        for(auto group : groups) {
            if(group.animID != mID && mID >= 0 && group.animIndex >= 0) {
                SwapBones(group, anims[group.animIndex], 0, anims[group.animIndex].GetNumFrames());
            }
        }
    }
    void ModelAnimation::SwapBones(const std::vector<BoneGroup>& groups, std::vector<ModelAnimation>& anims, size_t subStartFrame, size_t numSubFrames) {
        if(mActiveAnimation.empty()) {
            mActiveAnimation = mAnimation;
        }
        for(auto group : groups) {
            if(group.animID != mID && mID >= 0) {
                SwapBones(group, anims[group.animID], subStartFrame, numSubFrames);
            }
        }
    }
    void ModelAnimation::SwapBones(const BoneGroup& group, ModelAnimation& anim, size_t subStartFrame, size_t numSubFrames) {
        int numFrames = numSubFrames;
        int extraFrames = mAnimation.size() - mCurFrameNum - 1;
        if(numFrames > extraFrames && !mLoops) {
            numFrames = extraFrames;
        }
        else if(mLoops && numFrames >= mAnimation.size()) {
            numFrames = mAnimation.size();
        }
        size_t curFrame = mCurFrameNum;
        mSubAnimationSize = numFrames - 1;
        mSubAnimationFrame = 0;
        int endFrame = 0;
        for(size_t i = 0; i < numFrames; i++) {
            curFrame = mCurFrameNum + i - endFrame;
            if(curFrame >= mAnimation.size() && !endFrame) {
                endFrame = curFrame;
                curFrame = mCurFrameNum + i - endFrame;
            }
            AnimKeyFrame& frame = mActiveAnimation[curFrame];
            int subFrame = i + subStartFrame;
            if(i >= anim.GetNumFrames() || subFrame >= anim.GetNumFrames()) {
                break;
            }
            for(auto b : group.boneIDs) {
                if(b < frame.bones.size()) {
                    frame.bones[b].translation = anim.GetFrame(subFrame).bones[b].translation;
                    frame.bones[b].rotation = anim.GetFrame(subFrame).bones[b].rotation;
                    frame.bones[b].scale = anim.GetFrame(subFrame).bones[b].scale;
                }
            }
        }
    }
    void ModelAnimation::GenPrevAndNextFrames() {
        if(mAnimation.size() == 1) {
            mNextFrame = mActiveAnimation[0];
            mPrevFrame = mActiveAnimation[0];
        }
        else if(!mAnimation.empty()) {
            mNextFrame = GetNextFrame();
            mPrevFrame = mActiveAnimation[mCurFrameNum];
        }
    }
    void ModelAnimation::UpdateBones() {
        for(size_t i = 0; i < mRootBones.size(); i++) {
            UpdateBone(mRootBones[i], mParentMatrix);
        }
    }
    void ModelAnimation::MapBoneTransRot(int i, int k, ModelAnimation* anim) {
        if(i == -1 || k == -1 || !anim) {
            return;
        }

        anim->GetNextActiveFrame().bones[k].translation = mNextFrame.bones[i].translation;
        anim->GetNextActiveFrame().bones[k].rotation = mNextFrame.bones[i].rotation;
        //anim->GetNextActiveFrame().bones[k].offsetMatrix = mNextFrame.bones[i].offsetMatrix;

        anim->GetPrevActiveFrame().bones[k].translation = mPrevFrame.bones[i].translation;
        anim->GetPrevActiveFrame().bones[k].rotation = mPrevFrame.bones[i].rotation;
        //anim->GetPrevActiveFrame().bones[k].offsetMatrix = mPrevFrame.bones[i].offsetMatrix;
        /*
        int nextFrame = mCurFrameNum + 1;
        if(nextFrame >= mAnimation.size()) {
            nextFrame = mLoopFrame;
        }
        anim->GetNextActiveFrame().bones[k].translation = mAnimation[nextFrame].bones[i].translation;
        anim->GetNextActiveFrame().bones[k].rotation = mAnimation[nextFrame].bones[i].rotation;

        anim->GetPrevActiveFrame().bones[k].translation = mAnimation[mCurFrameNum].bones[i].translation;
        anim->GetPrevActiveFrame().bones[k].rotation = mAnimation[mCurFrameNum].bones[i].rotation;
        */
    }
    void ModelAnimation::MapBone(int i, int k, ModelAnimation* anim) {
        if(i == -1 || k == -1 || !anim || anim->GetFrames().empty() || mAnimation.empty()) {
            return;
        }
        if(anim->GetFrames().size() <= 1) {
            AnimKeyFrame f = GetCurrentFrame();
            anim->GetCurrentFrame().bones[k].translation = f.bones[i].translation;
            anim->GetCurrentFrame().bones[k].rotation = f.bones[i].rotation;
            anim->GetCurrentFrame().bones[k].scale = f.bones[i].scale;
        }
        else {
            anim->GetNextActiveFrame().bones[k].scale = mNextFrame.bones[i].scale;
            anim->GetNextActiveFrame().bones[k].translation = mNextFrame.bones[i].translation;
            anim->GetNextActiveFrame().bones[k].rotation = mNextFrame.bones[i].rotation;

            anim->GetPrevActiveFrame().bones[k].scale = mPrevFrame.bones[i].scale;
            anim->GetPrevActiveFrame().bones[k].translation = mPrevFrame.bones[i].translation;
            anim->GetPrevActiveFrame().bones[k].rotation = mPrevFrame.bones[i].rotation;
        }
        anim->UpdateBones();
    }
    void ModelAnimation::InterpNextFrame() {
        if(mNextFrame.bones.size() != mPrevFrame.bones.size() || mNextFrame.bones.size() != mCurFrame.bones.size() || mPrevFrame.bones.size() != mCurFrame.bones.size()) {
            return;
        }
        float time = (mCurrentTime - mPrevFrame.timestamp) / (GetNextTime() - mPrevFrame.timestamp);
        if(time > 1.0) {
            time = 1.0;
        }
        if(mAnimation.size() == 1) {
            mCurFrame = mNextFrame;
        }
        else if(time > 0.0) {
            for(size_t i = 0; i < mCurFrame.bones.size(); i++) {
                glm::vec3 scale = mNextFrame.GetBone(i)->scale - mPrevFrame.GetBone(i)->scale;
                glm::vec3 trans = mNextFrame.GetBone(i)->translation - mPrevFrame.GetBone(i)->translation;
                glm::quat rot = glm::slerp(mPrevFrame.GetBone(i)->rotation, mNextFrame.GetBone(i)->rotation, time);
                mCurFrame.GetBone(i)->scale = mPrevFrame.GetBone(i)->scale + scale * time;
                mCurFrame.GetBone(i)->translation = mPrevFrame.GetBone(i)->translation + trans * time;
                mCurFrame.GetBone(i)->rotation = rot;
            }
        }
    }
    void ModelAnimation::UpdateBone(size_t i, glm::mat4& parent) {
        if(!mBoneTransforms || mBoneTransforms->size() <= i) {
            return;
        }
        //std::cout << mBones[i].name << std::endl;
        mCurFrame.GetBone(i)->transform = mCurFrame.GetBone(i)->GetTransform();
        glm::mat4 trans = parent * mCurFrame.GetBone(i)->transform;
        mCurFrame.GetBone(i)->finalTransformation = trans;
        (*mBoneTransforms)[i] = trans * mCurFrame.GetBone(i)->offsetMatrix;
        for(auto child : mBones[i].children) {
            UpdateBone(child, trans);
        }
    }
    void ModelAnimation::ForceBoneTransform(size_t i, glm::mat4 transform) {
        if(mBoneTransforms) {
            (*mBoneTransforms)[i] = transform;
        }
    }
    double ModelAnimation::GetNextTime() {
        if(mCurFrameNum == mAnimation.size() -1) {
            int prevFrame = mCurFrameNum - 1;
            if(prevFrame < 0) {
                prevFrame = mAnimation.size() -1;
            }
            return mAnimation[mCurFrameNum].timestamp + (mAnimation[mCurFrameNum].timestamp - mAnimation[prevFrame].timestamp);
        }
        return mAnimation[mCurFrameNum + 1].timestamp;
    }
    double ModelAnimation::GetPrevTime() {
        if(mCurFrameNum > mLoopFrame) {
            return mPrevFrame.timestamp;
        }
        else {
            return 0;
        }
    }
    AnimKeyFrame& ModelAnimation::GetNextFrame() {
        if(mCurFrameNum < mActiveAnimation.size() - 1) {
            mNextFrame = mActiveAnimation[mCurFrameNum + 1];
        }
        else {
            mNextFrame = mActiveAnimation[mLoopFrame];
        }
        return mNextFrame;
    }
    AnimKeyFrame& ModelAnimation::GetPrevFrame() {
        mPrevFrame = mActiveAnimation[mCurFrameNum];
        return mPrevFrame;
    }
    AnimKeyFrame& ModelAnimation::GetNextActiveFrame() {
        if(mActiveAnimation.empty() && !mAnimation.empty()) {
            mActiveAnimation = mAnimation;
        }
        if(mCurFrameNum < mActiveAnimation.size() - 1) {
            return mActiveAnimation[mCurFrameNum + 1];
        }
        else {
            return mActiveAnimation[mLoopFrame];
        }
    }
    AnimKeyFrame& ModelAnimation::GetPrevActiveFrame() {
        if(mActiveAnimation.empty() && !mAnimation.empty()) {
            mActiveAnimation = mAnimation;
        }
        if(mCurFrameNum > 0) {
            return mActiveAnimation[mCurFrameNum - 1];
        }
        else {
            return mActiveAnimation.back();
        }
    }

    void ModelAnimation::GenBones() {
        mRootBones.clear();
        for(size_t i = 0; i < mBones.size(); i++) {
            Bone* parent = GetBone(mBones[i].parentName);
            if(parent) {
                parent->AddChild(i);
                mBones[i].parentID = GetBoneID(mBones[i].name);
            }
            else {
                mRootBones.push_back(i);
                mBones[i].parentID = -1;
            }
            //mBoneTransforms.push_back(glm::mat4(1.0f));
        }
        mCurFrame.bones = mBones;
        mActiveAnimation = mAnimation;
        if(mEndTime == 0.0) {
            mEndTime = 1.0;
        }
        UpdateBones();
    }
    void ModelAnimation::SetBoneTransforms(std::vector<Bone>& bones) {
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
    Bone* ModelAnimation::GetBone(string s) {
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
    void ModelAnimation::AddBone(Bone b) {
        if(!GetBone(b.name)) {
            mBones.push_back(b);
        }
    }
    void ModelAnimation::AddDeformBone(Bone b) {
        if(!GetBone(b.name)) {
            mBones.push_back(b);
        }
    }
    Bone& ModelAnimation::FindBone(string s) {
        for(auto& bone : mBones) {
            if(bone.name == s) {
                return bone;
            }
        }
        return mBones[0];
    }
    int ModelAnimation::GetBoneID(string s) {
        for(size_t i = 0; i < mBones.size(); i++) {
            if(mBones[i].name == s) {
                return i;
            }
        }
        return -1;
    }
    void ModelAnimation::BindUBO(ShaderProgram& sh, GLuint ubo, std::string name) {
        if(!mBoneTransforms || mBoneTransforms->empty()) {
            return;
        }
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        memcpy(p, &(*mBoneTransforms)[0], sizeof(glm::mat4) * mBoneTransforms->size());
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        unsigned int blockIndex = glGetUniformBlockIndex(sh.GetProgram(), name.c_str());
        size_t bindingPointIndex = 2;
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointIndex, ubo);
        glUniformBlockBinding(sh.GetProgram(), blockIndex, bindingPointIndex);
    }
    void ModelAnimation::PrintTransforms() {
        if(mBoneTransforms) {
            for(auto& t : *mBoneTransforms) {
                PrintMatrix(t);
            }
        }
    }
    void ModelAnimation::ForceFrame(size_t i) {
        if(mActiveAnimation.empty()) {
            mActiveAnimation = mAnimation;
        }
        if(i >= mActiveAnimation.size()) {
            i = 0;
        }
        if(i < 0) {
            i = mActiveAnimation.size() - 1;
        }
        mCurFrame = mActiveAnimation[i];
        mCurFrameNum = i;
        mCurrentTime = mActiveAnimation[i].timestamp;
        //SetBoneTransforms(mCurFrame.bones);
    }
    bool ModelAnimation::Finished() {
        if(mLoops == -1) {
            return false;
        }
        if(mLoopsPerformed > mLoopFrame) {
            return true;
        }
        else {
            return false;
        }
    }
    void ModelAnimation::Reset() {
        mCurrentTime = 0;
        mCurFrameNum = 0;
        mLoopsPerformed = 0;
        mActiveAnimation = mAnimation;
        mSubAnimationFrame = 0;
        mSubAnimationSize = 0;
        mCurFrame = mActiveAnimation.front();
    }
    void ModelAnimation::Write(std::ofstream& file) {
        BinaryWrite(file, mBones.size());
        for(auto b : mBones) {
            b.Write(file);
        }
        BinaryWrite(file, mCurrentTime);
        BinaryWrite(file, mEndTime);
        BinaryWrite(file, mLoopEndFrame);
    }
    void ModelAnimation::Read(std::ifstream& file) {
        size_t numBones;
        BinaryRead(file, numBones);
        mBones = std::vector<Bone>(numBones);
        for(auto& b : mBones) {
            b.Read(file);
        }
        BinaryRead(file, mCurrentTime);
        BinaryRead(file, mEndTime);
        BinaryRead(file, mLoopEndFrame);
        GenBones();
    }
    void ModelAnimation::Reverse() {
        size_t len = mAnimation.size() - 1;
        for(size_t i = 0; i < mAnimation.size() / 2; i++) {
            AnimKeyFrame temp = mAnimation[i];
            float tempTime = mAnimation[len - i].timestamp;
            mAnimation[i] = mAnimation[len - i];
            mAnimation[i].timestamp = temp.timestamp;
            mAnimation[len - i] = temp;
            mAnimation[len - i].timestamp = tempTime;
        }
    }
}

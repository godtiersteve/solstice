#include "scene3d.h"

namespace solstice {
SceneRenderer3D::SceneRenderer3D(size_t maxInstructions) {
    mInstructionsUsed = 0;
    mRenderInstructions.resize(maxInstructions);
    mLights.push_back(Light(glm::vec3(0.75, 0.75, 0.75)));
}
void SceneRenderer3D::DrawScene(ShaderProgram& shader3D, ShaderProgram& boneShader) {
    DrawSceneBones(boneShader);
    DrawScene(shader3D);
}
void SceneRenderer3D::DrawSceneBones(ShaderProgram& shader) {
    shader.UseProgram();
    shader.SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    shader.SetUniform3f("cameraPos", mCamera.position.x, mCamera.position.y, mCamera.mZoom);
    shader.SetUniformLights("lights", mLights);
    shader.SetUniform1i("numLights", mLights.size());

    for(size_t i = 0; i < mInstructionsUsed; i++) {
        int modelID = mRenderInstructions[i].mModelID;
        if(modelID >= 0 && modelID < mBoneModels.size()) {
            if(mRenderInstructions[i].mAnimationset) {
                mBoneModels[modelID].DrawAnimset(*mRenderInstructions[i].mAnimationset, shader, mTextures,
                                                 mRenderInstructions[i].mTransform);
            }
        }
    }
}
void SceneRenderer3D::DrawScene(ShaderProgram& shader) {
    shader.UseProgram();
    shader.SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    shader.SetUniform3f("cameraPos", mCamera.position.x, mCamera.position.y, mCamera.mZoom);
    shader.SetUniformLights("lights", mLights);
    shader.SetUniform1i("numLights", mLights.size());

    for(size_t i = 0; i < mInstructionsUsed; i++) {
        int modelID = mRenderInstructions[i].mModelID;
        if(modelID >= 0 && modelID < mModels.size()) {
            mModels[modelID].Draw(shader, mTextures,
                                  mRenderInstructions[i].mTransform);
        }
    }
}
void SceneRenderer3D::Update(double dt) {
    mView = mViewTransform.GetTransform();
    glm::vec3 scale = mViewTransform.GetScale();
}
void SceneRenderer3D::MoveCamera(float x, float y, float z) {
    mCamera.SetPosition(Vector2f(x, y));
    mCamera.mZoom = z;
    mView = mCamera.GetTransform();
}
void SceneRenderer3D::DrawModels(ShaderProgram& shader3D, ShaderProgram& boneShader) {
    shader3D.UseProgram();
    shader3D.SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    shader3D.SetUniform3f("cameraPos", mCamera.position.x, mCamera.position.y, mCamera.mZoom);
    shader3D.SetUniformLights("lights", mLights);
    shader3D.SetUniform1i("numLights", mLights.size());

    for(size_t i = 0; i < mModels.size(); i++) {
        mModels[i].DrawInternalTransform(shader3D, mTextures);
    }

    boneShader.UseProgram();
    boneShader.SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
    boneShader.SetUniform3f("cameraPos", mCamera.position.x, mCamera.position.y, mCamera.mZoom);
    boneShader.SetUniformLights("lights", mLights);
    boneShader.SetUniform1i("numLights", mLights.size());

    for(size_t i = 0; i < mBoneModels.size(); i++) {
        mBoneModels[i].DrawInternalTransform(boneShader, mTextures);
    }
}

void SceneRenderer3D::RenderObjects(std::vector<RenderableObject*>& objects) {
    if(mInstructionsUsed + objects.size() >= mRenderInstructions.size()) {
        mRenderInstructions.resize((mInstructionsUsed + objects.size()) * 2);
    }
    for(size_t i = 0; i < objects.size(); i++) {
        mRenderInstructions[mInstructionsUsed].mModelID = objects[i]->mModelID;
        mRenderInstructions[mInstructionsUsed].mTransform = objects[i]->mTransform.GetTransform();
        mRenderInstructions[mInstructionsUsed].mAnimationset = &(objects[i]->mAnimationSet);
        mInstructionsUsed++;
    }
}

void SceneRenderer3D::RenderObject(RenderableObject& object) {
    if(mInstructionsUsed >= mRenderInstructions.size()) {
        mRenderInstructions.resize(mRenderInstructions.size() * 2);
    }
    mRenderInstructions[mInstructionsUsed].mModelID = object.mModelID;
    mRenderInstructions[mInstructionsUsed].mTransform = object.mTransform.GetTransform();
    mRenderInstructions[mInstructionsUsed].mAnimationset = &object.mAnimationSet;
    mInstructionsUsed++;
}
void SceneRenderer3D::RenderModel(RenderInstruction instruct) {
    if(mInstructionsUsed >= mRenderInstructions.size()) {
        mRenderInstructions.resize(mRenderInstructions.size() * 2);
    }
    mRenderInstructions[mInstructionsUsed++] = instruct;
}
void SceneRenderer3D::RenderBoneModel(RenderInstruction instruct) {
    if(mInstructionsUsed >= mRenderInstructions.size()) {
        mRenderInstructions.resize(mRenderInstructions.size() * 2);
    }
    mRenderInstructions[mInstructionsUsed++] = instruct;
}
void SceneRenderer3D::LoadAnimatedModel(std::string path, std::string name) {
    mBoneModels.push_back(SkeletalModel());
    solstice::LoadModelAndAnimations(mBoneModels.back(), path, name, mTextures);
    //mBoneModels.back().GetTransform().SetScale(0.075, 0.075, 0.075);
    mBoneModels.back().SetupModel();
}

void SceneRenderer3D::LoadModel(std::string path, std::string name) {
    mModels.push_back(Model());
    solstice::LoadModel(&mModels.back(), path, mTextures, false);
    mModels.back().SetName(name);
    mModels.back().SetupModel();
    //mModels.back().GetTransform().SetScale(0.075, 0.075, 0.075);
}

int SceneRenderer3D::GetAnimatedModeLID(std::string name) {
    return -1;
}

int SceneRenderer3D::GetModelID(std::string name) {
    return -1;
}

}

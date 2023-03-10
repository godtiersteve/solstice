#include "modelloader.h"
namespace solstice {
ModelLoader::ModelLoader() {
    for(size_t i = 0; i < 20; i++) {
        mTexUniform[i] = i;
    }
    mLights.push_back(Light());
    mTimeSkip = 1.0f;
    mSelectedModel = 0;
    mShowSelectionOutline = false;
}
void ModelLoader::Setup() {
    for(auto& m : mModels) {
        m.SetupModel();
    }
}
void ModelLoader::AddModel(Model m)  {
    mModels.push_back(m);
}
void ModelLoader::AddModelTex(string path) {
    mModels.push_back(Model(path, mTextures, false));
}
void ModelLoader::InitLua(kaguya::State& state) {
    state["ModelLoader"].setClass(kaguya::UserdataMetatable<ModelLoader>()
                                  .setConstructors<ModelLoader()>()
                                  .addFunction("LoadTexture", &ModelLoader::LoadTexture)
                                  .addFunction("AddLight", &ModelLoader::AddLight)
                                  .addFunction("GetSun", &ModelLoader::GetSun)
                                  .addFunction("SetClearColor", &ModelLoader::SetBackgroundColor)
                                  .addFunction("GetTimeSkip", &ModelLoader::GetTimeSkip)
                                  .addFunction("SetTimeSkip", &ModelLoader::SetTimeSkip)
                                  .addFunction("SetOutlineSize", &ModelLoader::SetOutline)
                                  .addFunction("LoadAnimation", &ModelLoader::LoadAnimation)
                                  .addFunction("LoadAnimations", &ModelLoader::LoadAnimatedModels)
                                  .addFunction("AddAnimation", &ModelLoader::AddAnimation)
                                  .addFunction("SaveAnimationSet", &ModelLoader::SaveAnimationSet)
                                  .addFunction("SaveModel", &ModelLoader::SaveModel)
                                  .addFunction("LoadAnimationSet", &ModelLoader::LoadAnimBinary)
                                  .addFunction("LoadModelBinary", &ModelLoader::LoadModelBinary)
                                  .addOverloadedFunctions("LoadModel", static_cast<void (ModelLoader::*)(string)>(&ModelLoader::LoadModel),
                                                                       static_cast<void (ModelLoader::*)(string, string)>(&ModelLoader::LoadModel))
                                  .addOverloadedFunctions("LoadAnimatedModel", static_cast<void (ModelLoader::*)(string)>(&ModelLoader::LoadAnimatedModel),
                                                                               static_cast<void (ModelLoader::*)(string, string)>(&ModelLoader::LoadAnimatedModel))
                                  .addOverloadedFunctions("GetModel", static_cast<Model* (ModelLoader::*)(size_t)>(&ModelLoader::GetModelPtr),
                                                                      static_cast<Model* (ModelLoader::*)(string)>(&ModelLoader::GetModel))
                                  .addOverloadedFunctions("GetAnimatedModel", static_cast<SkeletalModel* (ModelLoader::*)(size_t)>(&ModelLoader::GetAnimatedModelPtr),
                                                                              static_cast<SkeletalModel* (ModelLoader::*)(string)>(&ModelLoader::GetAnimatedModel))
                                  .addOverloadedFunctions("MoveTo", static_cast<void (ModelLoader::*)(float, float, float)>(&ModelLoader::SetTranslation))
                                  .addOverloadedFunctions("SetTranslation", static_cast<void (ModelLoader::*)(float, float, float)>(&ModelLoader::SetTranslation))
                                  .addOverloadedFunctions("SetRotation", static_cast<void (ModelLoader::*)(float, float, float)>(&ModelLoader::SetRotation))
                                  .addOverloadedFunctions("Scale", static_cast<void (ModelLoader::*)(float, float, float)>(&ModelLoader::SetScale))
                                  );

    state["loader"] = this;
    state["sun"] = &mLights[0];
}
void ModelLoader::SetBackgroundColor(float r, float g, float b) {
    mClearColor = Color(r, g, b, 1.0f);
}

void ModelLoader::SaveAnimationSet(std::string model, std::string filename) {
    SkeletalModel* m = GetAnimatedModel(model);
    if(m) {
        std::ofstream file(filename);
        std::cout << "Saving " << model << " to " << filename << std::endl;
        m->Write(file);
        std::cout << "...Done\n";
        file.close();
    }
}
void ModelLoader::SaveModel(std::string model, std::string filename) {
    Model* m = GetModel(model);
    if(m) {
        std::ofstream file(filename);
        std::cout << "Saving " << model << " to " << filename << std::endl;
        m->Write(file);
        std::cout << "...Done\n";
        file.close();
    }
}
void ModelLoader::LoadAnimBinary(std::string file) {
    SkeletalModel m;
    std::ifstream f(file);
    m.Read(f);
    f.close();
    mBoneLoader.AddModel(m);
    mBoneLoader.GetModels().back().SetupModel();
}
void ModelLoader::LoadModelBinary(std::string file) {
    Model m;
    std::ifstream f(file);
    m.Read(f);
    f.close();
    mModels.push_back(m);
    mModels.back().SetupModel();
}
void ModelLoader::LoadModel(string m, string name) {
    mModels.push_back(solstice::LoadModel(m, mTextures));
    mModels.back().SetName(name);
}
void ModelLoader::LoadModelSep(string m) {
    mModels.push_back(solstice::LoadModel(m, mTextures));
}
void ModelLoader::LoadModel2(string m) {
    mModels.push_back(solstice::LoadModel(m, mTextures));
}
SkeletalModel* ModelLoader::GetSelectedAnimated() {
    if(mSelectedModel < 0 || (mModels.empty() && mBoneLoader.GetModels().empty())) {
        return NULL;
    }
    if(mModels.empty()) {
        return &mBoneLoader.GetModel(mSelectedModel);
    }
    else {
        if(mSelectedModel < mModels.size()) {
            return NULL;
        }
        else {
            return &mBoneLoader.GetModel(mSelectedModel - mModels.size());
        }
    }
    return NULL;
}
std::string ModelLoader::GetSelectedModelName() {
    if(mSelectedModel < 0 || (mModels.empty() && mBoneLoader.GetModels().empty())) {
        return "";
    }
    if(mModels.empty()) {
        return mBoneLoader.GetModel(mSelectedModel).GetName();
    }
    else {
        if(mSelectedModel < mModels.size()) {
            return mModels[mSelectedModel].GetName();
        }
        else {
            return mBoneLoader.GetModel(mSelectedModel - mModels.size()).GetName();
        }
    }
    return "";
}
void ModelLoader::IncSelectedModel() {
    if(!mShowSelectionOutline) {
        mShowSelectionOutline = true;
        return;
    }
    mSelectedModel++;
    if(!mModels.empty() && mSelectedModel >= mModels.size()) {
        if(mBoneLoader.GetModels().empty()) {
            mSelectedModel = 0;
        }
        else if(mSelectedModel >= mModels.size() + mBoneLoader.GetModels().size()) {
            mSelectedModel = 0;
        }
    }
    else if(mSelectedModel >= mBoneLoader.GetModels().size()) {
        mSelectedModel = 0;
    }
}
void ModelLoader::DecSelectedModel() {
    if(!mShowSelectionOutline) {
        mShowSelectionOutline = true;
        return;
    }
    mSelectedModel--;
    if(mSelectedModel < 0) {
        if(!mBoneLoader.GetModels().empty()) {
            if(!mModels.empty()) {
                mSelectedModel = mBoneLoader.GetModels().size() + mModels.size() - 1;
            }
            else {
                mSelectedModel = mBoneLoader.GetModels().size() -1;
            }
        }
        else {
            mSelectedModel = -1;
        }
    }
}
void ModelLoader::ToggleSelectionHide() {
    if(mSelectedModel >= 0 && !mModels.empty() && mSelectedModel < mModels.size()) {
        mModels[mSelectedModel].SetHidden(!mModels[mSelectedModel].GetHidden());
    }
    if(mModels.empty() && mSelectedModel < mBoneLoader.GetModels().size()) {
        mBoneLoader.GetModel(mSelectedModel).SetHidden(!mBoneLoader.GetModel(mSelectedModel).GetHidden());
    }
    if(!mModels.empty() && mSelectedModel >= mModels.size() && mSelectedModel - mModels.size() < mBoneLoader.GetModels().size()) {
        mBoneLoader.GetModel(mSelectedModel - mModels.size()).SetHidden(!mBoneLoader.GetModel(mSelectedModel - mModels.size()).GetHidden());
    }
}
void ModelLoader::LoadAnimatedModel(string m, string name) {
    mBoneLoader.LoadModel(m, mTextures, true, true);
    mBoneLoader.GetModels().back().SetName(name);
}
void ModelLoader::LoadAnimatedModelSep(string m) {
    mBoneLoader.LoadModel(m, mTextures, false, true);
}
void ModelLoader::LoadTexture(string tex, string ref) {
    mTextures.LoadImg(tex, ref);
}
void ModelLoader::LoadAnimation(string file, string model) {
    mBoneLoader.LoadAnimation(file, model);
}
void ModelLoader::LoadAnimatedModels(string path, string prefix, string ext) {
    mBoneLoader.LoadAnimatedModels(path, prefix, ext, mTextures);
}
void ModelLoader::AddAnimation(string file, string model, string anim) {
    mBoneLoader.AddAnimation(file, model, anim);
}
void ModelLoader::clear() {
    mTextures.clear();
    for(auto& m : mModels) {
        m.clear();
    }
    mBoneLoader.clear();
}
void ModelLoader::Draw(ShaderProgram& sh) {
    for(size_t i = 0; i < mModels.size(); i++) {
        mModels[i].Draw(sh, mTextures);
    }
}
void ModelLoader::Draw(ShaderProgram& sh, TextureArray& textures) {
    for(size_t i = 0; i < mModels.size(); i++) {
        mModels[i].Draw(sh, mTextures);
    }
}
void ModelLoader::DrawAnimated(ShaderProgram& sh) {
    for(auto& m : mBoneLoader.GetModels()) {
        m.Draw(sh, mTextures);
    }
}
void ModelLoader::DrawAnimatedOutline(ShaderProgram& sh) {
    for(size_t i = 0; i < mBoneLoader.GetModels().size(); i++) {
        if(mShowSelectionOutline) {
            if(mModels.empty() && mSelectedModel == i) {
                sh.SetUniform4f("outlineColor", 1.0f, 0.0f, 0.0f, 1.0f);
            }
            else if(!mModels.empty() && mSelectedModel == mModels.size() + i) {
                sh.SetUniform4f("outlineColor", 1.0f, 0.0f, 0.0f, 1.0f);
            }
            else {
                sh.SetUniform4f("outlineColor", 0.0f, 0.0f, 0.0f, 0.0f);
            }
        }
        else {
            sh.SetUniform4f("outlineColor", 0.0f, 0.0f, 0.0f, 0.0f);
        }
        mBoneLoader.GetModel(i).DrawOutline(sh, mTextures);
    }
}
void ModelLoader::Update(float t) {
    for(size_t i = 0; i < mBoneLoader.GetModels().size(); i++) {
        mBoneLoader.GetModel(i).Update(t * mTimeSkip);
    }
}
void ModelLoader::DrawOutline(ShaderProgram& sh) {
    for(size_t i = 0; i < mModels.size(); i++) {
        if(mShowSelectionOutline && i == mSelectedModel) {
            sh.SetUniform4f("outlineColor", 1.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            sh.SetUniform4f("outlineColor", 0.0f, 0.0f, 0.0f, 0.0f);
        }
        mModels[i].DrawOutline(sh, mTextures);
    }
}
void ModelLoader::SetTranslation(glm::vec3 trans) {
    mTransform.SetTranslation(trans);
}
void ModelLoader::SetTranslation(float x, float y, float z) {
    SetTranslation(glm::vec3(x, y, z));
}
void ModelLoader::SetRotation(float x, float y, float z) {
    mTransform.SetAngles(x, y, z);
}
void ModelLoader::SetRotation(glm::vec3 rot) {
    mTransform.SetRotation(rot);
}
void ModelLoader::SetScale(float x, float y, float z) {
    SetScale(glm::vec3(x, y, z));
}
void ModelLoader::SetScale(glm::vec3 scale) {
    mTransform.SetScale(scale);
}
void ModelLoader::AddLight(Light l) {
    if(mLights.size() < 20)
        mLights.push_back(l);
}
void ModelLoader::SetTransforms() {
    SetScale(mTransform.GetScale());
    SetRotation(mTransform.GetRotation());
    SetTranslation(mTransform.GetTranslation());
}
void ModelLoader::SetOutline(float s) {
    for(auto& model : mModels) {
        model.SetOutlineSize(s);
    }
    for(size_t i = 0; i < mBoneLoader.GetModels().size(); i++) {
        mBoneLoader.GetModel(i).SetOutlineSize(s);
    }
}
Model* ModelLoader::GetModel(std::string name) {
    for(size_t i = 0; i < mModels.size(); i++) {
        if(mModels[i].GetName() == name) {
            return &mModels[i];
        }
    }
    return NULL;
}
SkeletalModel* ModelLoader::GetAnimatedModel(std::string name) {
    for(size_t i = 0; i < mBoneLoader.GetModels().size(); i++) {
        if(mBoneLoader.GetModel(i).GetName() == name) {
            return &mBoneLoader.GetModel(i);
        }
    }
    return NULL;
}
}

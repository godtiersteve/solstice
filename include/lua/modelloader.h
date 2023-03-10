#ifndef MODELLOADER_H_INCLUDED
#define MODELLOADER_H_INCLUDED

#include "kaguya/kaguya.hpp"
#include "graphics/3d/model.h"
#include "graphics/3d/skeletalmodel.h"
#include "graphics/3d/light.h"
#include "loader/assimpload.h"
#include "loader/bone_loader.h"
#include "luabinds.h"

#include <vector>

namespace solstice {
class ModelLoader {
private:
    std::vector<Model> mModels;
    BoneLoader mBoneLoader;
    std::vector<Light> mLights;
    TextureArray mTextures;
    GLint mTexUniform[20];
    ModelTransform mTransform;
    void SetTransforms();
    Color mClearColor;
    float mTimeSkip;
    int mSelectedModel;
    bool mShowSelectionOutline;
public:
    ModelLoader();
    void InitLua(kaguya::State& state);
    void AddModelTex(string path);
    void AddModel(Model m);
    void LoadModel(string m, string name);
    void LoadModel(string m) {LoadModel(m, "");}
    void LoadModelSep(string m);
    void LoadModel2(string m);
    void LoadAnimatedModel(string m, string name);
    void LoadAnimatedModel(string m) {LoadAnimatedModel(m, "");}
    void LoadAnimatedModels(string path, string prefix, string ext);
    void LoadAnimation(string file, string model);
    void AddAnimation(string file, string model, string anim);
    void LoadAnimatedModelSep(string m);
    void LoadTexture(string tex, string ref = "");
    void SetOutline(float s);
    void SaveAnimationSet(std::string model, std::string filename);
    void SaveModel(std::string model, std::string filename);
    void LoadAnimBinary(std::string file);
    void LoadModelBinary(std::string file);
    std::vector<Model>& GetModels()                         {return mModels;}
    std::vector<SkeletalModel>& GetAnimatedModels()          {return mBoneLoader.GetModels();}
    Model&              GetModel(size_t i)                  {return mModels[i];}
    Model*              GetModelPtr(size_t i)               {return i >= 0 && i < mModels.size() ? &mModels[i] : NULL;}
    Model*              GetModel(std::string name);
    SkeletalModel*      GetAnimatedModel(std::string name);
    SkeletalModel*      GetAnimatedModelPtr(size_t i)       {return &mBoneLoader.GetModel(i);}
    SkeletalModel*      GetSelectedAnimated();
    SkeletalModel&      GetAnimatedModel(size_t i)          {return mBoneLoader.GetModel(i);}
    TextureArray&       GetTextureArray()                   {return mTextures;}
    ModelTransform&     GetTransform()                      {return mTransform;}
    std::vector<Light>&  GetLights()                        {return mLights;}
    Light& GetSun()                                         {return mLights[0];}
    Color& GetClearColor()                                  {return mClearColor;}
    Light& GetLight(size_t i)                               {return mLights[i];}
    void SetSelectedModel(int i)                            {mSelectedModel = i;}
    int GetSelectedModel()                                  {return mSelectedModel;}
    void IncSelectedModel();
    void DecSelectedModel();
    bool& GetShowSelectionOutline()                             {return mShowSelectionOutline;}
    std::string GetSelectedModelName();
    void ToggleSelectionHide();
    void Setup();
    void Draw(ShaderProgram& sh);
    void Draw(ShaderProgram& sh, TextureArray& textures);
    void DrawWithSelection(size_t model, size_t mesh, ShaderProgram& sh, TextureArray& textures);
    void DrawOutline(ShaderProgram& sh);
    void DrawAnimated(ShaderProgram& sh);
    void DrawAnimatedOutline(ShaderProgram& sh);
    void SetTranslation(glm::vec3 trans);
    void SetTranslation(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetRotation(glm::vec3 rot);
    void SetScale(glm::vec3 scale);
    void SetScale(float x, float y, float z);
    void SetBackgroundColor(float r, float g, float b);

    void AddLight(Light l);
    bool empty() {return mModels.empty();}
    void clear();
    void Update(float t);
    float GetTimeSkip() {return mTimeSkip;}
    void SetTimeSkip(float t) {mTimeSkip = t;}
};
}

#endif // MODELLOADER_H_INCLUDED

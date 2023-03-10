#ifndef SCENE3D_H_INCLUDED
#define SCENE3D_H_INCLUDED

#include "3d/light.h"
#include "shaderprogram.h"
#include "renderableobject.h"
#include "renderinstruction.h"

#include "loader/bone_loader.h"
#include "player/camera2d.h"

namespace solstice {
    class SceneRenderer3D {
    private:
        std::vector<SkeletalModel> mBoneModels;
        std::vector<Model> mModels;
        TextureArray mTextures;
        size_t mInstructionsUsed;
        std::vector<RenderInstruction> mRenderInstructions;
        glm::mat4 mView;
    public:
        float mXNorm, mYNorm;
        std::vector<Light> mLights;
        Camera mCamera;
        ModelTransform mViewTransform;
        glm::mat4 mProjectionMatrix;
        SceneRenderer3D(size_t maxInstructions = 400);

        void Update(double dt);

        void DrawScene(ShaderProgram& shader3D, ShaderProgram& boneShader);
        void DrawSceneBones(ShaderProgram& shader);
        void DrawScene(ShaderProgram& shader);
        void DrawModels(ShaderProgram& shader3D, ShaderProgram& boneShader);
        void RenderObjects(std::vector<RenderableObject*>& objects);
        void ClearRenderList() {mInstructionsUsed = 0;}
        void RenderObject(RenderableObject& object);
        void RenderModel(RenderInstruction instruct);
        void RenderBoneModel(RenderInstruction instruct);

        void LoadAnimatedModel(std::string path, std::string name);
        void LoadModel(std::string path, std::string name);

        void LoadTexture(std::string texture, std::string ref) {mTextures.LoadImg(texture, ref);}
        void AddTexture(Texture texture)    {mTextures.AddTexture(texture);}

        Texture GetTexture(size_t i) {return mTextures[i];}
        SkeletalModel& GetBoneModel(size_t i) {return mBoneModels[i];}
        Model& GetModel(size_t i) {return mModels[i];}

        int GetAnimatedModeLID(std::string name);
        int GetModelID(std::string name);
        void MoveCamera(float x, float y, float z);
        glm::mat4 GetViewTransform() {return mViewTransform.GetTransform();}
    };

}

#endif // SCENE3D_H_INCLUDED

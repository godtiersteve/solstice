#ifndef RENDERABLEOBJECT_H_INCLUDED
#define RENDERABLEOBJECT_H_INCLUDED

#include "graphics/modeltransform.h"
#include "graphics/shader.h"
#include "graphics/texarray.h"
#include "graphics/3d/model.h"
#include "graphics/3d/modelanimset.h"

namespace solstice {
    class RenderableObject {
    protected:
        void UpdateScale();
        void PivotTowardsTarget();
    public:
        glm::mat4* mTransformMatrix;
        static float sXNorm, sYNorm;
        float mZPos;
        int mModelID;
        float mPivotAngle;
        Vector2f mModelOffset;
        bool mXFlipped, mYFlipped, mZFlipped;
        float mTargetAngle, mPivotStep, mBasePivotAngle;

        ModelTransform mTransform;

        ModelAnimationset mAnimationSet;
        RenderableObject(int modelID = -1, float zPos = -3.0, glm::mat4* transform = nullptr);
        virtual void Draw(ShaderProgram& sh, TextureArray& textures, std::vector<Model>& models, Vector2f pos);
        void UpdateTransforms(Vector2f pos, float angle = 0);
        void Unproject(glm::vec3 pos, glm::mat4 modelView, glm::mat4 proj);
        void SetPivotAngle(float a) {mPivotAngle = a; mTargetAngle = a; mBasePivotAngle = a;}
    };
}

#endif // RENDERABLEOBJECT_H_INCLUDED

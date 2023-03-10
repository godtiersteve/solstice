#include "renderableobject.h"

namespace solstice {
    float RenderableObject::sXNorm = 1.0;
    float RenderableObject::sYNorm = 1.0;
    RenderableObject::RenderableObject(int modelID, float zPos, glm::mat4* transform):
    mModelID(modelID), mTransformMatrix(transform)
    {
        mZPos = 0.0;
        mPivotAngle = PI * 0.5;
        mTargetAngle = PI * 0.5;
        mBasePivotAngle = PI * 0.5;
        mXFlipped = false;
        mYFlipped = false;
        mZFlipped = false;
        mPivotStep = RADIAN * 10;
    }

    void RenderableObject::Draw(ShaderProgram& sh, TextureArray& textures, std::vector<Model>& models, Vector2f pos) {
        if(mModelID >= 0) {
            glm::vec3 normalizedPos(pos.x * sXNorm, pos.y * sYNorm, mZPos);
            models[mModelID].GetTransform().SetTranslation(normalizedPos);
            glm::vec3 scale = models[mModelID].GetTransform().GetScale();
            UpdateScale();
            models[mModelID].GetTransform().SetScale(scale);
            glm::mat4 translation = models[mModelID].GetTransform().GetTransform();
            models[mModelID].Draw(sh, textures, translation);
        }
    }
    void RenderableObject::UpdateTransforms(Vector2f pos, float zAngle) {
        UpdateScale();
        mTransform.SetTranslation(pos.x * sXNorm, pos.y * sYNorm, mZPos);
        mTransform.SetAngles(mPivotAngle, 0, zAngle);
    }
    void RenderableObject::PivotTowardsTarget() {
        if(mPivotAngle < mTargetAngle) {
            mPivotAngle = std::min(mTargetAngle, mPivotAngle + mPivotStep);
        }
        else if(mPivotAngle > mTargetAngle) {
            mPivotAngle = std::max(mTargetAngle, mPivotAngle - mPivotStep);
        }
    }
    void RenderableObject::UpdateScale() {
        glm::vec3 scale = mTransform.GetScale();
        if(mXFlipped) {
            scale.x = -fabs(scale.x);
        }
        else {
            scale.x = fabs(scale.x);
        }
        if(mYFlipped) {
            scale.y = -fabs(scale.y);
        }
        else {
            scale.y = fabs(scale.y);
        }
        if(mZFlipped) {
            scale.z = -fabs(scale.z);
        }
        else {
            scale.z = fabs(scale.z);
        }
        mTransform.SetScale(scale);
    }
    void RenderableObject::Unproject(glm::vec3 pos, glm::mat4 modelView, glm::mat4 proj) {
        glm::vec4 viewport(-640, -360, 1280, 720);
        pos = glm::unProject(pos,
                             glm::inverse(proj * modelView),
                             proj,
                             viewport);
        mTransform.SetTranslation(pos);
        printf("x: %f, y: %f, z: %f\n", pos.x, pos.y, pos.z);
    }
}

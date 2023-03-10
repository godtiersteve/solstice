#include "modeltransform.h"
namespace solstice {
    ModelTransform::ModelTransform() {
        mScale = glm::vec3(1.0f, 1.0f, 1.0f);
        mTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
        mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
        mUpdateRot = true;
        mUpdateTrans = true;
        mUpdateScale = true;
    }
    void ModelTransform::MultiplyScale(float x, float y, float z) {
        mScale.x*= x;
        mScale.y*=y;
        mScale.z*=z;
    }

    glm::mat4 ModelTransform::GetScaleMatrix() {
        if(mUpdateScale) {
            mScaleMat = glm::scale(mScale);
            mUpdateScale = false;
        }
        return mScaleMat;
    }
    glm::mat4 ModelTransform::GetRotationMatrix() {
        if(mUpdateRot) {
            mRotationMat = glm::yawPitchRoll(mRotation.x, mRotation.y, mRotation.z);
            mUpdateRot = false;
        }
        return mRotationMat;
    }
    glm::mat4 ModelTransform::GetTranslationMatrix() {
        if(mUpdateTrans) {
            mTranslationMat = glm::translate(mTranslation);
            mUpdateTrans = false;
        }
        return mTranslationMat;
    }
    glm::mat4 ModelTransform::GetTransform() {
        if(mUpdateRot || mUpdateScale || mUpdateTrans) {
            if(!mRotation.x && !mRotation.y && !mRotation.z) {
                if(mScale.x || mScale.y || mScale.z) {
                    mTransformation = GetTranslationMatrix() * GetScaleMatrix();
                }
                else {
                    mTransformation = GetTranslationMatrix();
                }
            }
            else {
                if(mScale.x || mScale.y || mScale.z) {
                    mTransformation =GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
                }
                else {
                    mTransformation = GetTranslationMatrix() * GetRotationMatrix();
                }
            }
        }
        return mTransformation;
    }
    void ModelTransform::Write(std::ofstream& file) {
        BinaryWriteVec3(file, mScale);
        BinaryWriteVec3(file, mTranslation);
        BinaryWriteVec3(file, mRotation);
    }
    void ModelTransform::Read(std::ifstream& file) {
        BinaryReadVec3(file, mScale);
        BinaryReadVec3(file, mTranslation);
        BinaryReadVec3(file, mRotation);
    }
}

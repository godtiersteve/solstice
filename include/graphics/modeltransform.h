#ifndef MODELTRANSFORM_H_INCLUDED
#define MODELTRANSFORM_H_INCLUDED

#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "graphics/vector2.h"

namespace solstice {
struct ModelTransform {
private:
    glm::mat4 mTransformation;
    bool mUpdateScale, mUpdateTrans, mUpdateRot;
    glm::mat4 mRotationMat, mTranslationMat, mScaleMat;
    glm::vec3 mScale;
    glm::vec3 mTranslation;
    glm::vec3 mRotation;
public:
    ModelTransform();
    glm::mat4 GetScaleMatrix();
    glm::mat4 GetRotationMatrix();
    glm::mat4 GetTranslationMatrix();
    glm::mat4 GetTransform();
    void MultiplyScale(float x, float y, float z);
    void SetAngles(float x, float y, float z)           {mRotation = glm::vec3(x, y, z); mUpdateRot = true;}
    void SetRotation(float x, float y, float z)         {mRotation = glm::vec3(x, y, z); mUpdateRot = true;}
    void SetRotation(glm::vec3 rot)                     {mRotation = rot; mUpdateRot = true;}
    void SetScale(glm::vec3 scale)                      {mScale = scale; mUpdateScale = true;}
    void SetScale(float x, float y, float z)            {SetScale(glm::vec3(x, y, z));}
    void SetTranslation(glm::vec3 trans)                {mTranslation = trans; mUpdateTrans = true;}
    void SetTranslation(float x, float y, float z)      {SetTranslation(glm::vec3(x, y, z));}
    glm::vec3 GetRotation() const                          {return mRotation;}
    glm::vec3 GetScale() const                               {return mScale;}
    glm::vec3 GetTranslation() const                      {return mTranslation;}
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
};
}

#endif // MODELTRANSFORM_H_INCLUDED

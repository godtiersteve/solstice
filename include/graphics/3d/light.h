#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "graphics/color.h"

namespace solstice {
class Light {
public:
    glm::vec3 mAmbient, mDiffuse, mSpecular;
    glm::vec4 mPosition, mDirection;
    float mConstant, mLinear, mQuadratic;
    float mSpecExponent, mSpotCutoff;
    Light();
    Light(glm::vec3 ambient);
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec4 position);

    glm::vec3& GetAmbient()             {return mAmbient;}
    glm::vec3& GetDiffuse()             {return mDiffuse;}
    glm::vec3& GetSpecular()            {return mSpecular;}
    glm::vec4& GetDirection()           {return mDirection;}
    glm::vec4& GetPosition()            {return mPosition;}
    float GetSpecExponent()             {return mSpecExponent;}
    float GetLinear()                   {return mLinear;}
    float GetConstant()                 {return mConstant;}
    float GetQuadratic()                {return mQuadratic;}
    void SetSpecExponent(float e)       {mSpecExponent = e;}
    void SetAmbient(float r, float g, float b);
    void SetDiffuse(float r, float g, float b);
    void SetSpecular(float r, float g, float b);
    void SetDirection(float x, float y, float z, float w = 0.0);
    void SetDirectionAxis(float w, float x, float y, float z);
    void SetDirectionAxisDeg(float w, float x, float y, float z);
    void SetPoint(float constant, float linear, float quadratic);
    void SetAttenuation(float constant, float linear, float quadratic);
    void SetPosition(glm::vec3 pos);
    void SetPosition(float x, float y, float z);
    void SetSpotCutoff(float s)                 {mSpotCutoff = s;}
    float GetSpotCutoff()                       {return mSpotCutoff;}
    Color GetColor()                {return mAmbient + mDiffuse + mSpecular;}
    void PresetAttenuation(int val);
};
}

#endif // LIGHT_H_INCLUDED

#include "light.h"
namespace solstice {
    Light::Light() {
        mDirection = glm::vec4(1.0f, 0.1f, 0.01f, 91.0f);
        mSpecExponent = 2.0f;
        mSpotCutoff = PI * 0.5 + RADIAN;
        mConstant = 1.0;
        mLinear = 0.1;
        mQuadratic = 0.01;
    }
    Light::Light(glm::vec3 ambient) {
        mAmbient = ambient;
        mDirection = glm::vec4(1.0f, 0.1f, 0.01f, 91.0f);
        mSpecExponent = 2.0f;
        mSpotCutoff = PI * 0.5 + RADIAN;
        mConstant = 1.0;
        mLinear = 0.1;
        mQuadratic = 0.01;
    }
    Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec4 position) {
        mAmbient = ambient;
        mDiffuse = diffuse;
        mSpecular = specular;
        mPosition = position;
        mConstant = 1.0;
        mLinear = 0.1;
        mQuadratic = 0.01;
    }
    void Light::SetAmbient(float r, float g, float b) {
        mAmbient = glm::vec3(r, g, b);
    }
    void Light::SetDiffuse(float r, float g, float b) {
        mDiffuse = glm::vec3(r, g, b);
    }
    void Light::SetSpecular(float r, float g, float b) {
        mSpecular = glm::vec3(r, g, b);
    }
    void Light::SetDirection(float x, float y, float z, float w) {
        glm::vec3 dirNormal = glm::normalize(glm::vec3(x, y, z));
        mDirection = glm::vec4(dirNormal.x, dirNormal.y, dirNormal.z, w);
    }
    void Light::SetDirectionAxis(float w, float x, float y, float z) {
        glm::vec3 angleAxis = glm::vec3(x, y, z);
        glm::quat dir = glm::angleAxis(w, angleAxis);
        glm::vec3 axis = glm::axis(dir);
        mDirection.x = axis.x;
        mDirection.y = axis.y;
        mDirection.z = axis.z;
        mDirection.w = 0.0;
    }
    void Light::SetDirectionAxisDeg(float w, float x, float y, float z) {
        glm::vec3 angleAxis = glm::vec3(x, y, z);
        glm::quat dir = glm::angleAxis(glm::radians(w), angleAxis);
        glm::vec3 axis = glm::axis(dir);
        mDirection.x = axis.x;
        mDirection.y = axis.y;
        mDirection.z = axis.z;
        mDirection.w = 0.0;
    }
    void Light::SetPoint(float constant, float linear, float quadratic) {
        mDirection.w = 0.0;
        SetAttenuation(constant, linear, quadratic);
    }
    void Light::SetAttenuation(float constant, float linear, float quadratic) {
        mConstant = constant;
        mLinear = linear;
        mQuadratic = quadratic;
    }
    void Light::SetPosition(glm::vec3 pos) {
        mPosition = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
    }
    void Light::SetPosition(float x, float y, float z) {
        mPosition = glm::vec4(x, y, z, 1.0f);
    }
    void Light::PresetAttenuation(int val) {
        switch(val) {
        case 0:
            SetAttenuation(1.0, 0.0014, 0.0002);
            break;
        case 1:
            SetAttenuation(1.0, 0.007, 0.0007);
            break;
        case 2:
            SetAttenuation(1.0, 0.014, 0.0019);
            break;
        case 3:
            SetAttenuation(1.0, 0.022, 0.028);
            break;
        case 4:
            SetAttenuation(1.0, 0.027, 0.0075);
            break;
        case 5:
            SetAttenuation(1.0, 0.045, 0.017);
            break;
        case 6:
            SetAttenuation(1.0, 0.07, 0.032);
            break;
        case 7:
            SetAttenuation(1.0, 0.14, 0.07);
            break;
        case 8:
            SetAttenuation(1.0, 0.22, 0.2);
            break;
        case 9:
            SetAttenuation(1.0, 0.35, 0.44);
            break;
        case 10:
            SetAttenuation(1.0, 0.7, 1.8);
            break;
        default:
            break;
        }
    }
}

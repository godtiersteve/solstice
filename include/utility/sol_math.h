#ifndef SOL_MATH_H_INCLUDED
#define SOL_MATH_H_INCLUDED

#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <float.h>
#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
//#include <boost/random.hpp>
//#include <boost/random/random_device.hpp>

const double PI = 3.1415926535897932384626433;
const double TWO_PI = PI * 2.0;
const double RADIAN = PI / 180;
typedef unsigned int uint;
typedef unsigned short ushort;
namespace solstice {
    int NextPowerOfTwo(int i);
    int Square(int x);
    bool SameSign(float x, float y);
    float CheckSign(float x, float y);
    float Randf(std::default_random_engine& rng, float min, float max);
    int Randi(std::default_random_engine& rng, int min, int max);
    float Randf(float min, float max);
    glm::vec2 RandVec2(glm::vec2 min, glm::vec2 max);
    glm::vec3 RandVec3(glm::vec3 min, glm::vec3 max);
    int Randi(int min, int max);
    unsigned int Randui(unsigned int min, unsigned int max);
    double Randd(double min, double max);
    unsigned int Randui(std::default_random_engine& rng, unsigned int min, unsigned int max);
    float Trunc(float value, float digits = 2);
    float SetZero(float x, float min, float max);
    double WrapMax(double x, double max);
    double WrapMinMax(double x, double min, double max);
    double WrapAngle(double a);
    float glmod(float a, float b);
    float DeltaAngle(float a1, float a2);
    float mod2(float a, float b);
    float DegreesToRadians(float angle);
    float RadiansToDegrees(float angle);
    double WrapToTwoPI(double angle);
    double WrapToPi(double angle);
    float AdjustAngle(float angle, float step, float targetAngle);
    double RotateAngle(double sourceAngle, double destAngle, double step);
    float RotateToZero(double angle, double step);
    glm::vec3 PointTowards(glm::vec3 orig, glm::vec3 dest);

    float Constrainf(float val,  float min, float max);
    int Constrainti(int val, int min, int max);
    size_t Constrainui(size_t val, size_t min, size_t max);
    glm::vec2 Constrain2f(glm::vec2 val, glm::vec2 min, glm::vec2 max);
    glm::vec3 Constrain3f(glm::vec3 val, glm::vec3 min, glm::vec3 max);
    glm::vec4 Constrain4f(glm::vec4 val, glm::vec4 min, glm::vec4 max);

    float ConstrainMinf(float val,  float min, float max);
    int ConstraintMini(int val, int min, int max);
    size_t ConstrainMinui(size_t val, size_t min, size_t max);
    glm::vec2 ConstrainMin2f(glm::vec2 val, glm::vec2 min, glm::vec2 max);
    glm::vec3 ConstrainMin3f(glm::vec3 val, glm::vec3 min, glm::vec3 max);
    glm::vec4 ConstrainMin4f(glm::vec4 val, glm::vec4 min, glm::vec4 max);
    glm::vec2 Project(glm::vec4 pos, glm::mat4 modelView, glm::mat4 proj);
    glm::vec4 ProjectPoint(glm::vec4 pt, glm::mat4 proj, float norm);
    glm::vec3 UnProjectPoint(glm::vec3 pt, glm::mat4 modelview, glm::mat4 proj, glm::vec4 viewport, glm::vec2 norm);
};

#endif // SOL_MATH_H_INCLUDED

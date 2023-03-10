#include "sol_math.h"

namespace solstice {
    int NextPowerOfTwo(int i) {
        int ret = 1;
        while(ret < i) {
            ret <<= 1;
        }
        return ret;
    }
    int Square(int x) {return x * x;}

    float Trunc(float value, float digits) {
        float d = pow(10, digits);
        float ret = round(value * d) / d;
        return ret;
    }
    bool SameSign(float x, float y) {
        return (x < 0 && y < 0) || (x > 0 && y > 0);
    }
    float CheckSign(float x, float y) {
        if((x < 0 && y < 0) || (x > 0 && y > 0)) {
            return -y;
        }
        else {
            return y;
        }
    }
    float Randf(std::default_random_engine& rng, float min, float max) {
        if(min == max) {
            return min;
        }
        else {
            std::uniform_real_distribution<float> dist(min,  std::nextafter(max, FLT_MAX));
            return dist(rng);
        }
    }
    int Randi(std::default_random_engine& rng, int min, int max) {
        if(min == max) {
            return min;
        }
        else {
            std::uniform_int_distribution<int> dist(min,  std::nextafter(max, INT_MAX));
            return dist(rng);
        }
    }
    glm::vec2 RandVec2(glm::vec2 min, glm::vec2 max) {
        return glm::vec2(Randf(min.x, max.x), Randf(min.y, max.y));
    }
    glm::vec3 RandVec3(glm::vec3 min, glm::vec3 max) {
        return glm::vec3(Randf(min.x, max.x), Randf(min.y, max.y), Randf(min.z, max.z));
    }
    float Randf(float min, float max) {
        static std::default_random_engine re{};
        using Dist = std::uniform_real_distribution<float>;
        static Dist uid {};
        return uid(re, Dist::param_type{min,  std::nextafter(max, FLT_MAX)});
        //static boost::random::random_device rng;
        //boost::random::uniform_real_distribution<> dist(min, max);
        //return dist(rng);
    }
    int Randi(int min, int max) {
        static std::default_random_engine re{};
        using Dist = std::uniform_int_distribution<int>;
        static Dist uid {};
        return uid(re, Dist::param_type{min,  std::nextafter(max, INT_MAX)});
    }
    unsigned int Randui(unsigned int min, unsigned int max) {
        static std::default_random_engine re{};
        using Dist = std::uniform_int_distribution<unsigned int>;
        static Dist uid {};
        return uid(re, Dist::param_type{min,  std::nextafter(max, INT_MAX)});
    }
    double Randd(double min, double max) {
        static std::default_random_engine re{};
        using Dist = std::uniform_real_distribution<double>;
        static Dist uid {};
        return uid(re, Dist::param_type{min, std::nextafter(max, DBL_MAX)});
    }
    unsigned int Randui(std::default_random_engine& rng, unsigned int min, unsigned int max) {
        if(min == max) {
            return min;
        }
        else {
            std::uniform_int_distribution<unsigned int> dist(min, max);
            return dist(rng);
        }
    }
    float SetZero(float x, float min, float max) {
        if(x < min && x > max) {
            return 0;
        }
        return x;
    }
    double WrapAngle(double a) {
        return WrapMinMax(a, -PI, PI);
    }
    double WrapMax(double x, double max) {
        return fmod(max + fmod(x, max), max);
    }
    double WrapMinMax(double x, double min, double max) {
        return min + WrapMax(x - min, max - min);
    }
    float DegreesToRadians(float angle) {
        return angle * RADIAN;
    }
    float RadiansToDegrees(float angle) {
        return angle * 180 / PI;
    }
    float glmod(float a, float b) {
        return a - b * floor(a / b);
    }
    float mod2(float a, float b) {
        if(b) {
            return a - floor(a / b) * b;
        }
        else {
            return 0;
        }
    }
    float DeltaAngle(float a1, float a2) {
        float delta = a2 - a1;
        return mod2(delta + PI, TWO_PI) - PI;
    }
    float AdjustAngle(float angle, float step, float targetAngle) {
        if(angle > targetAngle) {
            angle = std::min(targetAngle, angle - step);
        }
        if(angle < -targetAngle) {
            angle = std::max(targetAngle, angle + step);
        }
        return angle;
    }
    double WrapToTwoPi(double angle) {
        double a = fmod(angle, TWO_PI);
        if(a < 0) {
            a+= TWO_PI;
        }
        return a;
    }
    double WrapToPi(double angle) {
        double a = (angle + PI / 2, TWO_PI);
        if(a < 0) {
            a+= TWO_PI;
        }
        return a - PI;
    }
    float RotateToZero(double angle, double step) {
        float a = WrapToTwoPi(angle);
        float targetAngle = floor(a / (PI / 2)) * PI / 2;
        if(fabs(a - targetAngle) < step) {
            return targetAngle;
        }
        if(a - targetAngle < PI) {
            if(a < targetAngle) {
                a+= step;
            }
            else {
                a-= step;
            }
        }
        else {
            if(a < targetAngle) {
                a-= step;
            }
            else {
                a+= step;
            }
        }
        return a;
    }
    double RotateAngle(double sourceAngle, double destAngle, double step) {
        if(sourceAngle < destAngle) {
            return sourceAngle + step;
        }
        else if(sourceAngle > destAngle) {
            return sourceAngle - step;
        }
        return sourceAngle;
    }
    float Constrainf(float val,  float min, float max) {
        if(val > max) {
            val = max;
        }
        else if(val < min) {
            val = min;
        }
        return val;
    }
    int Constrainti(int val, int min, int max) {
        if(val > max) {
            val = max;
        }
        else if(val < min) {
            val = min;
        }
        return val;
    }
    size_t Constrainui(size_t val, size_t min, size_t max) {
        if(val > max) {
            val = max;
        }
        else if(val < min) {
            val = min;
        }
        return val;
    }
    glm::vec2 Constrain2f(glm::vec2 val, glm::vec2 min, glm::vec2 max) {
        val.x = Constrainf(val.x, min.x, max.x);
        val.y = Constrainf(val.y, min.y, max.y);
        return val;
    }
    glm::vec3 Constrain3f(glm::vec3 val, glm::vec3 min, glm::vec3 max) {
        val.x = Constrainf(val.x, min.x, max.x);
        val.y = Constrainf(val.y, min.y, max.y);
        val.z = Constrainf(val.z, min.z, max.z);
        return val;
    }
    glm::vec4 Constrain4f(glm::vec4 val, glm::vec4 min, glm::vec4 max) {
        val.x = Constrainf(val.x, min.x, max.x);
        val.y = Constrainf(val.y, min.y, max.y);
        val.z = Constrainf(val.z, min.z, max.z);
        val.w = Constrainf(val.w, min.w, max.w);
        return val;
    }

    float ConstrainMinf(float val,  float min, float max) {
        if(val < 0.0 && val > min) {
            val+= min;
        }
        else if(val > 0.0 && val < max) {
            val+= max;
        }
        return val;
    }
    int ConstraintMini(int val, int min, int max) {
        if(val < 0 && val > min) {
            val+= min;
        }
        else if(val > 0 && val < max) {
            val+= max;
        }
        return val;
    }
    size_t ConstrainMinui(size_t val, size_t min, size_t max) {
        if(val < 0 && val > min) {
            val+= min;
        }
        else if(val > 0 && val < max) {
            val+= max;
        }
        return val;
    }
    glm::vec2 ConstrainMin2f(glm::vec2 val, glm::vec2 min, glm::vec2 max) {
        val.x = ConstrainMinf(val.x, min.x, max.x);
        val.y = ConstrainMinf(val.y, min.y, max.y);
        return val;
    }
    glm::vec3 ConstrainMin3f(glm::vec3 val, glm::vec3 min, glm::vec3 max) {
        val.x = ConstrainMinf(val.x, min.x, max.x);
        val.y = ConstrainMinf(val.y, min.y, max.y);
        val.z = ConstrainMinf(val.z, min.z, max.z);
        return val;
    }
    glm::vec4 ConstrainMin4f(glm::vec4 val, glm::vec4 min, glm::vec4 max) {
        val.x = ConstrainMinf(val.x, min.x, max.x);
        val.y = ConstrainMinf(val.y, min.y, max.y);
        val.z = ConstrainMinf(val.z, min.z, max.z);
        val.w = ConstrainMinf(val.w, min.w, max.w);
        return val;
    }
    glm::vec3 PointTowards(glm::vec3 orig, glm::vec3 dest) {
        /*
        float length = glm::length(orig);
        if(fabs(length) < 0.001) {
            return glm::vec3(0.0);
        }
        orig = glm::normalize(orig);
        orig.x*= dest.x;
        orig.y*= dest.y;
        orig.z*= dest.z;
        return orig * length;
        */
        glm::vec3 old = orig;
        if(fabs(dest.y) > fabs(dest.x)) {
            float temp = orig.x;
            orig.x = orig.y;
            orig.y = temp;
            if(!SameSign(orig.y, dest.y)) {
                orig.y = -orig.y;
            }
        }
        else if(!SameSign(orig.x, dest.x)) {
            orig.x = -orig.x;
        }
        return orig;
    }
    glm::vec2 Unproject(glm::vec4 pos, glm::mat4 modelView, glm::mat4 proj) {
    }
    glm::vec2 Project(glm::vec4 pos, glm::mat4 modelView, glm::mat4 proj) {
        pos = proj * modelView * pos;
        return pos;
    }
    glm::vec4 ProjectPoint(glm::vec4 pt, glm::mat4 proj, float norm) {
        glm::vec4 ret(pt.x * norm, pt.y * norm, pt.z, 1.0);
        ret = ret * proj;
        return ret;
    }
    glm::vec3 UnProjectPoint(glm::vec3 pt, glm::mat4 modelview, glm::mat4 proj, glm::vec4 viewport, glm::vec2 norm) {
        modelview = glm::inverse(modelview * proj);
        pt = glm::unProject(pt, modelview, proj, viewport);
        pt.x = (pt.x / norm.x) * norm.y;
        return pt;
    }
};

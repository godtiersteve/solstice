#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED
#include <fstream>
#include "solvector.h"

#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

namespace solstice {
class SolVector;
}
using std::ifstream;
using std::ofstream;

enum ColorCode {COL_EMPTY, COL_CUSTOM, COL_RED, COL_BLUE, COL_GREEN, COL_ORANGE, COL_PINK, COL_CYAN, COL_PURPLE, COL_YELLOW,
                COL_MAGENTA, COL_NAVY, COL_OLIVE, COL_BROWN, COL_VIOLET, COL_INDIGO,
                COL_WHITE, COL_BLACK, COL_GRAY, COL_SILVER, COL_GOLD, COL_GREY};

struct Color {
    float red, green, blue, alpha;
    ColorCode code;

    Color(ColorCode col);
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(glm::vec3 col) {*this = col;}
    Color(glm::vec4 col) {*this = col;}

    Color& operator=(ColorCode code);
    Color& operator=(glm::vec3 col);
    Color& operator=(glm::vec4 col);
    Color& operator+=(Color col);
    Color& operator-=(Color col);

    friend bool operator!=(Color& col, ColorCode& code);
    friend bool operator==(Color& col1, Color& col2);
    friend Color operator+(Color c1, Color c2);
    friend Color operator-(Color c1, Color c2);
    operator bool() {return code != COL_EMPTY;}
    void ToArray(float* arr);
    Color& operator=(solstice::SolVector v);
    operator glm::vec3() {return glm::vec3(red, green, blue);}
    operator glm::vec4() {return glm::vec4(red, green, blue, alpha);}
};
#endif // COLOR_H_INCLUDED

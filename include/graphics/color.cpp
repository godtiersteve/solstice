#include "color.h"

Color::Color(ColorCode col)
{
    *this = col;
    alpha = 1.0f;
}
Color::Color(float r, float g, float b):code(COL_CUSTOM) {
    float adjust = 1.0f / 255.0f;
    red = r > 1.0f ? adjust * r : r;
    green = g > 1.0f ? adjust *  g : g;
    blue = b > 1.0f ? adjust * b : b;
    alpha = 1.0f;
}
Color::Color(float r, float g, float b, float a):code(COL_CUSTOM) {
    float adjust = 1.0f / 255.0f;
    red = r > 1.0f ? adjust * r : r;
    green = g > 1.0f ? adjust *  g : g;
    blue = b > 1.0f ? adjust * b : b;
    alpha = a > 1.0f ? adjust * a : a;

}
Color::Color():red(1.0f), green(1.0f), blue(1.0f), alpha(1.0f)
{}
Color& Color::operator=(glm::vec3 col) {
    red = col.r;
    green = col.g;
    blue = col.b;
    alpha = 1.0f;
    return *this;
}
Color& Color::operator=(glm::vec4 col) {
    red = col.r;
    green = col.g;
    blue = col.b;
    alpha = col.a;
    return *this;
}
Color& Color::operator=(ColorCode code) {
    switch(code) {
        case COL_EMPTY:
            red = -1.0f;
            green = -1.0f;
            blue = -1.0f;
            break;
        case COL_RED:
            red = 1.0f;
            green = 0.0f;
            blue = 0.0f;
            break;
        case COL_BLUE:
            red = 0.0f;
            green = 0.0f;
            blue = 1.0f;
            break;
        case COL_GREEN:
            red = 0.0f;
            green = 1.0f;
            blue = 0.0f;
            break;
        case COL_ORANGE:
            red = 1.0f;
            green = 0.5f;
            blue = 0.0f;
            break;
        case COL_PINK:
            red = 1.0f;
            green = 0.75f;
            blue = 0.8f;
            break;
        case COL_CYAN:
            red = 0.0f;
            green = 1.0f;
            blue = 1.0f;
            break;
        case COL_PURPLE:
            red = 0.5f;
            green = 0.15f;
            blue = 0.8f;
            break;
        case COL_YELLOW:
            red = 1.0f;
            green = 1.0f;
            blue = 0.0f;
            break;
        case COL_MAGENTA:
            red = 1.0f;
            green = 0.0f;
            blue = 1.0f;
            break;
        case COL_NAVY:
            red = 0.0f;
            green = 0.0f;
            blue = 0.5f;
            break;
        case COL_OLIVE:
            red = 0.5f;
            green = 0.5f;
            blue = 0.0f;
            break;
        case COL_BROWN:
            red = 0.58f;
            green = 0.3f;
            blue = 0.0f;
            break;
        case COL_VIOLET:
            red = 0.5f;
            green = 0.0f;
            blue = 1.0f;
            break;
        case COL_INDIGO:
            red = 0.43;
            green = 0.0f;
            blue = 1.0f;
            break;
        case COL_WHITE:
            red = 1.0f;
            green = 1.0f;
            blue = 1.0f;
            break;
        case COL_BLACK:
            red = 0.0f;
            green = 0.0f;
            blue = 0.0f;
            break;
        case COL_GRAY:
            red = 0.5f;
            green = 0.5f;
            blue = 0.5f;
            break;
        case COL_SILVER:
            red = 0.75f;
            green = 0.75f;
            blue = 0.75f;
            break;
        case COL_GOLD:
            red = 1.0f;
            green = 0.85f;
            blue = 0.0f;
            break;
        case COL_GREY:
            red = 0.5f;
            green = 0.5f;
            blue = 0.5f;
            break;
        default:
            break;
    }
    this->code = code;
    return *this;
}
Color& Color::operator=(solstice::SolVector v) {
    if(v.Size() == 4) {
        red = v.GetElement(0);
        green = v.GetElement(1);
        blue = v.GetElement(3);
        alpha = v.GetElement(4);
    }
    else if(v.Size() == 3) {
        red = v.GetElement(0);
        green = v.GetElement(1);
        blue = v.GetElement(3);
        alpha = 1.0f;
    }
    return *this;
}
bool operator==(Color& col1, Color& col2) {
    if(col1.code == col2.code && col1.code != COL_CUSTOM) {
        return true;
    }
    int r1, g1, b1, r2, g2, b2;
    r1 = col1.red * 100; g1 = col1.green * 100; b1 = col1.blue * 100;
    r2 = col2.red * 100; g2 = col2.green * 100; b2 = col2.blue * 100;

    if(r1 != r2)
        return false;
    else if(g1 != g2)
        return false;
    else if(b1 != b2)
        return false;
    return true;
}
Color operator+(Color c1, Color c2) {
    Color c3;
    c3.red = c1.red + c2.red;
    c3.blue = c1.blue + c2.blue;
    c3.green = c1.green + c2.green;
    return c3;
}
Color operator-(Color c1, Color c2) {
    Color c3;
    c3.red = c1.red - c2.red;
    c3.blue = c1.blue - c2.blue;
    c3.green = c1.green - c2.green;
    return c3;
}


void Color::ToArray(float* arr) {
    arr[0] = red;
    arr[1] = green;
    arr[2] = blue;
    arr[3] = alpha;
    arr[4] = red;
    arr[5] = green;
    arr[6] = blue;
    arr[7] = alpha;
    arr[8] = red;
    arr[9] = green;
    arr[10] = blue;
    arr[11] = alpha;
    arr[12] = red;
    arr[13] = green;
    arr[14] = blue;
    arr[15] = alpha;
}

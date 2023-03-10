#ifndef TEXT_LINE_H_INCLUDED
#define TEXT_LINE_H_INCLUDED
// TODO (Stephen#1#): Implement textlines, which store positional data about particular lines in a sol_text object. Allows for easily finding the edges of a line as well as its length (in characters) and number in the current page/paragraph

#include "graphics/Vector2.h"
struct TextLine {
    int length, number;
    Vector2<float> left, right, top, bottom;
};

#endif // TEXT_LINE_H_INCLUDED

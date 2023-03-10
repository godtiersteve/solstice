#ifndef SOL_STR_H_INCLUDED
#define SOL_STR_H_INCLUDED
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <vector>

using std::string;

namespace solstice {
    void SetStrNum(char* str, unsigned int number, unsigned short digits);
    string GetStartTag(string str);
    string GetEndTag(string str);
    string SetAttributeTo(string attr, string value);
    string PrintfToString(const char* str, ...);
    std::vector<std::string> Split(const std::string &text, char delim);

    bool StringComp(string str1, string str2);
    string BuildString(string st, string delim, int startPos);
    int BuildInt(string st, string delim, int startPos);
}
#endif // LOGIC_TEXT_H_INCLUDED


#include "sol_str.h"

namespace solstice {
    void SetStrNum(char* str, unsigned int number, unsigned short digits) {
		const int numSize = digits + 1;
		char* num = new char[numSize];
        sprintf(num, "%i", number);
        int size = strlen(num);
        if(size < digits) {
            int diff = digits - size;
            strcpy(str, "0");
            for(int i = 1; i < diff; i++) {
                strcat(str, "0");
            }
        }
        strcat(str, num);
		delete[] num;
    }

    string GetStartTag(string str) {
        return "<" + str + ">";
    }
    string GetEndTag(string str) {
        return "<\\" + str + ">";
    }
    string SetAttributeTo(string attr, string value) {
        return attr + " = " + value;
    }

    string PrintfToString(const char* str, ...) {
        char text[sizeof(str) + 32];
        va_list argList;
        if(!str) {
            *text = NULL;
        }
        else {
            va_start(argList, str);
            vsprintf(text, str, argList);
            va_end(argList);
        }

        return text;
    }

    bool StringComp(string str1, string str2) {
        if(str1.size() != str2.size()) {
            return false;
        }
        for(int i = 0; i < str1.size(); i++) {
            if(str1[i] != str2[i])
                return false;
        }
        return true;
    }
    std::vector<std::string> Split(const std::string &text, char delim) {
        std::vector<std::string> tokens;
        short start = 0, end = 0;
        while((end = text.find(delim, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    }
    string BuildString(string st, string delim, int startPos) {
        int i = startPos;
        string str;
        bool stop = false;
        while(i < st.size() && stop == false) {
            str+= st[i];
            i++;
            for(int j = 0; j < delim.size(); j++) {
                if(st[i] == delim[j])
                    stop = true;
            }
        }
        return str;
    }
    int BuildInt(string st, string delim, int startPos) {
        std::string foo = BuildString(st, delim, startPos);
        std::string n;
        for(int i = foo.size() - 1; i >= 0; i--) {
            n+= foo[i];
        }

        int num = 0;
        for(int i = 0; i < n.size(); i++) {
            int x = (int)n[i] - 48;
            x*= pow(10, i);
            num+= x;
        }
    return num;
    }
};

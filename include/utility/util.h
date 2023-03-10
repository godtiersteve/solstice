#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
namespace solstice {
    template<class T>
    void BinaryWrite(std::ofstream& file, const T value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }
    template<class T>
    void BinaryRead(std::ifstream& file, T& value) {
        file.read(reinterpret_cast<char*>(&value), sizeof(T));
    }
    template<class T>
    void Concat(const std::vector<T>& src, std::vector<T>& dest) {
        dest.insert(dest.end(), src.begin(), src.end());
    }

    template<class T>
    void InsertUnique(std::vector<T>& v, T t) {
        if(std::find(v.begin(), v.end(), t) == v.end()) {
            v.push_back(t);
        }
    }
    void PrintMatrix(glm::mat4 mat);
    void BinaryWriteMatrix(std::ofstream& file, glm::mat4 mat);
    void BinaryWriteString(std::ofstream& file, std::string str);
    void BinaryWriteVec3(std::ofstream& file, glm::vec3 vec);
    void BinaryWriteVec4(std::ofstream& file, glm::vec4 vec);
    void BinaryReadString(std::ifstream& file, std::string& str);
    void BinaryReadMatrix(std::ifstream& file, glm::mat4& mat);
    void BinaryReadVec3(std::ifstream& file, glm::vec3& vec);
    void BinaryReadVec4(std::ifstream& file, glm::vec4& vec);
}

#endif // UTIL_H_INCLUDED

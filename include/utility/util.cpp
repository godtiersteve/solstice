#include "util.h"
namespace solstice {
void PrintMatrix(glm::mat4 mat) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << mat[j][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void BinaryWriteMatrix(std::ofstream& file, glm::mat4 mat) {
    for(size_t i = 0; i < 4; i++) {
        for(size_t j = 0; j < 4; j++) {
            file.write(reinterpret_cast<const char*>(&mat[i][j]), sizeof(float));
        }
    }
}
void BinaryWriteString(std::ofstream& file, std::string str) {
    BinaryWrite(file, str.size());
    for(auto ch : str) {
        BinaryWrite(file, ch);
    }
}
void BinaryWriteVec3(std::ofstream& file, glm::vec3 vec) {
    BinaryWrite(file, vec.x);
    BinaryWrite(file, vec.y);
    BinaryWrite(file, vec.z);
}
void BinaryWriteVec4(std::ofstream& file, glm::vec4 vec) {
    BinaryWrite(file, vec.x);
    BinaryWrite(file, vec.y);
    BinaryWrite(file, vec.z);
    BinaryWrite(file, vec.w);
}
void BinaryReadString(std::ifstream& file, std::string& str) {
    size_t size;
    BinaryRead(file, size);
    for(size_t i = 0; i < size; i++) {
        char ch;
        BinaryRead(file, ch);
        str+= ch;
    }
}
void BinaryReadMatrix(std::ifstream& file, glm::mat4& mat) {
    for(size_t i = 0; i < 4; i++) {
        for(size_t j = 0; j < 4; j++) {
            float f;
            BinaryRead(file, f);
            mat[i][j] = f;
        }
    }
}
void BinaryReadVec3(std::ifstream& file, glm::vec3& vec) {
    BinaryRead(file, vec.x);
    BinaryRead(file, vec.y);
    BinaryRead(file, vec.z);
}
void BinaryReadVec4(std::ifstream& file, glm::vec4& vec) {
    BinaryRead(file, vec.x);
    BinaryRead(file, vec.y);
    BinaryRead(file, vec.z);
    BinaryRead(file, vec.w);
}
}

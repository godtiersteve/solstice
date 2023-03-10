#ifndef VERTEX3_H_INCLUDED
#define VERTEX3_H_INCLUDED

#include "gl/glew.h"
#include "gl/gl.h"
#include "color.h"
#include "glm/gtc/matrix_transform.hpp"

namespace solstice {
struct Vertex3f {
    glm::vec3 position;
    Color color;
    glm::vec3 normal;
    glm::vec2 texCoord;
    Vertex3f() {
        position = glm::vec3(0);
        normal = glm::vec3(0.0);
        texCoord = glm::vec2(0);
    }
    Vertex3f(float x, float y) {
        Vertex3f(x, y, 0);
    }
    Vertex3f(float x, float y, float z) {
        position = glm::vec3(x, y, z);
        normal = glm::vec3(0.0);
        texCoord = glm::vec2(0);
    }
    Vertex3f(glm::vec3 vec) {
        position = vec;
    }
    virtual void AddWeight(size_t i, float w) {}
    void Print() {
        std::cout << "position x, y, z: " << position.x << ", " << position.y << ", " << position.z << std::endl;
        std::cout << "normal x, y, z: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
        std::cout << "texcoord: x, y: " << texCoord.x << ", " << texCoord.y << std::endl;
        std::cout << "color rgba: " << color.red << ", " << color.green << ", " << color.blue << ", " << color.alpha << std::endl;
    }
    bool operator==(Vertex3f vt) {
        float epsilon = 0.000797f;
        return
            glm::length(vt.position - position) < epsilon &&
            glm::length(vt.normal - normal) < epsilon &&
            glm::length(vt.texCoord - texCoord) < epsilon;
        //return vt.position == position && vt.normal == normal && vt.texCoord == texCoord;
    }
    virtual void Write(std::ofstream& file) {
        BinaryWrite(file, (float)position.x);
        BinaryWrite(file, (float)position.y);
        BinaryWrite(file, (float)position.z);
        BinaryWrite(file, (float)normal.x);
        BinaryWrite(file, (float)normal.y);
        BinaryWrite(file, (float)normal.z);
        BinaryWrite(file, color.red);
        BinaryWrite(file, color.green);
        BinaryWrite(file, color.blue);
        BinaryWrite(file, 0.25f);
        BinaryWrite(file, (float)texCoord.x);
        BinaryWrite(file, (float)texCoord.y);
    }
    virtual void Read(std::ifstream& file) {
        float x, y, z;
        BinaryRead(file, x);
        BinaryRead(file, y);
        BinaryRead(file, z);
        position = glm::vec3(x, y, z);
        BinaryRead(file, x);
        BinaryRead(file, y);
        BinaryRead(file, z);
        normal = glm::vec3(x, y, z);
        BinaryRead(file, color.red);
        BinaryRead(file, color.green);
        BinaryRead(file, color.blue);
        BinaryRead(file, color.alpha);
        BinaryRead(file, x);
        BinaryRead(file, y);
        texCoord = glm::vec2(x, y);
    }
    };
}

#endif // VERTEX3_H_INCLUDED

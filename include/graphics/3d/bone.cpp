#include "bone.h"
namespace solstice {
Bone::Bone(string n) {
    name = n;
    scale = glm::vec3(1.0f);
    translation = glm::vec3(0.0f);
    parentID = -1;
}
Bone::Bone(string n, size_t i) {
    name = n;
    parentID = -1;
    scale = glm::vec3(1.0f);
    translation = glm::vec3(0.0f);
}

void Bone::SetTransform(glm::vec3 trans, glm::vec3 scale, glm::quat rot) {
    this->scale = scale;
    translation = trans;
    rotation = rot;
}
void Bone::AddChild(size_t i) {
    for(auto& child : children) {
        if(child == i) {
            return;
        }
    }
    children.push_back(i);
}
glm::mat4 Bone::GetTransform() {
    return glm::translate(translation) * glm::toMat4(rotation) * glm::scale(scale);
}
void Bone::Write(std::ofstream& file) {
    BinaryWriteString(file, name);
    BinaryWriteString(file, parentName);
    BinaryWrite(file, parentID);
    BinaryWrite(file, children.size());
    for(auto& c : children) {
        BinaryWrite(file, c);
    }
    BinaryWrite(file, rotation.x);
    BinaryWrite(file, rotation.y);
    BinaryWrite(file, rotation.z);
    BinaryWrite(file, rotation.w);
    BinaryWriteMatrix(file, offsetMatrix);
    BinaryWriteMatrix(file, transform);
    BinaryWriteVec3(file, translation);
    BinaryWriteVec3(file, scale);
}
void Bone::Read(std::ifstream& file) {
    BinaryReadString(file, name);
    BinaryReadString(file, parentName);
    BinaryRead(file, parentID);
    size_t numChildren;
    BinaryRead(file, numChildren);
    children.reserve(numChildren);
    for(size_t i = 0; i < numChildren; i++) {
        size_t c;
        BinaryRead(file, c);
        children.push_back(c);
    }
    BinaryRead(file, rotation.x);
    BinaryRead(file, rotation.y);
    BinaryRead(file, rotation.z);
    BinaryRead(file, rotation.w);
    BinaryReadMatrix(file, offsetMatrix);
    BinaryReadMatrix(file, transform);
    BinaryReadVec3(file, translation);
    BinaryReadVec3(file, scale);
}
std::string Bone::GetGroupName() {
    std::string ret;
    size_t startSize = name.find('_');
    if(startSize == std::string::npos) {
        return "";
    }
    std::string sub = name.substr(startSize + 1, name.size() - startSize + 1);
    size_t endSize = sub.find('_');
    if(endSize == std::string::npos) {
        return "";
    }
    ret = sub.substr(0, endSize);
    size_t size = name.size() - startSize + 2 + ret.size();
    std::string n = name.substr(startSize + 2 + ret.size(), size);
    printf("Name: %s\n", n.c_str());
    return ret;
}
}

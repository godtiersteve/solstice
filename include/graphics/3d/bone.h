#ifndef BONE_H_INCLUDED
#define BONE_H_INCLUDED

#include "graphics/vector2.h"
#include "graphics/vertex3.h"
#include "graphics/shaderprogram.h"
#include "graphics/texarray.h"
#include "graphics/color.h"
#include "graphics/vertexbuffer.h"
#include "geometry/vertex3.h"
#include "geometry/polygon.h"

#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <deque>

namespace solstice {
struct Bone {
    glm::mat4 finalTransformation;
    glm::mat4 transform;
    glm::vec3 translation, scale;
    glm::quat rotation;
    glm::mat4 offsetMatrix;
    std::vector<size_t> children;
    int parentID;
    string name;
    string parentName;

    Bone(string n = "");
    Bone(string n, size_t i);
    void SetTransform(glm::vec3 trans, glm::vec3 scale, glm::quat rot);
    void AddChild(size_t i);
    void ClearChildren() {children.clear();}
    glm::mat4 GetTransform();
    void Write(std::ofstream& file);
    void Read(std::ifstream& file);
    std::string GetGroupName();
};
struct BoneGroup {
    std::string name;
    std::vector<int> boneIDs;
    int animID, animIndex;
    BoneGroup(std::string n = "") {
        name = n;
    }
};
}

#endif // BONE_H_INCLUDED

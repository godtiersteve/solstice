#ifndef SKELVERTEX_H_INCLUDED
#define SKELVERTEX_H_INCLUDED
#include "graphics/vertex3.h"
namespace solstice {
    struct SkelVertex3f : public Vertex3f {
        glm::ivec3 jointIDs;
        glm::vec3 jointWeights;
        SkelVertex3f() {
            jointIDs.x = 0;
            jointIDs.y = 0;
            jointIDs.z = 0;
            jointWeights.x = 0.0f;
            jointWeights.y = 0.0f;
            jointWeights.z = 0.0f;
        }
        SkelVertex3f(Vertex3f vt) {
            position = vt.position;
            color = vt.color;
            normal = vt.normal;
            texCoord = vt.texCoord;
        }
        void AddWeight(size_t index, float weight) {
            for(size_t i = 0; i < 3; i++) {
                if(jointWeights[i] == 0.0f) {
                    jointIDs[i] = index;
                    jointWeights[i] = weight;
                    return;
                }
            }
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
            BinaryWrite(file, color.red);
            BinaryWrite(file, (float)texCoord.x);
            BinaryWrite(file, (float)texCoord.y);
            BinaryWrite(file, (float)jointWeights.x);
            BinaryWrite(file, (float)jointWeights.y);
            BinaryWrite(file, (float)jointWeights.z);
            int x = jointIDs.x, y = jointIDs.y, z = jointIDs.z;
            BinaryWrite(file, x);
            BinaryWrite(file, y);
            BinaryWrite(file, z);
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
            BinaryRead(file, x);
            BinaryRead(file, y);
            BinaryRead(file, z);
            jointWeights = glm::vec3(x, y, z);
            int xx, yy, zz;
            BinaryRead(file, xx);
            BinaryRead(file, yy);
            BinaryRead(file, zz);
            jointIDs = glm::ivec3(xx, yy, zz);
        }
    };
}


#endif // SKELVERTEX_H_INCLUDED

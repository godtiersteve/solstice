#ifndef RENDERINSTRUCTION_H_INCLUDED
#define RENDERINSTRUCTION_H_INCLUDED

#include "3d/modelanimset.h"

namespace solstice {
    class RenderInstruction {
        public:
            int mModelID;
            glm::mat4 mTransform;
            ModelAnimationset* mAnimationset;
            RenderInstruction() {
                mAnimationset = nullptr;
                mModelID = -1;
            }
            RenderInstruction(float x, float y, float z, float scale, int modelID) {
                mModelID = modelID;
                mAnimationset = nullptr;
                mTransform = glm::translate(glm::vec3(x, y, z)) * glm::scale(glm::vec3(scale, scale, scale));
            }
    };

}

#endif // RENDERINSTRUCTION_H_INCLUDED

#include "mesh.h"

namespace solstice {
    vector<GLuint> TriangulateIndices(vector<Vertex3f>& vt) {
        std::vector<GLuint> indices;
        GLuint index = 0;
        for(size_t i = 0; i < vt.size() / 4; i++) {
            indices.push_back(index++);
            indices.push_back(index++);
            indices.push_back(index);
            indices.push_back(index - 2);
            indices.push_back(index++);
            indices.push_back(index);
        }
        return indices;
    }
}


#ifndef RENDERER3D_H_INCLUDED
#define RENDERER3D_H_INCLUDED
#include "graphics/graphicglobals.h"
#include "mesh.h"
#include <vector>

using std::vector;

namespace solstice {
class Renderer3D {
    vector<Model> mModels;
    TextureArray& mTextures;
};
}

#endif // RENDERER3D_H_INCLUDED

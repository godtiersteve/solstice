#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "mesh.h"
#include "modelbase.h"
#include "ext/tiny_obj_loader.h"
#include <algorithm>

namespace solstice {
class Model : public ModelBase<Mesh> {
public:
    Model(string path = "", bool setup = true);
    Model(string path, TextureArray& tex, bool setup = true);
    Model(vector<Mesh> mesh, bool setup = false);

    void AddMeshes(string path, bool setup = true);
    void AddMeshes(string path, TextureArray& texArr, bool setup = true);
    void LoadModel(string path, bool setup = true);
};

}

#endif // MODEL_H_INCLUDED

#ifndef SKELETALMESH_H_INCLUDED
#define SKELETALMESH_H_INCLUDED

#include "meshbase.h"
#include "bone.h"
#include "modelanim.h"
#include "skelvertex.h"

namespace solstice {
class SkeletalMesh : public MeshBase<SkelVertex3f> {
public:
    void ClearVertices();
    void ClearIndices();
    void ClearBuffers();
    void BindBuffers();
    void SetupMesh();
    void DrawVao();
    void DrawOutline(ShaderProgram& sh);
    void DrawOutline(ShaderProgram& sh, TextureArray& textures);
    bool HasBones()                                     {return true;}
};
}

#endif // SKELETALMESH_H_INCLUDED

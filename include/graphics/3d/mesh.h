#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "meshbase.h"

using std::vector;
using std::string;


namespace solstice {
/*
class Mesh : public MeshBase<Vertex3f> {
public:
    Mesh(string path = "");
    void LoadMesh(string path);
};
*/
typedef MeshBase<Vertex3f> Mesh;
vector<GLuint> TriangulateIndices(vector<Vertex3f>& vt);
}
#endif // MESH_H_INCLUDED

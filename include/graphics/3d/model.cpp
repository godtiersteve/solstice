#include "model.h"

namespace solstice {
Model::Model(string path, bool setup) {
    if(!path.empty()) {
        LoadModel(path, setup);
    }
    mHidden = false;
}
Model::Model(vector<Mesh> mesh, bool setup) {
    mMeshes = mesh;
    if(setup) {
        SetupModel();
    }
    mHidden = false;
}
Model::Model(string path, TextureArray& tex, bool setup) {
    AddMeshes(path, tex, setup);
    mHidden = false;
}

void Model::AddMeshes(string path, bool setup) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    string dir;
    if(path.find('\\') != std::string::npos) {
        dir = path.substr(0, path.find_last_of('\\') + 1);
    }
    else {
        dir = path.substr(0, path.find_last_of('/') + 1);
    }
    tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), dir.c_str());
    if(!err.empty()) {
        std::cout << err << std::endl;
        return;
    }
    for(size_t s = 0; s < shapes.size(); s++) {
        mMeshes.push_back(Mesh());
        GLuint index = 0;
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes[s].mesh.num_face_vertices[f];
            for(size_t v = 0; v < fv; v++) {
                Vertex3f vt;
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vt.position.x = attrib.vertices[3 * idx.vertex_index];
                vt.position.y = attrib.vertices[3 * idx.vertex_index + 1];
                vt.position.z  = attrib.vertices[3 * idx.vertex_index + 2];

                vt.normal.x = attrib.normals[3 * idx.normal_index];
                vt.normal.y = attrib.normals[3 * idx.normal_index + 1];
                vt.normal.z = attrib.normals[3 * idx.normal_index + 2];
                if(!attrib.texcoords.empty()) {
                    float texOffset = 0;
                    if(!materials.empty()) {
                        texOffset = shapes[s].mesh.material_ids[f] * 1000 + 1000;
                    }
                    vt.texCoord.x  = attrib.texcoords[2 * idx.texcoord_index] + texOffset;
                    vt.texCoord.y = attrib.texcoords[2 * idx.texcoord_index + 1] + texOffset;
                }
                else {
                    vt.texCoord.x  = -1000;
                    vt.texCoord.y =  -1000;
                }
                std::vector<Vertex3f>::iterator it;
                it = std::find(mMeshes[s].GetVertices().begin(), mMeshes[s].GetVertices().end(), vt);
                if(it == mMeshes[s].GetVertices().end()) {
                    mMeshes[s].AddIndex(index++);
                    mMeshes[s].AddVertex(vt);
                    //mMeshes[s].indices.push_back(index++);
                    //mMeshes[s].vertices.push_back(vt);
                }
                else {
                    //mMeshes[s].AddIndex(it->index);
                    //mMeshes[s].indices.push_back(it->index);
                }
            }
            index_offset += fv;
        }
    }
    if(setup) {
        SetupModel();
    }
}
void Model::AddMeshes(string path, TextureArray& texArr, bool setup) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    string dir;
    if(path.find('\\') != std::string::npos) {
        dir = path.substr(0, path.find_last_of('\\') + 1);
    }
    else {
        dir = path.substr(0, path.find_last_of('/') + 1);
    }
    tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), dir.c_str());
    if(!err.empty()) {
        std::cout << err << std::endl;
        return;
    }
    for(size_t s = 0; s < shapes.size(); s++) {
        mMeshes.push_back(Mesh());
        GLuint index = 0;
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes[s].mesh.num_face_vertices[f];
            for(size_t v = 0; v < fv; v++) {
                Vertex3f vt;
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vt.position.x = attrib.vertices[3 * idx.vertex_index];
                vt.position.y = attrib.vertices[3 * idx.vertex_index + 1];
                vt.position.z  = attrib.vertices[3 * idx.vertex_index + 2];

                vt.normal.x = attrib.normals[3 * idx.normal_index];
                vt.normal.y = attrib.normals[3 * idx.normal_index + 1];
                vt.normal.z = attrib.normals[3 * idx.normal_index + 2];
                if(!attrib.texcoords.empty()) {
                    float texOffset = 0;
                    unsigned int matIndex = shapes[s].mesh.material_ids[f];
                    if(!materials.empty() && matIndex < materials.size()) {
                        texOffset = texArr.GetTextureID(materials[matIndex].name) * 1000 + 1000;
                    }
                    vt.texCoord.x  = attrib.texcoords[2 * idx.texcoord_index] + texOffset;
                    vt.texCoord.y = attrib.texcoords[2 * idx.texcoord_index + 1] + texOffset;
                }
                else {
                    vt.texCoord.x  = -1000;
                    vt.texCoord.y =  -1000;
                }
                std::vector<Vertex3f>::iterator it;
                it = std::find(mMeshes[s].GetVertices().begin(), mMeshes[s].GetVertices().end(), vt);
                if(it == mMeshes[s].GetVertices().end()) {
                    //vt.index = index;
                    mMeshes[s].AddIndex(index++);
                    mMeshes[s].AddVertex(vt);
                }
                else {
                    //mMeshes[s].AddIndex(it->index);
                }
            }
            index_offset += fv;
        }
        mMeshes[s].SetName(shapes[s].name.substr(0, shapes[s].name.find('_')));
    }
    if(setup) {
        SetupModel();
    }
}

void Model::LoadModel(string path, bool setup) {
    clear();
    AddMeshes(path, setup);
}
}

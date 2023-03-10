#include "plane.h"

namespace solstice {
Mesh CreatePlaneXY(float height, float width, glm::vec3 position, Color c) {
    return CreatePlaneXY(height, width, position, 1.0, c);
}
vector<Vertex3f> GetPlaneVertsXY(float height, float width, glm::vec3 pos, Vector2f texTile, Color c) {
    float texXmax = width >= height ? 1.0f : height / width;
    float texYmax = height >= width ? 1.0f : width / height;
    texTile.x*= texXmax;
    texTile.y*= texYmax;
    vector<Vertex3f> vertices;
    Vertex3f vt;
    vt.position = pos;
    vt.normal.x = 0;
    vt.normal.y = 0;
    vt.normal.z = 1;
    vt.texCoord.x = 0;
    vt.texCoord.y = 0;
    vt.color = c;
    vertices.push_back(vt);

    vt.texCoord.x = texTile.x;
    vt.texCoord.y = 0;
    vt.position.x+= width;
    vertices.push_back(vt);

    vt.texCoord.x = texTile.x;
    vt.texCoord.y = texTile.y;
    vt.position.y+= height;
    vertices.push_back(vt);

    vt.texCoord.x = 0;
    vt.texCoord.y = texTile.y;
    vt.position.x-= width;
    vertices.push_back(vt);
    return vertices;
}
vector<Vertex3f> GetPlaneVertsZX(float depth, float width, glm::vec3 pos, Vector2f texTile, Color c) {
    float texXmax = width >= depth ? 1.0f : depth / width;
    float texYmax = depth >= width ? 1.0f : width / depth;
    texTile.x*= texXmax;
    texTile.y*= texYmax;

    vector<Vertex3f> vertices;
    Vertex3f vt;
    vt.position = pos;
    vt.normal.x = 0;
    vt.normal.y = 1;
    vt.normal.z = 0;
    vt.texCoord.x = 0;
    vt.texCoord.y = 0;
    vertices.push_back(vt);

    vt.texCoord.x = texTile.y;
    vt.texCoord.y = 0;
    vt.position.z-= depth;
    vertices.push_back(vt);

    vt.texCoord.x = texTile.x;
    vt.texCoord.y = texTile.y;
    vt.position.x+= width;
    vertices.push_back(vt);

    vt.texCoord.x = 0;
    vt.texCoord.y = texTile.y;
    vt.position.z+= depth;
    vertices.push_back(vt);
    return vertices;
}
vector<Vertex3f> GetPlaneVertsYZ(float height, float depth, glm::vec3 pos, Vector2f texTile, Color c) {
    float texXmax = depth <= height ? 1.0f : depth / height;
    float texYmax = height <= depth ? 1.0f : height / depth;
    texTile.x*= texXmax;
    texTile.y*= texYmax;

    vector<Vertex3f> vertices;
    Vertex3f vt;
    vt.position = pos;
    vt.normal.x = 1;
    vt.normal.y = 0;
    vt.normal.z = 0;
    vt.texCoord.x = 0;
    vt.texCoord.y = 0;
    vertices.push_back(vt);

    vt.texCoord.x = texTile.x;
    vt.texCoord.y = 0;
    vt.position.y+= height;
    vertices.push_back(vt);

    vt.texCoord.x = texTile.x;
    vt.texCoord.y = texTile.y;
    vt.position.z-= depth;
    vertices.push_back(vt);

    vt.texCoord.x = 0;
    vt.texCoord.y = texTile.x;
    vt.position.y-= height;
    vertices.push_back(vt);
    return vertices;
}
Mesh CreatePlaneXY(float height, float width, glm::vec3 position, Vector2f texTile, Color c) {
    vector<Vertex3f> vertices = GetPlaneVertsXY(height, width, position, texTile, c);
    return Mesh(vertices, TriangulateIndices(vertices));
}
Mesh CreatePlaneYZ(float height, float depth, glm::vec3 position) {
    vector<Vertex3f> vertices = GetPlaneVertsYZ(height, depth, position, 1.0f, COL_WHITE);
    return Mesh(vertices, TriangulateIndices(vertices));
}
Mesh CreatePlaneZX(float depth, float width, glm::vec3 position) {
    vector<Vertex3f> vertices = GetPlaneVertsZX(depth, width, position, 1.0, COL_WHITE);
    return Mesh(vertices, TriangulateIndices(vertices));
}
Mesh CreatePlaneSubDivXYM(float height, float width, float subx, float suby, glm::vec3 position) {
    return CreatePlaneSubDivXYM(height, width, subx, suby, position, 1.0f, COL_WHITE, COL_WHITE);
}
Mesh CreatePlaneSubDivXYM(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color c) {
    return CreatePlaneSubDivXYM(height, width, subx, suby, position, texTile, c, c);
}
Mesh CreatePlaneSubDivXYM(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol) {
    std::vector<Vertex3f> vertices = CreatePlaneSubDivXYV(height, width, subx, suby, position, texTile, startCol, endCol);
    return Mesh(vertices, TriangulateIndices(vertices));
}
vector<Vertex3f> CreatePlaneSubDivXYV(float height, float width, float subx, float suby, glm::vec3 position) {
    return CreatePlaneSubDivXYV(height, width, subx, suby, position, 1.0f, COL_WHITE, COL_WHITE);
}
vector<Vertex3f> CreatePlaneSubDivXYV(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color c) {
    return CreatePlaneSubDivXYV(height, width, subx, suby, position, texTile, c, c);
}
vector<Vertex3f> CreatePlaneSubDivXYV(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol) {
    float subh = -height / suby;
    float subw = width / subx;
    float texXmax = width >= height ? 1.0f : height / width;
    float texYmax = height >= width ? 1.0f : width / height;
    texTile.x*= texXmax;
    texTile.y*= texYmax;
    float alphaStep = endCol.alpha - startCol.alpha;
    vector<Vertex3f> vertices;
    for(int i = 0; i < subx; i++) {
        for(int j = 0; j < suby; j++) {
            Vertex3f vt;
            glm::vec3 offset(subw * i, subh * j, 0);
            vt.position = position + offset;
            vt.normal.x = 0;
            vt.normal.y = 0;
            vt.normal.z = 1;
            vt.texCoord.x = 0;
            vt.texCoord.y = 0;
            vt.color = startCol;
            vertices.push_back(vt);

            vt.texCoord.x = texTile.x;
            vt.texCoord.y = 0;
            vt.position.x+= subw;
            vertices.push_back(vt);

            vt.color.alpha+= alphaStep * (j + 1);
            vt.texCoord.x = texTile.x;
            vt.texCoord.y = texTile.y;
            vt.position.y+= subh;
            vertices.push_back(vt);

            vt.texCoord.x = 0;
            vt.texCoord.y = texTile.y;
            vt.position.x-= subw;
            vertices.push_back(vt);
        }
    }
    return vertices;
}
vector<Vertex3f> CreatePlaneSubDivZXVt(float depth, float width, float subz, float subx, glm::vec3 position) {
    return CreatePlaneSubDivZXVt(depth, width, subz, subx, position, 1.0f, COL_WHITE, COL_WHITE);
}
vector<Vertex3f> CreatePlaneSubDivZXVt(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color c) {
    return CreatePlaneSubDivZXVt(depth, width, subz, subx, position, texTile, c, c);
}
vector<Vertex3f> CreatePlaneSubDivZXVt(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol) {
    float texXmax = width >= depth ? 1.0f : depth / width;
    float texYmax = depth >= width ? 1.0f : width / depth;
    texTile.x*= texXmax;
    texTile.y*= texYmax;
    float subw = width / subx;
    float subd = depth / subz;
    vector<Vertex3f> vertices;
    for(int i = 0; i < subx; i++) {
        for(int j = 0; j < subz; j++) {
            Vertex3f vt;
            glm::vec3 offset(i * subw, 0, j * -subd);
            vt.position = position + offset;
            vt.normal.x = 0.0f;
            vt.normal.y = 1.0f;
            vt.normal.z = 0.0f;
            vt.texCoord.x = 0;
            vt.texCoord.y = 0;
            vt.color = startCol;
            vertices.push_back(vt);

            vt.texCoord.x = texTile.x;
            vt.texCoord.y = 0;
            vt.position.z-= subd;
            vertices.push_back(vt);

            vt.texCoord.x = texTile.x;
            vt.texCoord.y = texTile.y;
            vt.position.x+= subw;
            vertices.push_back(vt);

            vt.texCoord.x = 0;
            vt.texCoord.y = texTile.y;
            vt.position.z+= subd;
            vertices.push_back(vt);
        }
    }
    return vertices;
}
Mesh CreatePlaneSubDivZXMe(float depth, float width, float subz, float subx, glm::vec3 position) {
    std::vector<Vertex3f> vertices = CreatePlaneSubDivZXVt(depth, width, subz, subx, position, 1.0f, COL_WHITE, COL_WHITE);
    return Mesh(vertices, TriangulateIndices(vertices));
}
Mesh CreatePlaneSubDivZXMe(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color c) {
    vector<Vertex3f> vertices = CreatePlaneSubDivZXVt(depth, width, subz, subx, position, texTile, c, c);
    return Mesh(vertices, TriangulateIndices(vertices));
}
Mesh CreatePlaneSubDivZXMe(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol) {
    vector<Vertex3f> vertices = CreatePlaneSubDivZXVt(depth, width, subz, subx, position, texTile, startCol, endCol);
    return Mesh(vertices, TriangulateIndices(vertices));
}
}

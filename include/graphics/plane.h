#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "graphicglobals.h"
#include "quadmath.h"
#include "vertex3.h"
#include "3d/mesh.h"
#include <vector>

using std::vector;

namespace solstice {
vector<Vertex3f> GetPlaneVertsXY(float height, float width, glm::vec3 pos, Vector2f texTile, Color c);
vector<Vertex3f> GetPlaneVertsZX(float depth, float width, glm::vec3 pos, Vector2f texTile, Color c);
vector<Vertex3f> GetPlaneVertsYZ(float height, float depth, glm::vec3 pos, Vector2f texTile, Color c);
Mesh CreatePlaneXY(float height, float width, glm::vec3 position = glm::vec3(0), Color c = COL_WHITE);
Mesh CreatePlaneXY(float height, float width, glm::vec3 position, Vector2f texTile, Color c);
Mesh CreatePlaneYZ(float height, float width, glm::vec3 position = glm::vec3(0));
Mesh CreatePlaneZX(float height, float width, glm::vec3 position = glm::vec3(0));
Mesh CreatePlaneSubDivXYM(float height, float width, float subx, float suby, glm::vec3 position);
Mesh CreatePlaneSubDivXYM(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color c);
Mesh CreatePlaneSubDivXYM(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol);

vector<Vertex3f> CreatePlaneSubDivXYV(float height, float width, float subx, float suby, glm::vec3 position);
vector<Vertex3f> CreatePlaneSubDivXYV(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color c);
vector<Vertex3f> CreatePlaneSubDivXYV(float height, float width, float subx, float suby, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol);
Mesh CreatePlaneSubDivYZM(float height, float width, float subh, float subw, glm::vec3 position, Vector2f texTile, Color c);
Mesh CreatePlaneSubDivZXM(float depth, float width, float subz, float subx, glm::vec3 position);

vector<Vertex3f> CreatePlaneSubDivZXVt(float depth, float width, float subz, float subx, glm::vec3 position);
vector<Vertex3f> CreatePlaneSubDivZXVt(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color c);
vector<Vertex3f> CreatePlaneSubDivZXVt(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol);
Mesh CreatePlaneSubDivZXMe(float depth, float width, float subz, float subx, glm::vec3 position);
Mesh CreatePlaneSubDivZXMe(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color c);
Mesh CreatePlaneSubDivZXMe(float depth, float width, float subz, float subx, glm::vec3 position, Vector2f texTile, Color startCol, Color endCol);

}

#endif // PLANE_H_INCLUDED

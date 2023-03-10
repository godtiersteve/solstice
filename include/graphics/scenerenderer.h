#ifndef SCENERENDERER_H_INCLUDED
#define SCENERENDERER_H_INCLUDED

#include "graphicglobals.h"
#include "geometry/rectangle.h"
#include "geometry/polygon.h"
#include "geometry/hitbox.h"
#include "graphics/shaderprogram.h"
#include "graphics/quaddata.h"
#include "graphics/multiquad.h"
#include "graphics/vertex2.h"
#include "utility/util.h"

namespace solstice {
struct QuadInstance {
    glm::vec4 quad;
    glm::vec4 textCoords;
    glm::vec4 color;
};
class SceneRenderer {
private:
    GLuint mVBO, mIBO, mVAO, mVAO2;
    std::vector<GLuint> mIndices;
    std::vector<Vertex2f> mVerts;
    size_t mUsedVertices, mUsedIndices;
    int mLastIndex;

    GLuint mVertexBuffer, mTextureBuffer;
    static GLuint sIndexBuffer;
    GLuint mColorBuffer, mTexCoordBuffer, mPositionBuffer, mTexIDBuff, mAngleBuffer;
    std::vector<glm::vec4> mPositions;
    std::vector<glm::vec4> mColors;
    std::vector<glm::vec4> mTextureOffsets;
    std::vector<int> mTextureIDs;
    std::vector<float> mAngles;

    size_t mMaxQuads, mUsedQuads;
    bool mPerspective;
    Vector2f mResolution, mNormalize;
    ModelTransform mTransform;
    glm::mat4 mProjection;

    void Setup(int vtBuffSize);
    void ReinitBuffers(unsigned int vtBuffSize);
    void AddQuadIndices();
    void AddVertex(Vertex2f vt);
    void ResizeBuffers(size_t size);
public:
    SceneRenderer(Vector2f resolution, int vtBuffSize);
    SceneRenderer(float resx, float resy, int vtBuffSize);
    void DrawRect(Rectanglef rect, Color c);
    void DrawRects(std::vector<Rectanglef> rects, Color c);
    void DrawRect(Rectanglef rect, int textureID, Color c);
    void DrawMultiQuad(Picture picture, Color c = COL_WHITE);
    void DrawQuad(QuadData q, Color c = COL_EMPTY);
    void DrawRectPoly(const Rectanglef& rect, Color c);
    void DrawQuadPoly(const Rectanglef& rect, int texID, Color c);
    void DrawQuadPoly(const QuadData& rect, Color c = COL_EMPTY);
    void DrawRectOutline(Rectanglef rect, float size, Color c);
    void DrawPoly(Polygonf p, Color c = COL_WHITE);
    void DrawPoly(Polygonf p, int texID, Vector2f offset, Color c);
    void DrawPoly(Polygonf p, int texID, Vector2f offset, float height, float width, Color c);
    void DrawPolyVerts(Polygonf p, float size, Color c);
    void DrawLine(Vector2f start, Vector2f end, float size, Color c);
    void DrawEdge(Edgef e, float size, Color c);
    void DrawHitBox(HitBoxf& hbox, Color c = COL_WHITE);
    void DrawShape(const IShapef* shape, Color c = COL_WHITE);
    void DrawCircle(Circlef circ, int verts, Color col);
    void DrawCircle(Circlef circ, int verts, int texId, Color col);
    void DrawRing(float innerRadius, float outerRadius, Color col);
    void Buffer();
    void ClearBuffers();
    void Render();
    void Render(ShaderProgram& sh);
    void RenderPolies(ShaderProgram& sh);
    ModelTransform& GetTransform()  {return mTransform;}
    glm::mat4& GetProjection()      {return mProjection;}
    Vector2f GetResolution()        {return mResolution;}
    Vector2f GetNormalizeVec()      {return mNormalize;}
    void SetResolution(Vector2f res);
    void SetPerspective(float fov, float aspect, float znear, float zfar);
    void SetProjection(glm::mat4 proj, bool perspective = true)      {mProjection = proj; mPerspective = perspective;}
    void SetOrtho();
    void CreateBuffers(size_t count);
};
}
#endif // SCENERENDERER_H_INCLUDED

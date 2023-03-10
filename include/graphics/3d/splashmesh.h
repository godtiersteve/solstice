#ifndef SPLASHMESH_H_INCLUDED
#define SPLASHMESH_H_INCLUDED

#include "graphics/scenerenderer.h"
#include "mesh.h"
#include "graphics/plane.h"
#include "graphics/vertex3.h"
#include "physics/spring.h"

namespace solstice {
struct WaterDeltas {
    float top, bottom, left, right, topLeft, topRight, botRight, botLeft;
    WaterDeltas() {
        top = 0;
        bottom = 0;
        left = 0;
        right = 0;
        topLeft = 0;
        topRight = 0;
        botRight = 0;
        botLeft = 0;
    }
};
class SplashMesh : public MeshBase<glm::vec3> {
protected:
    Vector2f mPosition;
    float mFrontHeight;
    int mNumIter;
    size_t mXSprings, mZSprings;
    float mSize;
    std::vector<Spring> mSprings;
    GLuint mHeightBuffer;
    GLuint mNormalBuff, mColBuff, mTexBuff;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec4> mColors;
    std::vector<glm::vec2> mTexCoords;
    std::vector<float> mHeights;
    void UpdateMesh();
    void CalcNormals();
public:
    float mSpread;
    float mDamping, mSpringConstant;
    float xScale, yScale;
    float bottom;
    SplashMesh();
    SplashMesh(float size, size_t xcount, size_t zcount);
    SplashMesh(float size, float frontHeight, size_t xcount, size_t zcount);
    void Init(float size, size_t xcount, size_t zcount);
    void Init(float size, float frontHeight, size_t xcount, size_t zcount);
    void Update(float t, float dt);
    void Update2();
    void Splash(int i, int j, float speed);
    void SplashRange(size_t x, size_t z, float speed);
    void SplashRange(int xStart, int xEnd, int zStart, int zEnd, float speed);
    void Draw(SceneRenderer& scene);
    void Draw(ShaderProgram& p);
    void Draw(ShaderProgram& p, TextureArray& textures);
    void CreateMesh();
    float GetSpringHeight(size_t i);
    float GetSpringHeight(size_t x, size_t z);
    glm::vec3 GetNormal(size_t i, size_t j, size_t k);
    glm::vec3& GetVertex(size_t x, size_t z);
    Spring& GetSpring(size_t x, size_t z);
    Rectanglef GetHitBox(size_t x, size_t z);

    void SetIterations(float i)         {mNumIter = i;}
    int GetIterations()                 {return mNumIter;}
    float GetSize()                     {return mSize;}
    size_t GetZSprings()                {return mZSprings;}
    size_t GetXSprings()                {return mXSprings;}
    int GetNumIter()                    {return mNumIter;}
    float GetFrontHeight()              {return mFrontHeight;}
    void SetupMesh();
    void DrawVao();
};

}



#endif // SPLASHMESH_H_INCLUDED

#ifndef SPLASH_H_INCLUDED
#define SPLASH_H_INCLUDED

#include "graphics/scenerenderer.h"
#include "mesh.h"
#include "graphics/plane.h"
#include "graphics/vertex3.h"
#include "physics/spring.h"

namespace solstice {
class Splash {
    std::vector<Spring> mSprings;
    size_t mColumns;
public:
    Splash(size_t columns = 10);

    void Update(double dt);
    void Draw(ShaderProgram& shader);
    void Splash(size_t i, float force);
};


}

#endif // SPLASH_H_INCLUDED

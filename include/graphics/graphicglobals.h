#ifndef GRAPHICGLOBALS_H_INCLUDED
#define GRAPHICGLOBALS_H_INCLUDED

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/functions.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "SDL2/sdl.h"
#include "SDL2/SDL_image.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "GL/glu.h"
#include "vector2.h"
#include "color.h"
#include "modeltransform.h"

#include <vector>
#include <string>

namespace solstice {
    const GLuint SOLSTICE_RENDER_TRIANGLES[] = {0, 1, 2, 0, 2, 3};
    enum DrawPoint {DRAW_TOPLEFT, DRAW_TOPCENTER, DRAW_TOPRIGHT, DRAW_RIGHTCENTER, DRAW_BOTRIGHT,
                    DRAW_BOTCENTER, DRAW_BOTLEFT, DRAW_LEFTCENTER, DRAW_CENTER, DRAW_HOTSPOT};
    DrawPoint IntToDrawPoint(int i);

    Uint32 GetSurfacePixel32(Vector2i origin, SDL_Surface* surface);
    int GetCellWidth(SDL_Surface* surface, Vector2i origin, short h, short w);
    int GetCellHeight(SDL_Surface* surface, Vector2i origin, short h, short w);
    Vector2i GetCellHW(SDL_Surface* surface, Vector2i origin, short h, short w);

    int CheckSurfaceTransparencyLeft(SDL_Surface* surface, Vector2i& origin, short h, short w);
    int CheckSurfaceTransparencyRight(SDL_Surface* surface, Vector2i& origin, short h, short w);
    int CheckSurfaceTransparencyTop(SDL_Surface* surface, Vector2i& origin, short h, short w);
    int CheckSurfaceTransparencyBottom(SDL_Surface* surface, Vector2i& origin, short h, short w);

    void DefaultTextureMatrix(GLfloat* arr);
    void PerspectiveMatrix(GLfloat* matrix, GLfloat fov, GLfloat a, GLfloat n, GLfloat f);
};
#endif // GRAPHICGLOBALS_H_INCLUDED


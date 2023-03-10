#ifndef GLOBALS_H
#define GLOBALS_H

#include "GL/glew.h"
#include "SDL2/sdl.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "GL/glu.h"
#include "utility/util.h"

#include <string>
#include <cmath>
#include <iostream>

const int INTERNAL_RESOLUTION_WIDTH = 1280;
const int INTERNAL_RESOLUTION_HEIGHT = 720;


const double RESOLUTION_X_ADJUST = 2.0 / INTERNAL_RESOLUTION_WIDTH;
const double RESOLUTION_Y_ADJUST = 2.0 / INTERNAL_RESOLUTION_HEIGHT;

bool LockFPS(unsigned int fps, Uint32& lastTime);
int CloseApplication(int ret = 0);
void InitGL();
void TestPrint(std::string text);
namespace solstice {
    void SetupOpenGL();
    void EnableGLDebugging();
    void glMessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam);

}
#endif

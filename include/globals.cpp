#include "globals.h"

bool LockFPS(unsigned int fps, Uint32& lastTime){
	unsigned int currentTime = SDL_GetTicks();
	fps = 1000 / (fps + 4);
	if((currentTime - lastTime) >= fps) {
		lastTime = currentTime;
		return false;
	}
	return true;
}
int CloseApplication(int ret) {
    SDL_Quit();
    return ret;
}

void InitGL() {
	glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearDepth(5.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void TestPrint(std::string text) {std::cout << text << std::endl;}
namespace solstice {
void SetupOpenGL() {
    glewInit();
    InitGL();
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_3D);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
void glMessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x\nseverity = 0x%x\nmessage = %s\n\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}
void EnableGLDebugging() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback( (GLDEBUGPROC) glMessageCallback, 0 );
    glDebugMessageControl(GL_DONT_CARE,
                          GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION,
                          0, 0, GL_FALSE);
}
}

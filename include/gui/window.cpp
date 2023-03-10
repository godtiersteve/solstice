#include "window.h"
namespace solstice {
Uint32 Window::sCurrentGLContext = 0;
Window::Window()
{
    mWindow = NULL;
    mWindowID = -1;
}

Window::Window(string title, Rectangle<int> dimensions, Uint32 flags) {
    mWindow = NULL;
    mDimensions = dimensions;
    MakeWindow(title, mDimensions.GetX(), mDimensions.GetY(), mDimensions.GetHeight(), mDimensions.GetWidth(), flags);
}

Window::Window(string title, int x, int y, int h, int w, Uint32 flags) {
    mWindow = NULL;
    mDimensions.SetXYHW(x, y, h, w);
    MakeWindow(title, x, y, h, w, flags);
}

Window::~Window() {
    Close();
}

void Window::SetDimensions(int x, int y, int h, int w) {
    mDimensions.SetXYHW(x, y, h, w);
}

void Window::Hide() {
    SDL_HideWindow(mWindow);
}
void Window::Show() {
    SDL_ShowWindow(mWindow);
}
void Window::Close() {
    if(mWindow) {
        SDL_DestroyWindow(mWindow);
    }
    SDL_GL_DeleteContext(mGLContext);
    mWindowID = -1;
}
void Window::MakeWindow(string title, int x, int y, int w, int h, Uint32 flags) {
    Close();
    mWindow = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    mWindowID = SDL_GetWindowID(mWindow);
}

void Window::SwapWindow() {
    SDL_GL_SwapWindow(mWindow);
}

void Window::SetGLContext() {
    mGLContext = SDL_GL_CreateContext(mWindow);
}

void Window::SetBorder(bool border) {
    SDL_SetWindowBordered(mWindow, border ? SDL_TRUE : SDL_FALSE);
}
Uint32 Window::GetWindowID() {
    return mWindowID;
}
void DefaultWindowHandleEvents(SDL_Event& event) {
}
void Window::ClearBits(GLenum colorBits) {
    //glClearColor(mClearColor.red, mClearColor.green, mClearColor.blue, mClearColor.alpha);
    glClear(colorBits);
}
void Window::SetClearColor(Color col) {
    mClearColor = col;
    glClearColor(mClearColor.red, mClearColor.green, mClearColor.blue, mClearColor.alpha);
}

void Window::SetClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    mClearColor.red = r;
    mClearColor.green = g;
    mClearColor.blue = b;
    mClearColor.alpha = a;
    glClearColor(mClearColor.red, mClearColor.green, mClearColor.blue, mClearColor.alpha);
}
Color Window::GetClearColor() {return mClearColor;}
void Window::SetFullScreen(Uint32 flags) {
    SDL_SetWindowFullscreen(mWindow, flags);
}
void Window::MakeCurrentContext() {
    if(sCurrentGLContext != mWindowID) {
        SDL_GL_MakeCurrent(mWindow, mGLContext);
        sCurrentGLContext = mWindowID;
    }
}
};


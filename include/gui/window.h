#ifndef SDL_Window_H
#define SDL_Window_H

#include "SDL2/sdl.h"
#include <functional>
#include "geometry/rectangle.h"
#include <string>

using std::string;
using std::function;
namespace solstice {
class Window {
private:
    SDL_Window* mWindow;
    SDL_GLContext mGLContext;
    Rectangle<int> mDimensions;
    Uint32 mFlags;
    string mTitle;
    Uint32 mWindowID;
    Color mClearColor;
    static Uint32 sCurrentGLContext;
public:
    Window();
    Window(string title, Rectangle<int> dimensions, Uint32 flags);
    Window(string title, int x, int y, int h, int w, Uint32 flags);
    ~Window();

    SDL_Window* GetWindow() { return mWindow; }
    void SetWindow(SDL_Window* val) { mWindow = val; }
    Rectangle<int> GetDimensions() { return mDimensions; }

    void SetDimensions(Rectangle<int> val) { mDimensions = val; }

    Uint32 GetFlags() { return mFlags; }
    void SetFlags(Uint32 val) { mFlags = val; }

    string GetTitle() { return mTitle; }
    void SetTitle(string val) { mTitle = val; }

    void SetDimensions(int x, int y, int h, int w);
    int GetHeight() {return mDimensions.GetHeight();}
    int GetWidth() {return mDimensions.GetWidth();}
    int GetX() {return mDimensions.GetX();}
    int GetY() {return mDimensions.GetY();}

    void Hide();
    void Show();
    void Close();
    void MakeWindow(string title, int x, int y, int w, int h, Uint32 flags);
    void SwapWindow();
    void SetGLContext();
    void SetBorder(bool border);

    void SetClearColor(Color col);
    void SetClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void ClearBits(GLenum colorBits);
    Color GetClearColor();
    void SetFullScreen(Uint32 flags);
    SDL_GLContext GetGLContext()               {return mGLContext;}
    void MakeCurrentContext();

    Uint32 GetWindowID();

    std::function<void(SDL_Event&)> HandleEvents;
};
void DefaultWindowHandleEvents(SDL_Event& event);

};
#endif // Window_H

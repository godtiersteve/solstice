#include "graphicglobals.h"
namespace solstice {
    DrawPoint IntToDrawPoint(int i) {
        if(i > 0 && i < DRAW_HOTSPOT) {
            return static_cast<DrawPoint>(i);
        }
        else {
            return DRAW_BOTLEFT;
        }

    }
    Uint32 GetSurfacePixel32(Vector2i origin, SDL_Surface* surface) {
        if(!surface) {
            return 0;
        }
        Uint32* pixels = (Uint32*)surface->pixels;
        return pixels[(origin.y * surface->w) + origin.x];
    }
    int GetCellWidth(SDL_Surface* surface, Vector2i origin, short h, short w) {
        int l = CheckSurfaceTransparencyLeft(surface, origin, h, w);
        int r = CheckSurfaceTransparencyRight(surface, origin, h, w);
        return r - l;
    }
    int GetCellHeight(SDL_Surface* surface, Vector2i origin, short h, short w) {
        int t = CheckSurfaceTransparencyTop(surface, origin, h, w);
        int b = CheckSurfaceTransparencyBottom(surface, origin, h, w);
        return b - t;
    }
    Vector2i GetCellHW(SDL_Surface* surface, Vector2i origin, short h, short w) {
        int l  = CheckSurfaceTransparencyLeft(surface, origin, h, w);
        int r = CheckSurfaceTransparencyRight(surface, origin, h, w);
        int t = CheckSurfaceTransparencyTop(surface, origin, h, w);
        int b = CheckSurfaceTransparencyBottom(surface, origin, h, w);
        Vector2i foo(r - l, b - t);
        return foo;
    }

    int CheckSurfaceTransparencyLeft(SDL_Surface* surface, Vector2i& origin, short h, short w) {
        int xEnd = origin.x + w;
        int yEnd = origin.y + h;
        Uint32 trans = GetSurfacePixel32(0, surface);

        for(int i = origin.x; i < xEnd; i++) {
            for(int j = origin.y; j < yEnd; j++) {
                if(GetSurfacePixel32(Vector2i(i, j), surface) != trans) {
                    return i;
                }
            }
        }
        return origin.x;
    }
    int CheckSurfaceTransparencyRight(SDL_Surface* surface, Vector2i& origin, short h, short w) {
        int xEnd = origin.x + w;
        int yEnd = origin.y + h;
        Uint32 trans = GetSurfacePixel32(0, surface);

        for(int i = xEnd; i > origin.x; i--) {
            for(int j = origin.y; j < yEnd; j++) {
                if(GetSurfacePixel32(Vector2i(i, j), surface) != trans) {
                    return i;
                }
            }
        }
        return origin.x;
    }
    int CheckSurfaceTransparencyTop(SDL_Surface* surface, Vector2i& origin, short h, short w) {
        int xEnd = origin.x + w;
        int yEnd = origin.y + h;
        Uint32 trans = GetSurfacePixel32(0, surface);

        for(int i = origin.y; i < yEnd; i++) {
            for(int j = origin.x; j < xEnd; j++) {
                if(GetSurfacePixel32(Vector2i(j, i), surface) != trans) {
                    return i;
                }
            }
        }
        return origin.y;
    }
    int CheckSurfaceTransparencyBottom(SDL_Surface* surface, Vector2i& origin, short h, short w) {
        int xEnd = origin.x + w;
        int yEnd = origin.y + h;
        Uint32 trans = GetSurfacePixel32(0, surface);

        for(int i = yEnd; i > origin.y; i--) {
            for(int j = origin.x; j < xEnd; j++) {
                if(GetSurfacePixel32(Vector2i(j, i), surface) != trans) {
                    return i;
                }
            }
        }
        return origin.y;
    }
    void DefaultTextureMatrix(GLfloat* arr) {
        arr[0] = 0.0f;
        arr[1] = 1.0f;
        arr[2] = 1.0;
        arr[3] = 1.0f;
        arr[4] = 1.0f;
        arr[5] = 0.0f;
        arr[6] = 0.0f;
        arr[7] = 0.0f;
    }
    void PerspectiveMatrix(GLfloat* matrix, GLfloat fov, GLfloat a, GLfloat n, GLfloat f) {
        for(int i = 0; i < 16; i++) {
            matrix[i] = 0;
        }
        float d = 1.0f / (tan(fov / 2));
        matrix[0] =  d / a;
        matrix[5] =  d;
        matrix[10] = (n + f) / (n - f);
        matrix[14] = (2 * n * f) / (n - f);
        matrix[11] = -1;
    }
};


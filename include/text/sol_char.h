// TODO (Stephen#1#): Encapsulate SolChar

#ifndef SOLCHAR_H_INCLUDED
#define SOLCHAR_H_INCLUDED

#include "graphics/quaddata.h"
#include "utility/sol_math.h"

#include <iostream>

namespace solstice {
    struct SolChar {
    protected:
        QuadData mDrawingQuad;
    public:
        int pitch;
        char letter;
        int left, top;
        int width, height;
        float xOff, yOff;
        int advance;
        SolChar() {
            pitch = 0;
            letter = '\0';
            left = 0;
            top = 0;
            width = 0;
            height = 0;
            xOff = 0;
            yOff = 0;
            advance = 0;
        }
        SolChar(char let, int lft, int tp): letter(let), left(lft), top(tp) {}
        QuadData& GetDrawingQuad();
    };

    struct Letter {
        char letter;
        QuadData quad;
        float y;
        bool read;
        bool fades, scales, warps;
        float scaleStart, scaleEnd, scaleStep;
        float fadeMin, fadeMax, fadeStep;
        float amplitude, frequency, phase, angle, angleStep;

        Letter();
        Letter(char let, Color col);
        Letter(char let, int x, int y);
        Letter(char let, Color col, int x, int y);
        Letter& operator=(Letter l);

        void Init();
        void Update();
        void Scale();
        void Fade();
        void Warp();
        void Stop();
        void Reset();
        void Complete();
        void ResetFade();
        void ResetScale();
        void ResetWarp();
        void SetScale(float start, float end, float step);
        void SetFade(float min, float max, float step);
        void SetWarp(float freq, float amp, float phase, float angleStep);
    };
};

#endif // GLCHAR_H_INCLUDED

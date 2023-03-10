#include "sol_char.h"

namespace solstice {
    QuadData& SolChar::GetDrawingQuad() {
        return mDrawingQuad;
    }
    Letter::Letter():
    letter('\0'), read(false), fades(false), scales(false), warps(false),
    scaleStart(0.0f), scaleEnd(1.0f), scaleStep(0.05f),
    fadeMin(0.0f), fadeMax(1.0f), fadeStep(0.05f),
    amplitude(0), frequency(0), phase(0), angle(0), angleStep(0)
    {
        y = quad.GetY();
    }

    Letter::Letter(char let, Color col):
    letter(let), read(false), fades(false), scales(false), warps(false),
    scaleStart(0.0f), scaleEnd(1.0f), scaleStep(0.05f),
    fadeMin(0.0f), fadeMax(1.0f), fadeStep(0.05f),
    amplitude(0), frequency(0), phase(0), angle(0), angleStep(0)
    {
        quad.SetColor(col);
        quad.SetXY(Vector2<float>(0));
        y = quad.GetY();
    }

    Letter::Letter(char let, int x, int y):
    letter(let), read(false), fades(false), scales(false), warps(false),
    scaleStart(0.0f), scaleEnd(1.0f), scaleStep(0.05f),
    fadeMin(0.0f), fadeMax(1.0f), fadeStep(0.05f),
    amplitude(0), frequency(0), phase(0), angle(0), angleStep(0)
    {
        quad.SetXY(Vector2<float>(x, y));
        y = quad.GetY();
    }

    Letter::Letter(char let, Color col, int x, int y):
    letter(let), read(false), fades(false), scales(false), warps(false),
    scaleStart(0.0f), scaleEnd(1.0f), scaleStep(0.05f),
    fadeMin(0.0f), fadeMax(1.0f), fadeStep(0.05f),
    amplitude(0), frequency(0), phase(0), angle(0), angleStep(0)
    {
        quad.SetXY(Vector2<float>(0));
        y = quad.GetY();
    }
    Letter& Letter::operator=(Letter l) {
        letter = l.letter;
        quad = l.quad;
        y = l.y;
        read = l.read;
        fades = l.fades;
        scales = l.scales;
        warps = l.warps;
        scaleStart = l.scaleStart;
        scaleEnd = l.scaleEnd;
        scaleStep = l.scaleStep;
        fadeMin = l.fadeMin;
        fadeMax = l.fadeMax;
        fadeStep = l.fadeStep;
        amplitude = l.amplitude;
        frequency = l.frequency;
        phase = l.phase;
        angle = l.angle;
        angleStep = l.angleStep;
        return *this;
    }
    void Letter::Init() {
        y = quad.GetY();
    }
    void Letter::Update() {
        if(fades)
            Fade();
        if(scales)
            Scale();
        if(warps)
            Warp();
    }
    void Letter::Scale() {
        if(scaleStep < 0) {
            if(quad.GetScale() > scaleEnd)
                quad.SetScale(quad.GetScale() + scaleStep);
            if(quad.GetScale() < scaleEnd)
                quad.SetScale(scaleEnd);
        }
        if(scaleStep > 0) {
            if(quad.GetScale() < scaleEnd)
                quad.SetScale(quad.GetScale() + scaleStep);
            if(quad.GetScale() > scaleEnd)
                quad.SetScale(scaleEnd);
        }
    }
    void Letter::Fade() {
        if(quad.GetColor().alpha < fadeMax)
            quad.GetColor().alpha+= fadeStep;
    }
    void Letter::Warp() {
        quad.SetY(y + (amplitude * sin((angle * frequency) + phase)));
        phase+= angleStep;
    }

    void Letter::Stop() {Reset(); warps = false; scales = false; fades = false;}
    void Letter::Reset() {ResetFade(); ResetScale(); ResetWarp();}
    void Letter::Complete() {
        if(fades) {
            quad.GetColor().alpha = fadeMax;
        }
        if(scales) {
            quad.SetScale(scaleEnd);
        }
        if(warps) {
            quad.SetAngle(0);
        }
    }
    void Letter::ResetFade() {
        if(fades)
            quad.GetColor().alpha = fadeMin;
    }
    void Letter::ResetScale() {
        if(scales)
            quad.SetScale(scaleStart);
    }
    void Letter::ResetWarp() {
        if(warps) {
            angle = 0.0f;
            quad.SetAngle(angle);
        }
    }
    void Letter::SetScale(float start, float end, float step) {
        scaleStart = start;
        scaleEnd = end;
        scaleStep = step;
        ResetScale();
        scales = true;
    }
    void Letter::SetFade(float min, float max, float step) {
        fadeMin = min;
        fadeMax = max;
        fadeStep = step;
        ResetFade();
        fades = true;
    }
    void Letter::SetWarp(float freq, float amp, float phase, float angleStep) {
        amplitude = amp;
        frequency = freq;
        this->phase = phase;
        this->angleStep = angleStep;
        warps = true;
    }
};


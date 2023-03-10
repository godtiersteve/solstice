#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "graphics/quaddata.h"
#include "geometry/geometry.h"
#include "graphics/QuadData.h"
#include "guicomponent.h"
#include "text/sol_font.h"
#include "tooltip.h"
#include "events/eventobserver.h"
#include <vector>
#include <string>
#include <functional>
namespace solstice {
    enum ButtonState {BUTTON_INACTIVE, BUTTON_ACTIVE, BUTTON_HOVER, BUTTON_PRESSED, BUTTON_RELEASED};
    class Button : public GUIComponent, public GUIEventSubject {
    protected:
        ButtonState mState;
        bool mClicked, mIsActive;
        short mCurFrame, mReleaseDur;
        string mText;
        Vector2<float> mPressOffset, mReleaseOffset;
        short mToolTipTimer, mToolTipFrame;
        FontData* mFont;
        QuadData* mDrawQuad;

    public:
        QuadData mInactive, mHover, mPress, mRelease, mActive;
        QuadData mIcon;
        int mID;
        ToolTip mToolTip;

        Button();
        Button(Vector2f origin, int h, int w, short texID = -1);
        Button(Rectanglef rect, QuadData inactive, short texID = -1);
        Button(Rectanglef rect, ClippingRect clip, short textureID);
        Button(const Button& button);
        Button& operator=(const Button& button);

        void Init(Vector2f origin, int h, int w, short texID = - 1);
        void Init(Rectanglef rect, QuadData inactive, short texID = -1);

        std::function<void(void)> ClickAction;

        void SetAllButtons(QuadData inactive, QuadData hover, QuadData press, QuadData release, QuadData active);
        void SetInactive(QuadData data);
        void SetActive(QuadData data);
        void SetRelease(QuadData data);
        void SetHover(QuadData data);
        void SetPress(QuadData data);

        void InitInactive(Vector2f origin, float h, float w, float texh, float texw);
        void InitActive(Vector2f origin, float h, float w, float texh, float texw);
        void InitRelease(Vector2f origin, float h, float w, float texh, float texw);
        void InitHover(Vector2f origin, float h, float w, float texh, float texw);
        void InitPress(Vector2f origin, float h, float w, float texh, float texw);

        void SetString(string str);
        void SetFont(FontData* font);
        void SetPressOffset(Vector2f off);
        void SetReleaseOffset(Vector2f off);
        void SetOffsets(Vector2f press, Vector2f release);
        void SetToolTipFrame(short frame);

        Vector2f GetPressOffset();
        Vector2f GetReleaseOffset();
        short GetCurrentFrame();
        short GetReleaseDur();
        short GetToolTipTimer();
        short GetToolTipFrame();
        ToolTip& GetToolTip();
        string GetText();
        ButtonState GetState()      {return mState;}

        QuadData& GetDrawQuad();
        QuadData& GetInactive();
        QuadData& GetHover();
        QuadData& GetActive();
        QuadData& GetPress();
        QuadData& GetRelease();

        void Trigger();
        void SetIsActive(bool flag);
        bool Triggered();
        bool Pressed();
        bool GetClicked() {return mClicked;}
        bool IsActive();

        void SetColors(Color c);

        void Press();
        void Release();
        void Reset();
        void UpdateDrawQuad();
        void Update(SDL_Event& event, Vector2f pt);
        void Update(Vector2f pt);
        void Update();
        void ChangeState(ButtonState id);
        void Draw(SceneRenderer& rend);
    };
}

#endif // BUTTON_H_INCLUDED

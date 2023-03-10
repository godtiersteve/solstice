#include "button.h"
namespace solstice {
    Button::Button():
    mState(BUTTON_INACTIVE), mClicked(false), mCurFrame(0),
    mIsActive(false), mReleaseDur(5),
    mToolTipTimer(0), mToolTipFrame(10),
    mFont(NULL)
    {
        mPressOffset.x = 3;
        mPressOffset.y = -3;
        mReleaseOffset.x = 0;
        mReleaseOffset.y = 0;
        mIcon.mColor = COL_EMPTY;
        mDrawQuad = &mInactive;
        ChangeState(BUTTON_INACTIVE);
        mID = -1;
    }
    Button::Button(Vector2<float> origin, int height, int width, short texID):
    mState(BUTTON_INACTIVE), mClicked(false), mCurFrame(0),
    mIsActive(false), mReleaseDur(5),
    mToolTipTimer(0), mToolTipFrame(10),
    mPressOffset(0), mReleaseOffset(0)
    {
        Init(origin, height, width, texID);
        mInactive = *this;
        mHover = *this;
        mPress = *this;
        mRelease = *this;
        mActive = *this;
        ChangeState(BUTTON_INACTIVE);
        mIcon.mColor = COL_EMPTY;
        mDrawQuad = &mInactive;
        mID = -1;
    }
    Button::Button(Rectangle<float> rect, QuadData inactive, short texID):
    mState(BUTTON_INACTIVE), mClicked(false), mCurFrame(0),
    mReleaseDur(5),
    mToolTipTimer(0), mToolTipFrame(10),
    mFont(NULL),
    mPressOffset(0), mReleaseOffset(0)
    {
        Init(rect, inactive, texID);
        ChangeState(BUTTON_INACTIVE);
        mIcon.mColor = COL_EMPTY;
        mDrawQuad = &mInactive;
        mID = -1;
    }
    Button::Button(Rectanglef rect, ClippingRect clip, short textureID) {
        mTextureID = textureID;
        mPosition = rect.GetPosition();
        rect.GetPosition() = 0;
        mInactive = mHover = mPress = mRelease = mActive = rect;
        Vector2f origin(0);

        mInactive.SetTextureCoords(origin, clip.clipHeight, clip.clipWidth, clip.height, clip.width);
        origin.x+= clip.clipWidth;
        mInactive.mTextureID = textureID;

        mHover.SetTextureCoords(origin, clip.clipHeight, clip.clipWidth, clip.height, clip.width);
        origin.x+= clip.clipWidth;
        mHover.mTextureID = textureID;

        mPress.SetTextureCoords(origin, clip.clipHeight, clip.clipWidth, clip.height, clip.width);
        origin.x+= clip.clipWidth;
        mPress.mTextureID = textureID;

        mRelease.SetTextureCoords(origin, clip.clipHeight, clip.clipWidth, clip.height, clip.width);
        origin.x+= clip.clipWidth;
        mRelease.mTextureID = textureID;

        mActive.SetTextureCoords(origin, clip.clipHeight, clip.clipWidth, clip.height, clip.width);
        origin.x+= clip.clipWidth;
        mActive.mTextureID = textureID;

        mTextureID = textureID;
        SetXYHW(rect.GetOrigin(), rect.GetHeight(), rect.GetWidth());
        mDrawQuad = &mInactive;
    }
    Button::Button(const Button& button) {
        *this = button;
    }
    Button& Button::operator=(const Button& button) {
        mInactive = button.mInactive;
        mHover = button.mHover;
        mPress = button.mPress;
        mRelease = button.mRelease;
        mActive = button.mActive;
        mClicked = button.mClicked;
        mIsActive = button.mIsActive;
        mCurFrame = button.mCurFrame;
        mReleaseDur = button.mReleaseDur;
        mText = button.mText;
        mPressOffset = button.mPressOffset;
        mReleaseOffset = button.mReleaseOffset;
        mToolTipTimer = button.mToolTipTimer;
        mToolTipFrame = button.mToolTipFrame;
        mToolTip = button.mToolTip;
        mFont = button.mFont;
        ClickAction = button.ClickAction;
        Init(button.mPosition, button.mH, button.mW);
        mColor = button.mColor;
        ChangeState(button.mState);
        mHidden = button.mHidden;
        mHoriFlipped = button.mHoriFlipped;
        mVertFlipped = button.mVertFlipped;
        mTextureID = button.mTextureID;
        mShaderID = button.mShaderID;
        mTextureRef = button.mTextureRef;
        mShaderRef = button.mShaderRef;
        mTextureCoords = button.mTextureCoords;
        mDrawQuad = &mInactive;
        mIcon = button.mIcon;
        mObservers = button.mObservers;
        return *this;
    }
    void Button::Init(Vector2<float> origin, int h, int w, short texID) {
        Rectangle<float>::SetXYHW(origin, h, w);
        SetPressOffset(0);
        SetReleaseOffset(0);
        mDrawQuad = &mInactive;
        ChangeState(BUTTON_INACTIVE);
    }
    void Button::Init(Rectangle<float> rect, QuadData inactive, short texID) {
        Rectangle<float>::SetXYHW(rect.GetOrigin(), rect.GetHeight(), rect.GetWidth());
        mInactive = inactive;
        mActive = inactive;
        mHover = inactive;
        mPress = inactive;
        mRelease = inactive;
        SetPressOffset(0);
        SetReleaseOffset(0);
        mDrawQuad = &mInactive;
        ChangeState(BUTTON_INACTIVE);
    }

    void Button::SetAllButtons(QuadData inactive, QuadData hover, QuadData press, QuadData release, QuadData active) {
        mInactive = inactive;
        mHover = hover;
        mPress = press;
        mRelease = release;
        mActive = active;
    }
    void Button::SetInactive(QuadData data)      {mInactive = data;}
    void Button::SetActive(QuadData data)        {mActive = data;}
    void Button::SetRelease(QuadData data)       {mRelease = data; mRelease.AddXY(mReleaseOffset);}
    void Button::SetHover(QuadData data)         {mHover = data;}
    void Button::SetPress(QuadData data)         {mPress = data; mPress.AddXY(mPressOffset);}

    void Button::SetString(string str)                                          {mText = str;}
    void Button::SetFont(FontData* font)                                        {mFont = font;}
    void Button::SetPressOffset(Vector2<float> off)                             {mPressOffset = off; mPress.AddXY(off);}
    void Button::SetReleaseOffset(Vector2<float> off)                           {mReleaseOffset = off; mRelease.AddXY(off);}
    void Button::SetOffsets(Vector2<float> press, Vector2<float> release) {
        mPressOffset = press;
        mPress.AddXY(press);
        mReleaseOffset = release;
        mRelease.AddXY(release);
    }
    void Button::SetToolTipFrame(short frame)                                   {mToolTipFrame = frame;}
    void Button::SetColors(Color c) {
        mInactive.mColor = c;
        mHover.mColor = c;
        mPress.mColor = c;
        mRelease.mColor = c;
        mActive.mColor = c;
        mIcon.mColor = c;
    }
    Vector2<float> Button::GetPressOffset()         {return mPressOffset;}
    Vector2<float> Button::GetReleaseOffset()       {return mReleaseOffset;}
    short Button::GetCurrentFrame()                 {return mCurFrame;}
    short Button::GetReleaseDur()                   {return mReleaseDur;}
    short Button::GetToolTipTimer()                 {return mToolTipTimer;}
    short Button::GetToolTipFrame()                 {return mToolTipFrame;}
    string Button::GetText()                        {return mText;}
    QuadData& Button::GetDrawQuad()                 {return *mDrawQuad;}

    void Button::Trigger()                          {mClicked = true;}
    void Button::SetIsActive(bool flag)             {mIsActive = flag;}
    bool Button::Triggered()                        {return mClicked;}
    bool Button::Pressed()                          {return mClicked;}
    bool Button::IsActive()                         {return mIsActive;}
    ToolTip& Button::GetToolTip()                   {return mToolTip;}

    void Button::Press() {
        ChangeState(BUTTON_PRESSED);
    }
    void Button::Release() {
        mClicked = true;
        mCurFrame = 0;
        ChangeState(BUTTON_RELEASED);
    }
    void Button::Reset() {
        mClicked = false;
        mCurFrame = 0;
        ChangeState(BUTTON_INACTIVE);
    }
    void Button::UpdateDrawQuad() {
        mDrawQuad->SetHidden(mHidden);
        /*
        Vector2f pos;
        switch(mState) {
            case BUTTON_INACTIVE:
                pos = mInactive.GetOrigin();
                break;
            case BUTTON_ACTIVE:
                pos = mActive.GetOrigin();
                break;
            case BUTTON_HOVER:
                pos = mHover.GetOrigin();
                break;
            case BUTTON_PRESSED:
                pos = mPress.GetOrigin();
                break;
            case BUTTON_RELEASED:
                pos = mRelease.GetOrigin();
                break;
            default:
                pos = mInactive.GetOrigin();
                break;
        }
        */
        mDrawQuad->SetXY(mPosition);
    }
    void Button::Update() {
        Vector2f pos = GetBotRight();
        pos.y-= mToolTip.GetTextDimensions().y;
        int y = mToolTip.GetTextDimensions().y;
        mToolTip.SetPosition(pos);
        mToolTip.Update();
    }
    void Button::Update(SDL_Event& event, Vector2<float> pt)  {
        if(PointRectCollision(pt, *this)) {
            if(mState != BUTTON_PRESSED && mState != BUTTON_RELEASED)
                ChangeState(BUTTON_HOVER);
        }
        else if(mState != BUTTON_RELEASED) {
            if(!mIsActive) {
                ChangeState(BUTTON_INACTIVE);
            }
            else {
                ChangeState(BUTTON_ACTIVE);
            }
            mToolTip.SetHidden(true);
            mToolTipTimer = 0;
        }
        else {
            mToolTipTimer = 0;
        }

        if(mState == BUTTON_HOVER) {
            if(mToolTipTimer > mToolTipFrame) {
                if(mToolTip.Hidden()) {
                    mToolTip.SetHidden(false);
                }
            }
            else if(mToolTipTimer >= 0)
                mToolTipTimer++;

            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                Press();
                mToolTip.SetHidden(true);
                mToolTipTimer = -1;
            }
        }
        else if(mState == BUTTON_PRESSED) {
            if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
                Release();
        }
        else if(mState == BUTTON_RELEASED) {
            ChangeState(BUTTON_INACTIVE);
        }

        if(mClicked) {
            if(ClickAction != nullptr) {
                ClickAction();
            }
            if(mCurFrame == 1)
                mClicked = false;
        }
        if(mCurFrame >= 0 && mCurFrame < mReleaseDur) {
            mCurFrame++;
        }
        if(mCurFrame >= mReleaseDur) {
            mCurFrame = 0;
        }
        if(!mToolTip.Empty()) {
            mToolTip.Update();
        }
        UpdateDrawQuad();
        mIcon.SetPosition(mDrawQuad->GetCenter());
    }
    void Button::Update(Vector2<float> pt) {
        if(PointRectCollision(pt, *this))
            ChangeState(BUTTON_HOVER);
        else if(!mIsActive)
            ChangeState(BUTTON_INACTIVE);
        else
            ChangeState(BUTTON_ACTIVE);

    }

    void Button::InitInactive(Vector2<float> origin, float h, float w, float texh, float texw) {
        mInactive.Init(origin, h, w);
        mInactive.SetTextureCoords(texh, texw);
    }
    void Button::InitActive(Vector2<float> origin, float h, float w, float texh, float texw) {
        mActive.Init(origin, h, w);
        mActive.SetTextureCoords(texh, texw);
    }
    void Button::InitRelease(Vector2<float> origin, float h, float w, float texh, float texw) {
        mRelease.Init(origin, h, w);
        mRelease.SetTextureCoords(texh, texw);
    }
    void Button::InitHover(Vector2<float> origin, float h, float w, float texh, float texw) {
        mHover.Init(origin, h, w);
        mHover.SetTextureCoords(texh, texw);
    }
    void Button::InitPress(Vector2<float> origin, float h, float w, float texh, float texw) {
        mPress.Init(origin, h, w);
        mPress.SetTextureCoords(texh, texw);
    }

    QuadData& Button::GetInactive()         {return mInactive;}
    QuadData& Button::GetHover()            {return mHover;}
    QuadData& Button::GetActive()           {return mActive;}
    QuadData& Button::GetPress()            {return mPress;}
    QuadData& Button::GetRelease()          {return mRelease;}

    void Button::ChangeState(ButtonState id) {
        mState = id;
        switch(id) {
            case BUTTON_INACTIVE:
                mDrawQuad = &mInactive;
                mToolTip.SetHidden(true);
                break;

            case BUTTON_ACTIVE:
                mDrawQuad = &mActive;
                break;

            case BUTTON_HOVER:
                mDrawQuad = &mHover;
                break;

            case BUTTON_PRESSED:
                mDrawQuad = &mPress;
                Notify(*this, SOL_EVENT_BUTTON_PRESSED);
                if(ClickAction) {
                    ClickAction();
                }
                break;

            case BUTTON_RELEASED:
                mDrawQuad = &mRelease;
                break;
            default:
                mDrawQuad = &mInactive;
                mToolTip.SetHidden(true);
                break;
        }
    }
    void Button::Draw(SceneRenderer& rend) {
        /*
        switch(mState) {
            case BUTTON_ACTIVE:
                rend.DrawQuadPoly(mActive);
                break;

            case BUTTON_HOVER:
                rend.DrawQuadPoly(mHover);
                break;

            case BUTTON_PRESSED:
                rend.DrawQuadPoly(mPress);
                break;

            case BUTTON_RELEASED:
                rend.DrawQuadPoly(mRelease);
                break;
            case BUTTON_INACTIVE:
                rend.DrawQuadPoly(mInactive);
                break;
            default:
                rend.DrawQuadPoly(mInactive);
                break;
        }
        */
        rend.DrawQuadPoly(*mDrawQuad);
        if(!mIcon.Empty()) {
            rend.DrawQuadPoly(mIcon);
        }
        mToolTip.Draw(rend);
    }
};

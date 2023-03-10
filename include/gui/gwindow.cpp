#include "gwindow.h"
namespace solstice {
GUIWindow::GUIWindow():
mTitle(""), mMoving(false), mResizing(false),
mCollapsed(false), mExpanded(false), mClosed(false),
mMaxWidth(300), mMaxHeight(300), mCurWidth(300), mCurHeight(300),
mFont(NULL)
{
    InitButtons();
}
GUIWindow::GUIWindow(QuadData titleBar, Button collapse, Button expand, Button close, Button restore):
mTitle(""), mMoving(false), mResizing(false),
mCollapsed(false), mExpanded(false), mClosed(false),
mMaxWidth(300), mMaxHeight(300), mCurWidth(300), mCurHeight(300),
mFont(NULL) {
    mTitleBar = titleBar;
    mCollapse = collapse;
    mExpand = expand;
    mClose = close;
    mRestore = restore;
    InitButtons();
}
GUIWindow::GUIWindow(QuadData titleBar, string title):
mTitle(""), mMoving(false), mResizing(false),
mCollapsed(false), mExpanded(false), mClosed(false),
mMaxWidth(300), mMaxHeight(300), mCurWidth(300), mCurHeight(300),
mFont(NULL) {
    mTitleBar = titleBar;
    mTitle = title;
    InitButtons();
}

void GUIWindow::InitButtons() {
    mCollapse.GetToolTip().SetText("Collapses the GUIWindow, hiding everything but the titlebar.");
    mExpand.GetToolTip().SetText("Expands the GUIWindow to its maximum size.");
    mCollapse.GetToolTip().SetText("Closes the GUIWindow.");
    mRestore.GetToolTip().SetText("Restores the GUIWindow.");
    mRestore.SetHidden(true);
}

void GUIWindow::SetTitle(string t) {mTitle = t;}
void GUIWindow::SetCollapse(Button c) {mCollapse = c;}
void GUIWindow::SetExpand(Button e) {mExpand = e;}
void GUIWindow::SetClose(Button c) {mClose = c;}
void GUIWindow::SetRestore(Button r) {mRestore = r;}
void GUIWindow::SetButtons(Button collapse, Button expand, Button close, Button restore) {
    mCollapse = collapse;
    mExpand = expand;
    mClose = close;
    mRestore = restore;
}
void GUIWindow::SetMaxHeight(int h)             {mMaxHeight = h;}
void GUIWindow::SetMaxWidth(int w)              {mMaxWidth = w;}
void GUIWindow::SetMaxHW(int h, int w)          {mMaxHeight = h; mMaxWidth = w;}
void GUIWindow::SetTitleBar(QuadData bar)       {mTitleBar = bar;}
void GUIWindow::SetFont(FontData* font)         {mFont = font;}
void GUIWindow::SetTitleColor(Color col)        {mTitleColor = col;}
void GUIWindow::SetTitlePos(Vector2<float> pos)       {mTitlePos = pos;}
void GUIWindow::SetTitlePos(float x, float y)   {mTitlePos.x = x; mTitlePos.y = y;}

void GUIWindow::SetHW(int h, int w, int maxH, int maxW) {
    Rectangle::SetHW(h, w);
    SetMaxHW(maxH, maxW);
}
string GUIWindow::GetTile()                {return mTitle;}
Button& GUIWindow::GetCollapse()           {return mCollapse;}
Button& GUIWindow::GetExpand()             {return mExpand;}
Button& GUIWindow::GetClose()              {return mClose;}
Button& GUIWindow::GetRestore()            {return mRestore;}
int GUIWindow::GetMaxWidth()               {return mMaxWidth;}
int GUIWindow::GetMaxHeight()              {return mMaxHeight;}
int GUIWindow::GetCurWidth()               {return mCurWidth;}
int GUIWindow::GetCurHeight()              {return mCurHeight;}
QuadData& GUIWindow::GetTitleBar()           {return mTitleBar;}
Color GUIWindow::GetTitleColor()           {return mTitleColor;}
Vector2<float> GUIWindow::GetTitlePos()          {return mTitlePos;}

void GUIWindow::Move(int x, int y) {
    Vector2<float> pos(x,y);
    SetXY(pos);
}
void GUIWindow::Move(Vector2<float> pos) {
    SetXY(pos);
}

void GUIWindow::Expand() {
    if(!mHidden) {
        Rectangle::SetHW(mMaxHeight, mMaxWidth);
    }
    mHidden = false;
}
void GUIWindow::Collapse() {
    mHidden = true;
}
void GUIWindow::Open() {
    mClosed = false;
    mExpand.SetHidden(false);
    mCollapse.SetHidden(false);
    mClose.SetHidden(false);
    mTitleBar.SetHidden(false);
    SetHidden(false);
}
void GUIWindow::Close() {
    mClosed = true;
    mExpand.SetHidden(true);
    mCollapse.SetHidden(true);
    mClose.SetHidden(true);
    mTitleBar.SetHidden(true);
    SetHidden(true);
}

void GUIWindow::Update() {
    if(mMoveStart.x != mMoveEnd.x && mMoveStart.y != mMoveEnd.y) {
        int xMove = mMoveEnd.x - mMoveStart.x;
        int yMove = mMoveEnd.y - mMoveStart.y;
        AddX(xMove);
        AddY(yMove);
        mMoveStart = mMoveEnd;
    }
    mTitleBar.SetWidth(GetWidth());
    mTitleBar.SetDrawFrom(DRAW_TOPLEFT);
    mTitleBar.SetXY(GetTopLeft());
    mRestore.SetXY(GetTopRight());
    mRestore.SetDrawFrom(DRAW_TOPRIGHT);
    mClose.SetXY(GetTopRight());
    mClose.SetDrawFrom(DRAW_TOPRIGHT);
    mExpand.SetXY(mClose.GetTopLeft());
    mExpand.SetDrawFrom(DRAW_TOPRIGHT);
    mCollapse.SetXY(mExpand.GetTopLeft());
    mCollapse.SetDrawFrom(DRAW_TOPRIGHT);
}
void GUIWindow::Update(SDL_Event& event, Vector2<float> cursor) {
    mClose.Update(event, cursor);
    mCollapse.Update(event, cursor);
    mExpand.Update(event, cursor);
    if(mClosed) {
        mRestore.Update(event, cursor);
        mRestore.SetHidden(false);
    }
    else {
        mRestore.SetHidden(true);
    }
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if(!mMoving) {
            if(PointRectCollision(cursor, mTitleBar)) {
                mMoving = true;
                mMoveStart = cursor;
            }
        }
    }
    else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        mMoving = false;
        mMoveStart = 0;
        mMoveEnd = 0;
    }
    if(mMoving) {
        mMoveEnd = cursor;
    }
    if(mClose.Pressed()) {
        Close();
    }
    else if(mCollapse.Pressed()) {
        Collapse();
    }
    else if(mExpand.Pressed()) {
        Expand();
    }
}

bool GUIWindow::Closed()       {return mClosed;}
bool GUIWindow::Opened()       {return !mClosed;}
bool GUIWindow::Collapsed()    {return mCollapsed;}
bool GUIWindow::Expanded()     {return mExpanded;}
bool GUIWindow::Resizing()     {return mResizing;}
bool GUIWindow::Moving()       {return mMoving;}
};

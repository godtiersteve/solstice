#ifndef GWINDOW_H_INCLUDED
#define GWINDOW_H_INCLUDED
// TODO (Stephen#1#): Test and properly implement "gwindow"

#include "text/sol_font.h"
#include "gui/button.h"
#include "graphics/quaddata.h"
#include <string>

namespace solstice {
using std::string;
class GUIWindow : public QuadData {
private:
    Button mCollapse, mExpand, mClose, mRestore;
    string mTitle;
    QuadData mTitleBar;
    bool mMoving, mResizing, mCollapsed, mExpanded, mClosed;
    int mMaxWidth, mMaxHeight;
    int mCurWidth, mCurHeight;
    Vector2<float> mMoveStart, mMoveEnd;
    FontData* mFont;
    Color mTitleColor;
    Vector2<float> mTitlePos;
    ToolTip mToolTip();
    void InitButtons();
public:
    GUIWindow();
    GUIWindow(QuadData titleBar, Button collapse, Button expand, Button close, Button restore);
    GUIWindow(QuadData titleBar, string title);

    void SetTitle(string t);
    void SetCollapse(Button c);
    void SetExpand(Button e);
    void SetClose(Button c);
    void SetRestore(Button r);
    void SetButtons(Button collapse, Button expand, Button close, Button restore);
    void SetMaxHeight(int h);
    void SetMaxWidth(int w);
    void SetMinHeight(int h);
    void SetMinWidth(int w);
    void SetMaxHW(int h, int w);
    void SetMinSize(int h, int w);
    void SetTitleBar(QuadData bar);
    void SetFont(FontData* font);
    void SetTitleColor(Color col);
    void SetTitlePos(Vector2<float> pos);
    void SetTitlePos(float x, float y);

    void SetHW(int h, int w, int maxH, int maxW);

    string GetTile();
    Button& GetCollapse();
    Button& GetExpand();
    Button& GetClose();
    Button& GetRestore();
    int GetMaxWidth();
    int GetMaxHeight();
    int GetMinWidth();
    int GetMinHeight();
    int GetCurWidth();
    int GetCurHeight();
    QuadData& GetTitleBar();
    Color GetTitleColor();
    Vector2<float> GetTitlePos();

    void Move(int x, int y);
    void Move(Vector2<float> pos);

    void Update();
    void Update(SDL_Event& event, Vector2<float> cursor);

    void Expand();
    void Collapse();
    void Open();
    void Close();
    void Restore();

    bool Closed();
    bool Opened();
    bool Collapsed();
    bool Expanded();
    bool Resizing();
    bool Moving();
};

};
#endif // GWINDOW_H_INCLUDED

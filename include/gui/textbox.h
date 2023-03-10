#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include <vector>
#include <deque>
#include <string>

#include "animation/animation.h"
#include "graphics/multiquad.h"
#include "text/sol_text.h"

using std::vector;
using std::deque;
using std::string;
namespace solstice {
    enum SpeakerPos {SPK_RIGHT, SPK_CENTER, SPK_LEFT, SPK_NONE};
    enum TextBoxMode {TXTBOX_ADV, TXTBOX_NVL};

// TODO (Stephen#1#): Implement backlog and NVL modes for textboxes
    class TextBox : public MultiQuadImg {
    protected:
        string mSpeaker;
        Vector2f mSpeakerRightPos, mSpeakerLeftPos, mSpeakerCenterPos, mCurSpeakerPos;
        Vector2f mTextOffset;
        Color mSpeakerColor;
        SolText mTextRenderer;
        SpeakerPos mSpeakerPos;
        deque<string> mText;
        int mLastLine, mCurrentLine, mMaxLines;

        unsigned short mLinesPerPage;

        TextBoxMode mMode;
        float mBacklogMode;
        Animation mNextPage, mNextLine;
    public:
        TextBox();
        TextBox(string text, string speaker = "");
        TextBox& operator=(MultiQuadImg image);

        SolText& GetTextRenderer();

        void ClearText();

        void SetSpeaker(string speaker);
        void SetText(string text);
        void SetSpkRightPos(Vector2f pos);
        void SetSpkLeftPos(Vector2f pos);
        void SetSpkCentPos(Vector2f pos);
        void SetSpeakerColor(Color col);
        void SetSpeakerPos(SpeakerPos pos);
        void SetSpeakerPos(Vector2f left, Vector2f right, Vector2f center);
        void InitFont(const char* fname, unsigned int h);
        void SetMaxLines(int max);
        void SetTextOffset(Vector2f offset);
        void SetLinesPerPage(unsigned short lines);
        void SetTextRenderer(SolText& text) {
            mTextRenderer = text;
            if(mTextRenderer.GetString() != "") {
                mText.push_front(text.GetText());
            }
            mTextRenderer.SetString("");
        }

        int GetMaxLines();
        unsigned int GetLinesPerPage();
        Color GetSpeakerColor();
        string GetSpeaker();
        Vector2f GetSpkRightPos();
        Vector2f GetSpkLeftPos();
        Vector2f GetSpkCentPos();
        Vector2f GetCurSpkPos();
        Vector2f GetTextOffset();
        SpeakerPos GetSpeakerPos();
        bool LineFinished();
        bool PageFinished();

        void Next();
        void Previous();
        void FinishLine();
        void AddPage(string page);
        void AddLine(string line);
        void AddLine(string line, unsigned int pos);
        void AddLines(vector<string> lines);
        void AddLines(deque<string> lines);
        void AddLines(string lines[], int amount);
        int GetCurrentLineCount();
        deque<string> GetReadLines();
        deque<string> GetUnreadLines();
        deque<string> GetText() {return mText;}
        string GetCurrentLine();
        Animation& GetNextPage();
        Animation& GetNextLine();

        void SetXY(Vector2f pos);

        void Update();
        //void Draw(Renderer& r);
    };
};

#endif // TEXTBOX_H_INCLUDED

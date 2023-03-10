#include "textbox.h"

namespace solstice {
    TextBox::TextBox():
    mSpeaker(""), mCurrentLine(0), mMaxLines(10), mLastLine(-1), mLinesPerPage(0)
    {
    }
    TextBox::TextBox(string text, string speaker):
    mSpeaker(speaker), mCurrentLine(0), mMaxLines(10), mLastLine(-1), mLinesPerPage(0)
    {
        mText.push_back(text);
        mTextRenderer.SetString(text);
    }
    TextBox& TextBox::operator=(MultiQuadImg image) {
        mQuads.clear();
        for(int i = 0; i < image.Size(); i++) {
            mQuads.push_back(image.GetQuad(i));
        }
        SetXY(image.GetOrigin());
        mLinesPerPage = 0;
        return *this;
    }

    SolText& TextBox::GetTextRenderer() {return mTextRenderer;}

    void TextBox::AddPage(string page) {
        if(mMaxLines <= mText.size()) {
            mText.pop_front();
        }
        mText.push_back(page);
    }
    void TextBox::AddLine(string line) {
        if(mMaxLines <= mText.size()) {
            mText.pop_front();
        }
        mText.push_back(line);
    }
    void TextBox::AddLine(string line, unsigned int pos) {
        if(pos >= mMaxLines) {
            return;
        }
        if(mMaxLines <= mText.size()) {
            mText.pop_front();
        }
        if(mMode == TXTBOX_NVL) {
            mText[pos] = mText[pos] + '\n' + line;
        }
        else if(mMode == TXTBOX_ADV) {
            mText[pos] = line;
        }
    }
    void TextBox::ClearText()                   {mText.clear();}

    void TextBox::SetSpeaker(string speaker)    {mSpeaker = speaker;}
    void TextBox::SetText(string text) {
        if(mText.empty()) {
            mText.push_front(text);
        }
        mCurrentLine = 0;
        mLastLine = -1;
    }
    void TextBox::SetSpkRightPos(Vector2<float> pos)    {
        mSpeakerRightPos = pos + GetOrigin();
    }
    void TextBox::SetSpkLeftPos(Vector2<float> pos)     {mSpeakerLeftPos = pos;}
    void TextBox::SetSpkCentPos(Vector2<float> pos)     {mSpeakerCenterPos = pos;}
    void TextBox::SetSpeakerColor(Color col)            {mSpeakerColor = col;}
    void TextBox::SetSpeakerPos(SpeakerPos pos)         {mSpeakerPos = pos;}
    void TextBox::SetLinesPerPage(unsigned short lines) {mLinesPerPage = lines;}
    void TextBox::SetSpeakerPos(Vector2<float> left, Vector2<float> right, Vector2<float> center) {
        SetSpkRightPos(right);
        SetSpkLeftPos(left);
        SetSpkCentPos(center);
    }
    void TextBox::InitFont(const char* fname, unsigned int h) {
        mTextRenderer.Init(fname, h, "", false);
        if(!mText.empty()) {
            mTextRenderer.SetString(mText[0]);
        }
    }
    void TextBox::SetMaxLines(int max) {
        mMaxLines = max;
        if(max < mText.size()) {
            for(int i = max; i < mMaxLines; i++) {
                mText.pop_back();
            }
        }
        mCurrentLine = 0;
        mLastLine = -1;
    }

    int TextBox::GetMaxLines()                  {return mMaxLines;}
    Color TextBox::GetSpeakerColor()            {return mSpeakerColor;}
    string TextBox::GetSpeaker()                {return mSpeaker;}
    Vector2<float> TextBox::GetSpkRightPos()    {return mSpeakerRightPos;}
    Vector2<float> TextBox::GetSpkLeftPos()     {return mSpeakerLeftPos;}
    Vector2<float> TextBox::GetSpkCentPos()     {return mSpeakerCenterPos;}
    Vector2<float> TextBox::GetCurSpkPos()      {return mCurSpeakerPos;}
    SpeakerPos TextBox::GetSpeakerPos()         {return mSpeakerPos;}
    Animation& TextBox::GetNextPage()           {return mNextPage;}
    Animation& TextBox::GetNextLine()           {return mNextLine;}
    bool TextBox::LineFinished()                {return mTextRenderer.Done();}
    bool TextBox::PageFinished()                {return mTextRenderer.Done();}
    unsigned int TextBox::GetLinesPerPage()     {return mLinesPerPage;}
    void TextBox::Update() {
        if(mCurrentLine != mLastLine) {
            mTextRenderer.SetString(GetCurrentLine());
            mLastLine = mCurrentLine;
        }

        if(mTextRenderer.Done()) {
            if(mNextPage.GetHidden()) {
                mNextPage.SetHidden(false);
                mNextPage.Play();
            }
        }
        else {
            mNextPage.SetHidden(true);
            mNextLine.SetHidden(true);
            mNextPage.Reset();
        }
        if(mTextRenderer.Done()) {
            mNextPage.Update();
        }
        mTextRenderer.Update();
        switch(mSpeakerPos) {
            case SPK_RIGHT:
                mCurSpeakerPos = mSpeakerRightPos;
                break;
            case SPK_LEFT:
                mCurSpeakerPos = mSpeakerLeftPos;
                break;
            case SPK_CENTER:
                mCurSpeakerPos = mSpeakerCenterPos;
                break;
            default:
                mCurSpeakerPos = mSpeakerLeftPos;
                break;
        }
    }
    void TextBox::FinishLine() {
        if(mTextRenderer.Done()) {
            Next();
        }
        else if(mTextRenderer.Paused()) {
            mTextRenderer.Unpause();
        }
        else {
            mTextRenderer.FinishLine();
        }
    }
    void TextBox::Next() {
        if(mCurrentLine < mText.size() - 1) {
            mCurrentLine++;
            mTextRenderer.Unpause();
        }
    }
    void TextBox::Previous() {
        if(mCurrentLine > 0) {
            mCurrentLine--;
            mTextRenderer.Unpause();
        }
    }
    void TextBox::AddLines(vector<string> lines) {
        for(int i = 0; i < lines.size(); i++) {
            if(mMaxLines > mText.size()) {
                mText.push_back(lines[i]);
            }
            else {
                return;
            }
        }
    }
    void TextBox::AddLines(deque<string> lines) {
        for(int i = 0; i < lines.size(); i++) {
            if(mMaxLines > mText.size()) {
                mText.push_back(lines[i]);
            }
            else {
                return;
            }
        }
    }
    void TextBox::AddLines(string lines[], int amount) {
        for(int i = 0; i < amount; i++) {
            if(mMaxLines > mText.size()) {
                mText.push_back(lines[i]);
            }
            else {
                return;
            }
        }
    }
    void TextBox::SetTextOffset(Vector2<float> offset) {
        mTextOffset = offset;
    }
    int TextBox::GetCurrentLineCount()   {
        return mCurrentLine;
    }
    Vector2<float> TextBox::GetTextOffset() {
        return mTextOffset;
    }
    string TextBox::GetCurrentLine() {
        return mText[mCurrentLine];
    }
    deque<string> TextBox::GetReadLines() {
        deque<string> ret;
        for(int i = 0; i < mCurrentLine - 1; i++) {
            ret.push_back(mText[i]);
        }
        return ret;
    }
    deque<string> TextBox::GetUnreadLines() {
        deque<string> ret;
        for(int i = mCurrentLine + 1; i < mText.size(); i++) {
            ret.push_back(mText[i]);
        }
        return ret;
    }
    void TextBox::SetXY(Vector2<float> pos) {
        MultiQuadImg::SetXY(pos);
        mTextRenderer.SetPosition(mTextOffset);
    }

    /*
    void TextBox::Draw(Renderer& r) {
        r.DrawMultiQuadImg(*this);
        r.DrawQuad(mNextPage.GetDrawQuad());
        if(!mNextLine.GetHidden())
        r.DrawQuad(mNextLine.GetDrawQuad());
        mTextRenderer.Draw();
        if(mTextRenderer.GetShadow()) {
            mTextRenderer.Print(Color(0, 0, 0, mTextRenderer.GetShadowAlpha()),
                                mCurSpeakerPos.x + mOrigin.x + 2,
                                mCurSpeakerPos.y + mOrigin.y -2,
                                mSpeaker.c_str());
        }
        mTextRenderer.Print(mSpeakerColor, mCurSpeakerPos.x + mOrigin.x, mCurSpeakerPos.y + mOrigin.y, mSpeaker.c_str());
    }
    */
};

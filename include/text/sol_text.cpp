#include "sol_text.h"

namespace solstice {
    SolText::SolText():
    mOrigin(Vector2<float>(0,0)), mBaseColor(COL_WHITE),
    mLineBreak(300), mLettersStepped(0), mFrameSkip(0), mFrameTimer(0), mCharsPerFrame(1),
    mCurXPos(0), mPaused(false), mDelayTimer(0), mPeriodPause(false), mNewlinePause(false),
    mElipsisDel(0), mCommaDel(0), mColonDel(0), mSemiColDel(0),
    mHasShadow(false), mWarps(false), mFades(false), mScales(false), mElipsisType(0),
    mFrequency(0), mAmplitude(0), mPhase(0), mAngleStep(0),
    mFadeMin(1), mFadeMax(1), mFadeStep(1), mShadowAlpha(0.75f),
    mScaleStart(0), mScaleEnd(0), mScaleStep(0),
    mAlignment(ALIGN_LEFT)
    {
    }

    SolText::SolText(const char* font, unsigned int h, bool stroked, int strokeSize):
    mOrigin(0), mBaseColor(COL_WHITE),
    mLineBreak(300), mLettersStepped(0), mFrameSkip(0), mFrameTimer(0), mCharsPerFrame(1),
    mCurXPos(0), mPaused(false), mDelayTimer(0), mPeriodPause(false), mNewlinePause(false),
    mElipsisDel(0), mCommaDel(0), mColonDel(0), mSemiColDel(0),
    mHasShadow(false), mWarps(false), mFades(false), mScales(false), mElipsisType(0),
    mFrequency(0), mAmplitude(0), mPhase(0), mAngleStep(0),
    mFadeMin(1), mFadeMax(1), mFadeStep(1), mShadowAlpha(0.75f),
    mScaleStart(0), mScaleEnd(0), mScaleStep(0),
    mAlignment(ALIGN_LEFT)
    {
        FontData::Init(font, h, stroked, strokeSize);
    }
    SolText::SolText(const char* font, unsigned int pointSize, string text, Vector2<float> origin, bool stroked, int strokeSize):
    mBaseColor(COL_WHITE),
    mLineBreak(300), mLettersStepped(0), mFrameSkip(0), mFrameTimer(0), mCharsPerFrame(1),
    mCurXPos(0), mPaused(false), mDelayTimer(0), mPeriodPause(false), mNewlinePause(false),
    mElipsisDel(0), mCommaDel(0), mColonDel(0), mSemiColDel(0),
    mHasShadow(false), mWarps(false), mFades(false), mScales(false), mElipsisType(0),
    mFrequency(0), mAmplitude(0), mPhase(0), mAngleStep(0),
    mFadeMin(1), mFadeMax(1), mFadeStep(1), mShadowAlpha(0.75f),
    mScaleStart(0), mScaleEnd(0), mScaleStep(0),
    mAlignment(ALIGN_LEFT)
    {
        mText = text;
        FontData::Init(font, pointSize, stroked, strokeSize);
        mOrigin = origin;
    }
    Letter& SolText::operator[](unsigned int i){
        if(i < mText.size())
            return mLetters[i];
        else {
            return mLetters[0];
        }
    }
    SolText& SolText::operator=(const SolText& text) {
        Init(text.mFilename.c_str(), text.mPointSize, text.mText, text.mStroked, text.mStrokeSize);
        SetTextAlignment(text.mAlignment);
        SetShadow(text.mHasShadow);
        SetColor(text.mBaseColor);
        mPaused = text.mPaused;
        mPeriodPause = text.mPeriodPause;
        SetPosition(text.mOrigin);
        SetDelays(text.mElipsisDel, text.mCommaDel, text.mColonDel, text.mSemiColDel);
        mDelayTimer = 0;
        mElipsisType = text.mElipsisType;
        SetLineBreak(text.mLineBreak);
        SetFrameSkip(text.mFrameSkip);
        mFades = text.mFades;
        mWarps = text.mWarps;
        mScales = text.mScales;
        if(text.mWarps) {
            Warp(text.mFrequency, text.mAmplitude, text.mPhase, text.mAngleStep);
        }
        if(text.mFades) {
            Fade(text.mFadeMin, text.mFadeMax, text.mFadeStep);
        }
        if(text.mScales) {
            Scale(text.mScaleStart, text.mScaleEnd, text.mScaleStep);
        }
        return *this;
    }
    void SolText::Clean() {
        mLetters.clear();
        mText.clear();
        //mRenderer.Clear();
    }
    void SolText::UpdateCharPositions(Vector2f pos) {
        Vector2f diff = pos - mOrigin;
        mOrigin = pos;
        for(int i = 0; i < mLetters.size(); i++) {
            mLetters[i].quad.AddXY(diff.x, diff.y);
        }
    }
    void SolText::GenCharPositions() {
        if(mText.empty())
            return;
        mLettersStepped = 0;
        mLetters.clear();
        //mRenderer.ClearQuadPtrs();
        mLettersToStep = 0;
        mCurXPos = 0;
        int lb = 0;
        int steppedWidth = 0;
        int spaceLeft = 0;
        int lastChecked = 0;
        for(unsigned int i = 0; i < mText.size(); i++) {
            int x, y;
            if(mText[i] == '\n') {
                AlignSubString(lastChecked, i, steppedWidth, spaceLeft);
                lb-= mLineBreakHeight;
                mCurXPos = 0;
                steppedWidth = 0;
                lastChecked = i + 1;
            }
            else if(mText[i] == ' ') {
                spaceLeft = mLineBreak - steppedWidth;
                string substr = mText.substr(i + 1, string::npos);
                int wordSize = CheckWordLength(substr.substr(0, substr.find(' ')));

                if(wordSize > spaceLeft) {
                    AlignSubString(lastChecked, i, steppedWidth, spaceLeft);
                    lb-= mLineBreakHeight;
                    mCurXPos = 0;
                    steppedWidth = 0;
                    lastChecked = i + 1;
                }
            }

            char& ch = mText[i];
            SolChar& currChar =  mCharSet[ch];
            x = mCurXPos;
            y = lb - currChar.yOff;

            mLetters.push_back(Letter(mText[i], mBaseColor, x, y));
            mLetters[i].quad = currChar.GetDrawingQuad();
            mLetters[i].quad.SetTextureID(ch);
            mLetters[i].quad.SetXY(Vector2f(x, y));
            mLetters[i].y = y;
            mLetters[i].quad.SetColor(mBaseColor);
            if(mFades)
                mLetters[i].SetFade(mFadeMin, mFadeMax, mFadeStep);
            if(mScales)
                mLetters[i].SetScale(mScaleStart, mScaleEnd, mScaleStep);
            if(mWarps)
                mLetters[i].SetWarp(mFrequency, mAmplitude, mPhase, mAngleStep);
            mLetters[i].quad.Init(Vector2<float>(x, y), currChar.GetDrawingQuad().GetHeight(), currChar.GetDrawingQuad().GetWidth());
            mLetters[i].Reset();

            if(mText[i] != '\n') {
                if(mText[i] > 32 || (mText[i] == ' ' && mCurXPos > 0)) {
                    mCurXPos+= currChar.advance;
                    steppedWidth += currChar.advance;
                }
            }
            if(mText[i] < 31 || mText[i] == '\n') {
                mLetters[i].quad.SetHidden(true);
            }
        }
        AlignSubString(lastChecked, mLetters.size(), steppedWidth);

        for(unsigned int i = 0; i < mLetters.size(); i++) {
            //mRenderer.AddQuadPtr(&mLetters[i].quad);
        }

    }
    void SolText::Init(const char* filename, int h, string text, bool stroked, int strokeSize) {
        mText = text;
        FontData::Init(filename, h, stroked, strokeSize);
        GenCharPositions();
    }

    void SolText::SetString(string text) {
        mText = text;
        GenCharPositions();
    }
    void SolText::SetX(int x) {mOrigin.x = x;}
    void SolText::SetY(int y) {mOrigin.y = y;}
    void SolText::SetXY(int x, int y) {mOrigin.x = x; mOrigin.y = y;}
    void SolText::SetPosition(Vector2f pos){
        //GenCharPositions(pos);
        //UpdateCharPositions(pos);
        mOrigin = pos;
    }
    void SolText::SetColor(Color col) {
        mBaseColor = col;
        for(unsigned int i = 0; i < mLetters.size(); i++) {
            mLetters[i].quad.SetColor(col);
        }
    }
    void    SolText::SetLineBreakH(short lb)        {mLineBreakHeight = lb; GenCharPositions();}
    void    SolText::SetLineBreak(short max)        {mLineBreak = max; GenCharPositions();}
    void    SolText::SetFrameSkip(short skip)       {mFrameSkip  = skip; mFrameTimer = 0;}
    void    SolText::SetFrequency(float freq)       {mFrequency = freq;}
    void    SolText::SetAmplitude(float amp)        {mAmplitude = amp;}
    void    SolText::SetPhase(float phase)          {mPhase = phase;}
    void    SolText::SetAngleStep(float step)       {mAngleStep = step;}
    void    SolText::SetFadeMin(float fadeMin)      {mFadeMin = fadeMin;}
    void    SolText::SetFadeMax(float fadeMax)      {mFadeMax = fadeMax;}
    void    SolText::SetFadeStep(float step)        {mFadeStep = step;}
    void    SolText::SetShadowAlpha(float a)        {mShadowAlpha = a;}
    void    SolText::SetScaleStart(float start)     {mScaleStart = start;}
    void    SolText::SetScaleEnd(float end)         {mScaleEnd = end;}
    void    SolText::SetScaleStep(float step)       {mScaleStep = step;}
    void    SolText::SetCharsPerFrame(int c)        {mCharsPerFrame = c;}
    void    SolText::SetNewlinePause(bool nl)       {mNewlinePause = nl;}
    void SolText::SetTextAlignment(TextAlignment alignment) {
        mAlignment = alignment;
        GenCharPositions();
    }
    void SolText::Fade(float min, float max, float step) {
        mFadeMin = min;
        mFadeMax = max;
        mFadeStep = step;
        mFades = true;
        if(!mLetters.empty()) {
            for(unsigned int i = 0; i < mLetters.size(); i++) {
                mLetters[i].SetFade(min, max, step);
            }
        }
    }
    void SolText::Fade() {
        if(mFades)
            Fade(mFadeMin, mFadeMax, mFadeStep);
    }
    void SolText::Scale(float start, float end, float step) {
        mScaleStart = start;
        mScaleEnd = end;
        mScaleStep = step;
        mScales = true;
        if(!mLetters.empty()) {
            for(unsigned int i = 0; i < mLetters.size(); i++) {
                mLetters[i].SetScale(start, end, step);
            }
        }
    }
    void SolText::Warp(float freq, float amp, float phase, float step) {
        mFrequency = freq;
        mAmplitude = amp;
        mPhase = phase;
        mAngleStep = step;
        mWarps = true;
        for(unsigned int i = 0; i < mLetters.size(); i++) {
            mLetters[i].SetWarp(freq, amp, phase, step);
        }
    }
    void SolText::SetDelays(short elipsis, short comma, short colon, short semicolon) {
        mElipsisDel = elipsis;
        mCommaDel = comma;
        mColonDel = colon;
        mSemiColDel = semicolon;
    }

    void SolText::FadeRange(float start, float end, float step, unsigned int first, unsigned int last) {
        if(first < mLetters.size()) {
            for(unsigned int i = first; i < last && i < mLetters.size(); i++) {
                mLetters[i].SetFade(start, end, step);
            }
        }
    }
    void SolText::ScaleRange(float start, float end, float step, unsigned int first, unsigned int last) {
        if(first < mLetters.size()) {
            for(unsigned int i = first; i < last && i < mLetters.size(); i++) {
                mLetters[i].SetScale(start, end, step);
            }
        }
    }
    void SolText::WarpRange(float freq, float amp, float phase, float step, unsigned int first, unsigned int last) {
        mWarps = true;
        if(first < mLetters.size()) {
            for(unsigned int i = first; i < last && i < mLetters.size(); i++) {
                mLetters[i].SetWarp(freq, amp, phase, step);
            }
        }
    }
    void SolText::ColorRange(Color col, unsigned int first, unsigned int last) {
        if(first < mLetters.size()) {
            for(unsigned int i = first; i < last && i < mLetters.size(); i++) {
                mLetters[i].quad.SetColor(col);
            }
        }
    }
    string SolText::GetText() {return mText;}
    Vector2<float> SolText::GetOrigin() {return mOrigin;}
    Color SolText::GetColor() {return mBaseColor;}
    short SolText::GetLineBreak() {return mLineBreak;}
    short SolText::GetFrameSkip() {return mFrameSkip;}
    short SolText::GetFrameTimer() {return mFrameTimer;}
    short SolText::GetCharsPerFrame()   {return mCharsPerFrame;}
    short SolText::GetLetterStepped() {return mLettersStepped;}
    float SolText::GetFrequency() {return mFrequency;}
    float SolText::GetAmplitude() {return mAmplitude;}
    float SolText::GetPhase() {return mPhase;}
    float SolText::GetAngleStep() {return mAngleStep;}
    float SolText::GetFadeMin() {return mFadeMin;}
    float SolText::GetFadeMax() {return mFadeMax;}
    float SolText::GetFadeStep() {return mFadeStep;}
    float SolText::GetScaleStart() {return mScaleStart;}
    float SolText::GetScaleEnd() {return mScaleEnd;}
    float SolText::GetScaleStep() {return mScaleStep;}
    float SolText::GetShadowAlpha() {return mShadowAlpha;}
    TextAlignment SolText::GetTextAlignment() {return mAlignment;}

    bool    SolText::Paused()                           {return mPaused;}
    bool    SolText::Delayed()                          {return mDelayTimer;}
    bool    SolText::Done()                             {return mLettersStepped == mText.size();}
    short   SolText::GetElipsisDelay()                  {return mElipsisDel;}
    short   SolText::GetCommaDelay()                    {return mCommaDel;}
    short   SolText::GetColonDelay()                    {return mColonDel;}
    short   SolText::GetSemiColDelay()                  {return mSemiColDel;}
    bool    SolText::PeriodPause()                      {return mPeriodPause;}
    bool    SolText::GetShadow()                        {return mHasShadow;}
    bool    SolText::Fades()                            {return mFades;}
    bool    SolText::Scales()                           {return mScales;}
    bool    SolText::Warps()                            {return mWarps;}
    bool    SolText::GetNewlinePause()                  {return mNewlinePause;}
    void    SolText::SetElipsisDelay(short timer)       {mElipsisDel = timer;}
    void    SolText::SetCommaDelay(short timer)         {mCommaDel = timer;}
    void    SolText::SetColonDelay(short timer)         {mColonDel = timer;}
    void    SolText::SetSemiColDelay(short timer)       {mSemiColDel = timer;}
    void    SolText::PeriodPause(bool flag)             {mPeriodPause = flag;}
    void    SolText::SetShadow(bool flag)               {mHasShadow = flag;}
    void    SolText::Fades(bool flag)                   {mFades = flag;}
    void    SolText::Scales(bool flag)                  {mScales = flag;}
    void    SolText::Warps(bool flag)                   {mWarps = flag;}
    void SolText::AlignSubString(unsigned int start, unsigned int end, int stepped, int spaceLeft) {
        if(start > mLetters.size() || end > mLetters.size()) {
            return;
        }
        if(mAlignment == ALIGN_RIGHT) {
            for(int j = start; j < end; j++) {
                mLetters[j].quad.AddX(-stepped);
            }
        }
        else if(mAlignment == ALIGN_CENTER) {
            for(int j = start; j < end; j++) {
                mLetters[j].quad.AddX(-stepped/ 2);
            }
        }
        else if(mAlignment == ALIGN_JUSTIFY) {
            if(!spaceLeft) {
                return;
            }
            int spaceAmt = 0;
            for(int j = start; j < end; j++) {
                if(mText[j] == ' ') {
                    spaceAmt++;
                }
            }

            int shiftAmt = (spaceLeft / spaceAmt) + mCharSet[' '].advance;
            for(int j = start; j < end; j++) {
                if(mText[j] == ' ') {
                    ShiftCharacters(j, end, shiftAmt);
                }
            }
        }
    }
    unsigned int SolText::CheckStringWidth(const string& s) {
         unsigned int lineWidth = 0;
         for(unsigned int i = 0; i < s.size(); i++) {
            if(s[i] > ' ' && s[i] != '\n') {
                lineWidth+= mCharSet[i].width;
            }
            else if(s[i] == '\n') {
                return lineWidth;
            }
         }
         return lineWidth;
    }
    void SolText::ShiftCharacters(unsigned int start, unsigned int end, int amount) {
        if(start > mLetters.size() || end > mLetters.size()) {
            return;
        }
        for(unsigned int i = start; i < end; i++) {
            mLetters[i].quad.AddX(amount);
        }
    }
    unsigned int SolText::CheckWordLength(const string& s) {
        unsigned int wordWidth = 0;
        for(int i = 0; i < s.size(); i++) {
            if(s[i] == '\n' || s[i] == '\r' || s[i] == ' ') {
                return wordWidth;
            }
            else {
                wordWidth+= mCharSet[i].width;
            }
        }
        return wordWidth;
    }

    unsigned int SolText::GetLineLength(const string& s) {
        unsigned int spaceLeft = 0;
        unsigned int steppedWidth = 0;
        for(int i = 0; i < s.size(); i++) {
            if(s[i] == '\n') {
                steppedWidth = 0;
            }
            else if(s[i] == ' ') {
                spaceLeft = mLineBreak - steppedWidth;
                string substr = s.substr(i + 1, string::npos);
                int wordSize = CheckWordLength(substr.substr(0, substr.find(' ')));
                if(wordSize > spaceLeft) {
                    steppedWidth = 0;
                }
            }
            steppedWidth+= mCharSet[i].advance;
        }
        return steppedWidth;
    }

    void SolText::Update() {
        if(!mPaused) {
            if(mFrameSkip <= mFrameTimer)
                mFrameTimer = 0;
            else {
                if(mDelayTimer > 0)
                    mDelayTimer--;
                mFrameTimer++;
            }
            if(!mDelayTimer) {
                if(!mFrameTimer) {
                    if(mElipsisType > 0) {
                        if(mLettersStepped < mLetters.size()) {
                            for(int i = 0; i < mCharsPerFrame && i < mLetters.size(); i++) {
                                if(mLettersStepped > mLetters.size()) {
                                    mLettersStepped = mLetters.size() - 1;
                                }
                            }
                            mElipsisType--;
                        }
                    }
                    else if(mLettersStepped < mLetters.size()) {
                        for(int i = 0; i < mCharsPerFrame && i < mLetters.size(); i++) {
                            mLettersStepped++;
                            if(mLettersStepped > mLetters.size()) {
                                    mLettersStepped = mLetters.size() - 1;
                            }
                            for(int i = mLettersStepped - mCharsPerFrame; i <= mLettersStepped; i++) {
                                if(!mLetters[mLettersStepped].read)
                                    CheckPunctuation(mLettersStepped);
                            }
                        }
                    }
                }
            }
        }
        if(!mLetters.empty()) {
            for(unsigned int i = 0; i < mLettersStepped; i++)
                mLetters[i].Update();
        }
        if(mNewlinePause && mLettersStepped < mLetters.size() && mText[mLettersStepped] == '\n') {
            Pause();
            mLettersStepped++;
        }
    }
    void SolText::CheckPunctuation(unsigned int i) {
        Letter& foo = mLetters[i];

        if(foo.letter  != '!' || foo.letter  != '.' ||
           foo.letter  != '?' || foo.letter  != ':' ||
           foo.letter  != ';' || foo.letter  != ',')
        {
            foo.read = true;
            return;
        }
        if( (i + 2) < mLetters.size() ) {
            Letter& foo2 = mLetters[i+1];
            Letter& foo3 = mLetters[i+2];

            if(foo3.letter != '\0') {
                if(foo2.letter == '.' && foo3.letter == '.' && mElipsisDel) {
                    mDelayTimer = mElipsisDel;
                    foo.read = foo2.read = foo3.read = true;
                    mLettersStepped++;
                    mElipsisType = 3;
                    return;
                }
                else if(!mElipsisDel && foo2.letter == '.' && foo3.letter == '.' ) {
                    foo.read = foo2.read = foo3.read = true;
                    mLettersStepped+=4;
                    mElipsisType = 3;
                    mDelayTimer = -1;
                    return;
                }
                else if(foo.letter == '.' && mPeriodPause) {
                    mLettersStepped++;
                    mDelayTimer = -1;
                    foo.read = true;
                    return;
                }
            }
        }

        if(foo.letter == ',' && mCommaDel) {
            mDelayTimer = mCommaDel;
            mLettersStepped++;
        }
        else if(foo.letter == ':' && mColonDel) {
            mDelayTimer = mColonDel;
            mLettersStepped++;
        }
        else if(foo.letter == ';' && mSemiColDel) {
            mDelayTimer = mSemiColDel;
            mLettersStepped++;
        }
        else if(foo.letter == '?' && mPeriodPause) {
            mLettersStepped++;
            mDelayTimer = -1;
        }
        else if(foo.letter == '!' && mPeriodPause) {
            mLettersStepped++;
            mDelayTimer = -1;
        }
        foo.read = true;
    }
    void SolText::Reset() {
        mPaused = false;
        if(!mLetters.empty())
        for(unsigned int i = 0; i < mLetters.size(); i++)
            mLetters[i].Reset();
    }
    void SolText::SetupVBO() {
        //mVBO.Clear();
        delete[] indices;
        INDICES = mText.size() * 6;
        GLuint VERTICES = mText.size() * 8;
        GLfloat vertices[VERTICES];
        GLfloat texcoords[VERTICES];
        indices = new GLuint[INDICES];
        int i = 0;
        for(auto& let : mLetters) {
            texcoords[i] = 0.0f;
            vertices[i++] = let.quad.GetLeft() + mOrigin.x;
            texcoords[i] = 0.0f;
            vertices[i++] = let.quad.GetBottom() + mOrigin.y;
            texcoords[i] = 1.0f;
            vertices[i++] = let.quad.GetRight() + mOrigin.x;
            texcoords[i] = 0.0f;
            vertices[i++] = let.quad.GetBottom() + mOrigin.y;
            texcoords[i] = 1.0f;
            vertices[i++] = let.quad.GetRight() + mOrigin.x;
            texcoords[i] = 1.0f;
            vertices[i++] = let.quad.GetTop() + mOrigin.y;
            texcoords[i] = 1.0f;
            vertices[i++] = let.quad.GetRight() + mOrigin.x;
            texcoords[i] = 0.0f;
            vertices[i++] = let.quad.GetBottom() + mOrigin.y;
        }
        int j = 0, k = 0;
        cout << mLetters.size() << endl;
        for(int i = 0; i < mLetters.size(); i++) {
            indices[k++] = j;
            indices[k++] = j + 1;
            indices[k++] = j + 2;
            indices[k++] = j;
            indices[k++] = j + 3;
            indices[k++] = j + 2;
            j+=6;
        }
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        sizeof(vertices),
                        vertices);
        glBufferSubData(GL_ARRAY_BUFFER,
                        sizeof(vertices),
                        sizeof(texcoords),
                        texcoords);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertices));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void SolText::Draw2() {
    }
    void SolText::Draw2(ShaderProgram& shader) {
        glUniform4f(shader.GetUniformLocation("color"), 1.0f, 0.0f, 0.0f, 1.0f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, INDICES, GL_UNSIGNED_INT, &indices);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void SolText::FinishLine() {
        int letters = mText.substr(mLettersStepped, mText.size()).find('\n');
        mLettersStepped = letters != -1 ? mLettersStepped + letters : mLetters.size();
        for(int i = 0; i < mLettersStepped; i++) {
            mLetters[i].Complete();
        }
        Unpause();
    }
    void SolText::FinishText() {
        mLettersStepped = mLetters.size();
        for(int i = 0; i < mLetters.size(); i++) {
            mLetters[i].Complete();
        }
    }
    void SolText::Pause() {mPaused = true;}
    void SolText::Stop() {Reset();}
    void SolText::Unpause() {mPaused = false;}
    void SolText::Next() {
        mDelayTimer = 0;
        mElipsisType = 0;
    }
};

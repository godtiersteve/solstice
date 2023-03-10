#ifndef GUIGAUGE_H_INCLUDED
#define GUIGAUGE_H_INCLUDED

#include "guicontrol.h"

namespace solstice {
class GUIGauge : public GUIControl {
private:
    int mValue, mMaxValue, mMinValue, mTargetValue;
    int mMaxWidth, mCurrentWidth, mTargetWidth, mHeight;
    int mDrainStep;
    bool mVertical;
public:
    GUIGauge(int max = 100, int min = 0);
    void Init(int max, int min, int maxWidth);
    void InitDimensions(int maxWidth, int height, bool vertical = false);
    void Update();
    void Draw(SceneRenderer& rend);

    Color GetColor(Color c)             {return mQuads.front().GetColor();}
    Color GetDrainColor(Color c)        {return mQuads[1].GetColor();}
    int GetMaxValue()                   {return mMaxValue;}
    int GetMinValue()                   {return mMinValue;}
    int GetMaxWidth()                   {return mMaxWidth;}
    int GetCurrentWidth()               {return mCurrentWidth;}
    int GetTargetWidth()                {return mTargetWidth;}
    int GetDrainStep()                  {return mDrainStep;}
    bool GetIsVertical()                {return mVertical;}
    int GetHeight()                     {return mHeight;}
    int GetTargetValue()                {return mTargetValue;}
    int GetValue()                      {return mValue;}

    void SetColor(Color c)              {mQuads.front().SetColor(c);}
    void SetDrainColor(Color c)         {mQuads[1].SetColor(c);}
    void SetMaxValue(int m)             {mMaxValue = m;}
    void SetMinValue(int m)             {mMinValue = m;}
    void SetMaxWidth(int m)             {mMaxWidth = m;}
    void SetDrainStep(int d)            {mDrainStep = d;}
    void SetValue(int val)              {mValue = val;}
    void SetIsVertical(bool v)          {mVertical = v;}
    void SetHeight(int h)               {mHeight = h;}
    void SetTargetValue(int t)          {mTargetValue = t;}

    void SetToMax()                     {mValue = mMaxValue;}
    void SetToMin()                     {mValue = mMinValue;}
    void SubFromGauge(int val)          {mValue-= val;}
    void AddToGauge(int val)            {mValue+= val;}
    void SetColors(Color base, Color drain);

};

}

#endif // GUIGAUGE_H_INCLUDED

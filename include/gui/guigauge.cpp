#include "guigauge.h"

namespace solstice {
GUIGauge::GUIGauge(int max, int min) {
    Init(max, min, max);
    mQuads.push_back(QuadData());
    mQuads.push_back(QuadData());
    mDrainStep = 0;
    mVertical = false;
}
void GUIGauge::Init(int max, int min, int maxWidth) {
    mMaxValue = max;
    mMinValue = min;
    mMaxWidth = maxWidth;
    mValue = max;
    mTargetValue = max;
}
void GUIGauge::InitDimensions(int maxWidth, int height, bool vertical) {
    mVertical = vertical;
    if(mVertical) {
        int temp = height;
        height = maxWidth;
        maxWidth = height;
    }
    mQuads[0].Init(mPosition, height, maxWidth);
    mQuads[1].Init(mPosition, height, maxWidth);
    mHeight = height;
    mMaxWidth = maxWidth;
    mCurrentWidth = mMaxWidth;
}
void GUIGauge::Update() {
    mValue = std::max(mValue, mMinValue);
    mValue = std::min(mValue, mMaxValue);
    float range = mMaxValue - mMinValue;
    if(mDrainStep) {
        if(mTargetValue < mValue) {
            mTargetValue+= std::min(mDrainStep,  mValue - mTargetValue);
        }
        else {
            mTargetValue-= std::min(mDrainStep, mTargetValue - mValue);
        }
        mTargetWidth = mMaxWidth * mTargetValue / range;
        if(mVertical) {
            mQuads[1].SetHeight(mTargetWidth);
            if(mQuads[1].GetWidth() != mHeight) {
                mQuads[1].SetWidth(mHeight);
            }
        }
        else {
            mQuads[1].SetWidth(mTargetWidth);
            if(mQuads[1].GetHeight() != mHeight) {
                mQuads[1].SetHeight(mHeight);
            }
        }
    }
    int size = mMaxWidth * mValue / range;
    if(mVertical) {
        mQuads[0].SetHeight(size);
        if(mQuads[0].GetWidth() != mHeight) {
            mQuads[0].SetWidth(mHeight);
        }
    }
    else {
        mQuads[0].SetWidth(size);
        if(mQuads[0].GetHeight() != mHeight) {
            mQuads[0].SetHeight(mHeight);
        }
    }
}
void GUIGauge::Draw(SceneRenderer& rend) {
    if(mDrainStep && mValue != mTargetValue) {
        rend.DrawQuad(mQuads[1]);
    }
    rend.DrawQuad(mQuads[0]);
}
void GUIGauge::SetColors(Color base, Color drain) {
    SetColor(base);
    SetDrainColor(drain);
}
}

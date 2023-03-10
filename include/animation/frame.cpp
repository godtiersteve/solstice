#include "frame.h"
namespace solstice {
	Frame::Frame()
	{
		mFinished = false;
		mHitBoxColor = COL_RED;
		mHitBoxColor.alpha = 0.75f;
		mHurtBoxColor = COL_BLUE;
		mHurtBoxColor.alpha = 0.75f;
	}
	Frame::Frame(FrameData data)
	{
		mFinished = false;
		mHitBoxColor = COL_RED;
		mHitBoxColor.alpha = 0.75f;
		mHurtBoxColor = COL_BLUE;
		mHurtBoxColor.alpha = 0.75f;
		*this = data;
	}
	Frame::Frame(TextureData data, short duration)
	{
		mFinished = false;
		*this = data;
		mHitBoxColor = COL_RED;
		mHitBoxColor.alpha = 0.75f;
		mHurtBoxColor = COL_BLUE;
		mHurtBoxColor.alpha = 0.75f;
		mDuration = duration;
	}
	Frame::Frame(Vector2<float> pos, TextureData data, short duration)
	{
		mFinished = false;
		mHitBoxColor = COL_RED;
		mHitBoxColor.alpha = 0.75f;
		mHurtBoxColor = COL_BLUE;
		mHurtBoxColor.alpha = 0.75f;
	}
	Frame::Frame(const Frame& data) {
	    mDuration = data.mDuration;
	    mHurtBoxColor = data.mHurtBoxColor;
	    mHitBoxColor = data.mHitBoxColor;
	    mFinished = false;
	    mElapsed = 0;
        Init(data.mTopLeft,
             data.mTopRight,
             data.mBotRight,
             data.mBotLeft);
        mTexRef = data.mTexRef;
		mTextureID = data.mTextureID;
        mDuration = data.mDuration;
        mFinished = data.mFinished;
        mScale = data.mScale;
        mAngle = data.mAngle;
        mDrawOffset = data.mDrawOffset;
        mColor = data.mColor;
    }
// TODO (Stephen#1#): Ensure = operator works for frames
	Frame& Frame::operator=(const Frame& data) {
        mHitBoxes.clear();
	    mHurtBoxes.clear();
	    if(!data.mHitBoxes.empty()) {
            for(size_t i = 0; i < data.mHitBoxes.size(); i++) {
                mHitBoxes.push_back(data.mHitBoxes[i]);
            }
	    }
	    if(!data.mHurtBoxes.empty()) {
            for(size_t i = 0; i < data.mHurtBoxes.size(); i++) {
                mHurtBoxes.push_back(data.mHurtBoxes[i]);
            }
	    }
	    mDuration = data.mDuration;
	    mHurtBoxColor = data.mHurtBoxColor;
	    mHitBoxColor = data.mHitBoxColor;
	    mFinished = false;
	    mElapsed = 0;
        Init(data.mTopLeft,
             data.mTopRight,
             data.mBotRight,
             data.mBotLeft);
        mTexRef = data.mTexRef;
		mTextureID = data.mTextureID;
        mDuration = data.mDuration;
        mFinished = data.mFinished;
        mScale = data.mScale;
        mAngle = data.mAngle;
        mDrawOffset = data.mDrawOffset;
        mColor = data.mColor;
	    return *this;
	}
	Frame& Frame::operator=(TextureData& data) {
		Init(data.GetTopLeft(),
             data.GetTopRight(),
             data.GetBotRight(),
             data.GetBotLeft());
		return *this;
	}

	Frame& Frame::operator=(FrameData& data) {
		Init(data.GetTopLeft(),
             data.GetTopRight(),
             data.GetBotRight(),
             data.GetBotLeft());
		mTexRef = data.GetTextureRef();
		mTextureID = data.GetTextureID();
        mDuration = data.GetDuration();
        mFinished = data.GetFinished();
        mScale = data.GetScale();
        mAngle = data.GetAngle();
        mDrawOffset = data.GetDrawOffset();
		return *this;
	}
	void Frame::LoadImage(TextureArray& arr, string texture, short duration) {
        Init(0, arr[texture].GetHeight(), arr[texture].GetWidth());
        mTexRef = texture;
        mDuration = duration;
	}

	PlayerHitBox& Frame::GetHitBox(unsigned short i) {
		if (i < mHitBoxes.size())
			return mHitBoxes[i];
		else
			return mHitBoxes[0];
	}
	PlayerHitBox&    Frame::GetHurtBox(unsigned short i) {
		if (i < mHurtBoxes.size())
			return mHurtBoxes[i];
		else
			return mHurtBoxes[0];
	}
	Vector2<float>& Frame::GetActionPoint(unsigned short i) {
		if (i < mActionPoints.size())
			return mActionPoints[i];
		else
			return mActionPoints[0];
	}

	void Frame::AddHitBox(PlayerHitBox hbox) { mHitBoxes.push_back(hbox); }
	void Frame::AddHurtBox(PlayerHitBox hbox) { mHurtBoxes.push_back(hbox); }
	void Frame::AddActionPoint(Vector2<float> ap) { mActionPoints.push_back(ap); }
	void Frame::AddHitBox() {
		mHitBoxes.push_back(PlayerHitBox());
		mHitBoxes.back().SetColor(mHitBoxColor);
	}
	void Frame::AddHurtBox() {
		mHurtBoxes.push_back(PlayerHitBox());
		mHurtBoxes.back().SetColor(mHurtBoxColor);
	}

	void Frame::AddActionPoint() {
        mActionPoints.push_back(0);
	}
	PlayerHitBox& Frame::GetLastHitBox() { return mHitBoxes.back(); }
	PlayerHitBox& Frame::GetLastHurtBox() { return mHurtBoxes.back(); }

	void Frame::SetHitBoxColor(Color c) { mHitBoxColor = c; }
	void Frame::SetHurtBoxColor(Color c) { mHurtBoxColor = c; }
	void Frame::ForceHitBoxColor(Color c) {
		for (unsigned int i = 0; i < mHitBoxes.size(); i++) {
			mHitBoxes[i].SetColor(c);
		}
	}
	void Frame::ForceHurtBoxColor(Color c) {
		for (unsigned int i = 0; i < mHurtBoxes.size(); i++) {
			mHurtBoxes[i].SetColor(c);
		}
	}
	void Frame::ForceHitHurtBoxColor(Color hit, Color hurt) {
		for (unsigned int i = 0; i < mHitBoxes.size(); i++) {
			mHitBoxes[i].SetColor(hit);
		}
		for (unsigned int i = 0; i < mHurtBoxes.size(); i++) {
			mHurtBoxes[i].SetColor(hurt);
		}
	}
	void Frame::ForceHitBoxColor() {
		for (unsigned int i = 0; i < mHitBoxes.size(); i++) {
			mHitBoxes[i].SetColor(mHitBoxColor);
		}
	}
	void Frame::ForceHurtBoxColor() {
		for (unsigned int i = 0; i < mHurtBoxes.size(); i++) {
			mHurtBoxes[i].SetColor(mHurtBoxColor);
		}
	}
	void Frame::ForceHitHurtBoxColor() {
		for (unsigned int i = 0; i < mHitBoxes.size(); i++) {
			mHitBoxes[i].SetColor(mHitBoxColor);
		}
		for (unsigned int i = 0; i < mHurtBoxes.size(); i++) {
			mHurtBoxes[i].SetColor(mHurtBoxColor);
		}
	}

	void Frame::EraseHitBox(unsigned short i) {
		if (i >= mHitBoxes.size())
			return;
		for (unsigned int j = i + 1; j < mHitBoxes.size(); j++) {
			mHitBoxes[j - 1] = mHitBoxes[j];
		}
		mHitBoxes.pop_back();
	}
	void Frame::EraseHurtBox(unsigned short i) {
		if (i >= mHurtBoxes.size())
			return;
		for (unsigned int j = i + 1; j < mHurtBoxes.size(); j++) {
			mHurtBoxes[j - 1] = mHitBoxes[j];
		}
		mHurtBoxes.pop_back();
	}
	void Frame::EraseActionPoint(unsigned short i) {
		if (i >= mActionPoints.size())
			return;
		for (unsigned int j = i + 1; j < mActionPoints.size(); j++) {
			mActionPoints[j - 1] = mActionPoints[j];
		}
		mActionPoints.pop_back();
	}

	void Frame::ClearHitBoxes() { mHitBoxes.clear(); }
	void Frame::ClearHurtBoxes() { mHurtBoxes.clear(); }
	void Frame::ClearActionPoints() { mActionPoints.clear(); }

	short Frame::AmountOfHitBoxes() { return mHitBoxes.size(); }
	short Frame::AmountOfHurtBoxes() { return mHurtBoxes.size(); }

	void Frame::SetColor(Color col) { mColor = col; }

	Color    Frame::GetHitBoxColor() { return mHitBoxColor; }
	Color    Frame::GetHurtBoxColor() { return mHurtBoxColor; }

	void Frame::Update(float dur) {
		FrameData::Update(dur);
	}

	void Frame::Update(Vector2f origin) {
		FrameData::Update();
		AdjustHitBoxes(origin);
	}

	void Frame::AdjustHitBoxes(Vector2<float> origin) {
		for (unsigned int i = 0; i < mHitBoxes.size(); i++) {
			Vector2<float> adjust = mHitBoxes[i].GetOffset();
			adjust += origin;
			mHitBoxes[i].SetXY(origin);
		}
		for (unsigned int i = 0; i < mHurtBoxes.size(); i++) {
			Vector2<float> adjust = mHurtBoxes[i].GetOffset();
			adjust += origin;
			mHurtBoxes[i].SetXY(adjust);
		}
	}

	void Frame::DrawHitBoxes() {

	}
	void Frame::DrawHurtBoxes() {

	}
	void Frame::DrawHitHurtBoxes() {
	}

	void Frame::SaveToFile(const char* filename) {
	}
	void Frame::DumpToFile(ofstream& outFile) {}
	void Frame::LoadFromFile(const char* filename) {}
}

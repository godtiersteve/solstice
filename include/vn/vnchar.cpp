#include "vnchar.h"

namespace solstice {
    VNCharEmote::VNCharEmote() {
        emotion = "";
    }
    VNCharEmote::VNCharEmote(string emote, MultiImage sprite) {
        emotion = emote;
        Init(sprite);
    }
    VNCharEmote& VNCharEmote::operator=(const MultiImage& img) {
        Init(img);
        return *this;
    }
    VNCharEmote& VNCharEmote::Initialize(string emote, Picture pic) {
        emotion = emote;
        Init(pic);
        return *this;
    }

    VNCharacter::VNCharacter() {
        Init();
    }
    VNCharacter::VNCharacter(string name, std::vector<VNCharEmote> emotes) {
        Init(name, emotes);
    }
    VNCharacter::VNCharacter(const VNCharacter& vnchar) {
        *this = vnchar;
    }
    VNCharacter& VNCharacter::operator=(const VNCharacter& vnchar) {
        mEmotes.clear();
        mName = vnchar.mName;
        for(int i = 0; i < vnchar.mEmotes.size(); i++) {
            mEmotes.push_back(vnchar.mEmotes[i]);
        }
        mChangeEffect = vnchar.mChangeEffect;
        return *this;
    }

    VNCharEmote& VNCharacter::GetEmote(size_t i)        {return mEmotes[i];}
    VNCharEmote& VNCharacter::GetEmote(string emote)    {
        for(int i = 0; i < mEmotes.size(); i++) {
            if(mEmotes[i].emotion == emote) {
                return mEmotes[i];
            }
        }
        return mEmotes[0];
    }
    void VNCharacter::AddEmote(VNCharEmote emote) {
        mEmotes.push_back(emote);
    }
    void VNCharacter::RemoveEmote(size_t i) {
        mEmotes.erase(mEmotes.begin() + i);
    }
    int VNCharacter::Size() {return mEmotes.size();}

    void VNCharacter::Init() {
        mAngle = 0;
        mScale = 1.0f;
        mID = 0;
        mName = "";
    }
    void VNCharacter::Init(string name, std::vector<VNCharEmote> emotes) {
        mName = name;
        mID = 0;
        mAngle = 0;
        mScale = 0;
        mEmotes.clear();
        for(int i = 0; i < emotes.size(); i++) {
            mEmotes.push_back(emotes[i]);
        }
        mChangeEffect.Clear();
    }

    VNCharEmote& VNCharacter::GetActiveEmote()          {return mActiveEmote;}
    void VNCharacter::ChangeEmote(string emote) {
        mActiveEmote = GetEmote(emote);
        mActiveEmote.SetPosition(mPosition);
        mActiveEmote.SetAngle(mAngle);
        mActiveEmote.SetScale(mScale);
        mChangeEffect.Reset();
    }
    void VNCharacter::ChangeEmote(size_t i) {
        mActiveEmote = mEmotes[i];
    }
    void VNCharacter::Update() {
        mChangeEffect.Update(mActiveEmote);
        mActiveEmote.SetScale(mScale);
        mActiveEmote.SetAngle(mAngle);
    }
    /*
    void VNCharacter::Draw(Renderer& r) {
        Vector2f oldPos = mActiveEmote.GetOrigin();
        mActiveEmote.SetPosition(oldPos + mPosition);
        r.DrawMultiQuadImg(*mActiveEmote.GetMultiImage());
        mActiveEmote.SetPosition(oldPos);
    }
    */
}

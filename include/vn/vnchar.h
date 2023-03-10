#ifndef VNCHARACTER_H_INCLUDED
#define VNCHARACTER_H_INCLUDED

#include <string>
#include <vector>

#include "graphics/vector2.h"
#include "graphics/multiquad.h"
#include "effects/imageeffect.h"

using std::string;

namespace solstice {
class VNCharEmote : public MultiImage {
    public:
        string emotion;
        VNCharEmote();
        VNCharEmote(string emote, MultiImage sprite);
        VNCharEmote& operator=(const MultiImage& img);
        VNCharEmote& Initialize(string emote, Picture pic);
        MultiImage* GetMultiImage() {return this;}
};
class VNCharacter : public MultiImage {
    protected:
        string mName;
        int mID;
        std::vector<VNCharEmote> mEmotes;
        VNCharEmote mActiveEmote;
        Vector2f mPosition;
        float mAngle, mScale;
        ImageEffect mChangeEffect;
    public:
        VNCharacter();
        VNCharacter(string name, std::vector<VNCharEmote> emotes);
        VNCharacter(const VNCharacter& vnchar);
        VNCharacter& operator=(const VNCharacter& vnchar);

        void SetName(string n)              {mName = n;}
        void SetPosition(Vector2f pos)      {mPosition = pos;}
        void SetAngle(float a)              {mAngle = a;}
        void SetScale(float s)              {mScale = s; MultiImage::SetScale(mScale);}
        void SetID(int id)                  {mID = id;}

        string GetName()                        {return mName;}
        Vector2f GetPosition()                  {return mPosition;}
        float GetAngle()                        {return mAngle;}
        float GetScale()                        {return mScale;}
        ImageEffect& GetChangeEffect()          {return mChangeEffect;}
        int GetID()                             {return mID;}

        VNCharEmote& GetEmote(size_t i);
        VNCharEmote& GetEmote(string emote);
        void AddEmote(VNCharEmote emote);
        void RemoveEmote(size_t i);
        int Size();

        void Init();
        void Init(string name, std::vector<VNCharEmote> emotes);

        VNCharEmote& GetActiveEmote();
        void ChangeEmote(string emote);
        void ChangeEmote(size_t i);
        void Update();
};

}
#endif // VNSPRITE_H_INCLUDED

#include "playerhitbox.h"
namespace solstice {
PlayerHitBox::PlayerHitBox():
mType(HBOX_NONE), mDamage(0), mXPush(0), mYPush(0), mOffset(0),
mPassable(true), mHittable(true), mFlipped(false)
{}

PlayerHitBox::PlayerHitBox(Rectangle<float> rect):
mType(HBOX_NONE), mDamage(0), mXPush(0), mYPush(0), mOffset(0),
mPassable(true), mHittable(true), mFlipped(false)
{
    *this = rect;
}
PlayerHitBox::PlayerHitBox(Rectangle<float> rect, PlayerHitBoxType type, float damage):
mType(type), mDamage(damage), mXPush(0), mYPush(0),
mPassable(true), mHittable(true), mFlipped(false)
{
    *this = rect;
}
PlayerHitBox& PlayerHitBox::operator=(Rectangle<float>& rect) {
    this->SetXYHW(rect.GetOrigin(), rect.GetHeight(), rect.GetWidth());
    this->SetAngle(rect.GetAngle());
    this->SetScale(rect.GetScale());
    this->SetDrawFrom(rect.GetDrawFrom());
    return *this;
}

void PlayerHitBox::SetType(PlayerHitBoxType type)       {mType = type;}
void PlayerHitBox::SetDamage(float damage)              {mDamage = damage;}
void PlayerHitBox::SetXPush(float x)                    {mXPush = x;}
void PlayerHitBox::SetYPush(float y)                    {mYPush = y;}
void PlayerHitBox::SetXYPush(float x, float y)          {mXPush = x; mYPush = y;}
void PlayerHitBox::SetPassable(bool pass)               {mPassable = pass;}
void PlayerHitBox::SetHittable(bool hit)                {mHittable = hit;}
void PlayerHitBox::Flip()                               {mFlipped = !mFlipped;}
void PlayerHitBox::SetOffsetX(float x)                  {mOffset.x = x;}
void PlayerHitBox::SetOffsetY(float y)                  {mOffset.y = y;}
void PlayerHitBox::SetOffset(Vector2<float> off)        {mOffset = off;}
void PlayerHitBox::SetColor(Color c)                    {mColor = c;}

PlayerHitBoxType    PlayerHitBox::GetType()   {return mType;}
float               PlayerHitBox::GetDamage() {return mDamage;}
float               PlayerHitBox::GetXPush()  {return mXPush;}
float               PlayerHitBox::GetYPush()  {return mYPush;}
bool                PlayerHitBox::Passable()  {return mPassable;}
bool                PlayerHitBox::Hittable()  {return mHittable;}
bool                PlayerHitBox::Flipped()   {return mFlipped;}
const Vector2f&     PlayerHitBox::GetOffset() {return mOffset;}
Color               PlayerHitBox::GetColor()  {return mColor;}
const Rectanglef&   PlayerHitBox::GetHitRect() {return *this;}
};

#ifndef PHITBOX_H_INCLUDED
#define PHITBOX_H_INCLUDED

#include <fstream>

#include "geometry/rectangle.h"
#include "geometry/geometry.h"
#include "graphics/color.h"
#include "graphics/texture.h"
#include "graphics/texdata.h"

using std::ifstream;
using std::ofstream;
namespace solstice {
enum PlayerHitBoxType {HBOX_NONE, HBOX_DAMAGE, HBOX_HEAL, HBOX_HURTBOX, HBOX_HBOX,
             HBOX_OBSTACLE, HBOX_PLAYER, HBOX_ENEMY, HBOX_ALLY, HBOX_CUSTOM};
class PlayerHitBox : public Rectanglef
{
protected:
    PlayerHitBoxType mType;
    float mDamage;
    float mXPush, mYPush;
    bool mPassable, mHittable;
    bool mFlipped, mVisible;
    Vector2f mOffset;
    Color mColor;

public:
    PlayerHitBox();
    PlayerHitBox(Rectanglef rect);
    PlayerHitBox(Rectanglef rect, PlayerHitBoxType type, float damage = 0);
    PlayerHitBox& operator=(Rectanglef& rect);

    void SetType(PlayerHitBoxType type);
    void SetDamage(float damage);
    void SetXPush(float x);
    void SetYPush(float y);
    void SetXYPush(float x, float y);
    void SetPassable(bool pass);
    void SetHittable(bool hit);
    void Flip();
    void SetOffsetX(float x);
    void SetOffsetY(float y);
    void SetOffset(Vector2f off);
    void SetColor(Color c);

    PlayerHitBoxType GetType();
    float GetDamage();
    float GetXPush();
    float GetYPush();
    bool Passable();
    bool Hittable();
    bool Flipped();
    const Vector2f& GetOffset();
    Color GetColor();
    const Rectanglef& GetHitRect();
};
}
#endif // HITBOX_H_INCLUDED

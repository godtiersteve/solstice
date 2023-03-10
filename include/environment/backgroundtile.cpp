#include "backgroundtile.h"

namespace solstice {
BackgroundTile2::BackgroundTile2(TileType type) {
    SetType(type);
    mFriction = 0.0f;
    mStartPosition = 0;
    mEndPosition = 0;
    mPathSpeed = 0;
    mSprings = false;
    mReleased = false;
    mCollisions = 0;
    mWeightHeld = 0;
    mElasticity = 0.0f;
    mAngle = 0;
    mCollisionHint = COLHINT_GROUND;
    mFallThrough = true;
}
BackgroundTile2::BackgroundTile2(Rectanglef hitbox, TileType type) {
    *this = hitbox;
    SetType(type);
    mFriction = 0.0f;
    mStartPosition = 0;
    mEndPosition = 0;
    mPathSpeed = 0;
    mSprings = false;
    mReleased = false;
    mCollisions = 0;
    mWeightHeld = 0;
    mElasticity = 0.0f;
    mAngle = 0;
    mCollisionHint = COLHINT_GROUND;
    mFallThrough = true;
}
BackgroundTile2::BackgroundTile2(std::shared_ptr<IShapef> shape, TileType type) {
    SetType(type);
    mFriction = 0.0f;
    mStartPosition = 0;
    mEndPosition = 0;
    mPathSpeed = 0;
    mSprings = false;
    mReleased = false;
    mCollisions = 0;
    mWeightHeld = 0;
    mHitBox2.Clear();
    mHitBox2.AddHitBox(shape);
    mElasticity = 1.0f;
    mAngle = 0;
    mCollisionHint = COLHINT_GROUND;
}
BackgroundTile2::BackgroundTile2(const BackgroundTile2& bg) {
    Init(bg);
}
BackgroundTile2& BackgroundTile2::operator=(BackgroundTile2 bg) {
    Init(bg);
    return *this;
}
void BackgroundTile2::Init(const BackgroundTile2& bg) {
    mFriction = bg.mFriction;
    mStartPosition = bg.mStartPosition;
    mEndPosition = bg.mEndPosition;
    mPathSpeed = bg.mPathSpeed;
    mHitBox = bg.mHitBox;
    mType = bg.mType;
    mStyle = bg.mStyle;
    mSprings = bg.mSprings;
    mSpring = bg.mSpring;
    mReleased = bg.mReleased;
    mCollisions = bg.mCollisions;
    mWeightHeld = bg.mWeightHeld;
    mHitBox2 = bg.mHitBox2;
    mElasticity = bg.mElasticity;
    mAngle = bg.mAngle;
    mCollisionHint = bg.mCollisionHint;
}
BackgroundTile2& BackgroundTile2::operator=(Rectanglef& hitbox) {
    mHitBox = hitbox;
    mSpring.targetHeight = hitbox.GetBottom();
    mSpring.height = hitbox.GetBottom();
    mHitBox2.Clear();
    mHitBox2.AddHitBox(std::make_shared<Rectanglef>(mHitBox));
    return *this;
}
void BackgroundTile2::SetPath(Vector2f start, Vector2f end, float speed) {
    mStartPosition = start;
    mEndPosition = end;
    mPathSpeed = speed;
}
void BackgroundTile2::Update(float tstep) {
    if(mType == TILE_MOVING) {
        if(mPathSpeed != 0) {
            if(mHitBox.GetOrigin().x != mEndPosition.x && mHitBox.GetOrigin().y != mEndPosition.y) {
                Vector2f distance = mEndPosition = mHitBox.GetOrigin();
                mHitBox.AddXY(distance.x / mPathSpeed * tstep, distance.y / mPathSpeed * tstep);
            }
        }
    }
    else if(mType == TILE_PLATFORM) {
        if(mSprings) {
            //mSpring.height = mSpring.targetHeight - mWeightHeld;
            if(fabs(mSpring.height - mSpring.targetHeight) < 0.075f) {
                mSpring.height = mSpring.targetHeight;
                mSpring.speed = 0;
            }
            else {
                mSpring.Update(0.0, tstep);
                mHitBox.SetY(mSpring.height);
                Vector2f pos = mHitBox2.GetRect(0).GetPosition();
                pos.y = mSpring.height;
                mHitBox2.GetRect(0).SetPosition(pos);
            }
            mWeightHeld = 0;
        }
    }
    //*mHitBox2.GetHitBox(0) = mHitBox;
}
void BackgroundTile2::Push(float velocity) {
    mSpring.speed+= velocity / 2;
    mHitBox.SetY(mSpring.height);
}
void BackgroundTile2::SetHitBox(Rectanglef rect) {
    mHitBox = rect;
}
void BackgroundTile2::SetPosition(Vector2f position) {
    mHitBox.SetPosition(position);
    mSpring.height = position.y;
    mSpring.targetHeight = position.y;
    mHitBox2.SetPosition(position);
}
void BackgroundTile2::Write(std::ofstream& file) {
    BinaryWrite(file, mFriction);
    BinaryWrite(file, mStartPosition.x);
    BinaryWrite(file, mStartPosition.y);
    BinaryWrite(file, mEndPosition.x);
    BinaryWrite(file, mEndPosition.y);
    BinaryWrite(file, mType);
    BinaryWrite(file, mPathSpeed);
    mHitBox.Write(file);
}
void BackgroundTile2::Read(std::ifstream& file) {
    BinaryRead(file, mFriction);
    BinaryRead(file, mStartPosition.x);
    BinaryRead(file, mStartPosition.y);
    BinaryRead(file, mEndPosition.x);
    BinaryRead(file, mEndPosition.y);
    int type;
    BinaryRead(file, type);
    mType = static_cast<TileType>(type);
    BinaryRead(file, mPathSpeed);
    mHitBox.Read(file);
    mHitBox2.AddHitBox(std::shared_ptr<Rectanglef>(&mHitBox));
}
void BackgroundTile2::Write2(std::ofstream& file) {
    BinaryWrite(file, mFriction);
    BinaryWrite(file, mStartPosition.x);
    BinaryWrite(file, mStartPosition.y);
    BinaryWrite(file, mEndPosition.x);
    BinaryWrite(file, mEndPosition.y);
    BinaryWrite(file, mType);
    BinaryWrite(file, mPathSpeed);
    mHitBox2.Write(file);
}
void BackgroundTile2::Read2(std::ifstream& file) {
    BinaryRead(file, mFriction);
    BinaryRead(file, mStartPosition.x);
    BinaryRead(file, mStartPosition.y);
    BinaryRead(file, mEndPosition.x);
    BinaryRead(file, mEndPosition.y);
    int type;
    BinaryRead(file, type);
    mType = static_cast<TileType>(type);
    BinaryRead(file, mPathSpeed);
    mHitBox2.Read(file);
}
std::string BackgroundTile2::Style() {
    switch(mStyle) {
        case TILE_SLOPE_30_L:
            return "Left Slope (30)";
        case TILE_SLOPE_45_L:
            return "Left Slope (45)";
        case TILE_SLOPE_60_L:
            return "Left Slope (60)";
        case TILE_SLOPE_30_R:
            return "Right Slope (30)";
        case TILE_SLOPE_45_R:
            return "Right Slope (45)";
        case TILE_SLOPE_60_R:
            return "Right Slope (60)";
        case TILE_FLAT:
        default:
            return "Flat";
        break;
    }
}
std::string BackgroundTile2::Type() {
    switch(mType) {
    case TILE_MOVING:
        return "Moving Tile";
    case TILE_PLATFORM:
        return "Platform";
    case TILE_WATER:
        return "Water";
    case TILE_GROUND:
    default:
        return "Ground Tile";
    break;
    }
}
}

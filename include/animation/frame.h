#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include "framedata.h"
#include "graphics/texarray.h"
#include "player/playerhitbox.h"
#include "geometry/hitbox.h"

#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

namespace solstice {
class Frame : public FrameData {
protected:
    vector<PlayerHitBox> mHitBoxes;
    vector<PlayerHitBox> mHurtBoxes;
    Color mHitBoxColor, mHurtBoxColor;
    vector<Vector2f> mActionPoints;
public:
    HitBoxf mHitBox;
    Frame();
    Frame(FrameData data);
    Frame(TextureData data, short duration = 0);
    Frame(Vector2f pos, TextureData data, short duration = 0);
    Frame(const Frame& data);

    Frame& operator=(const Frame& data);
    Frame& operator=(TextureData& data);
    Frame& operator=(FrameData& data);

    PlayerHitBox& GetHitBox(unsigned short i);
    PlayerHitBox& GetHurtBox(unsigned short i);
    Vector2f& GetActionPoint(unsigned short i);

    void LoadImage(TextureArray& arr, string texture, short duration = 0);

    short AmountOfHitBoxes();
    short AmountOfHurtBoxes();

    void AddHitBox(PlayerHitBox hbox);
    void AddHurtBox(PlayerHitBox hbox);
    void AddActionPoint(Vector2<float> ap);
    void AddHitBox();
    void AddHurtBox();
    void AddActionPoint();
    PlayerHitBox& GetLastHitBox();
    PlayerHitBox& GetLastHurtBox();

    void SetHitBoxColor(Color c);
    void SetHurtBoxColor(Color c);

    void ForceHitBoxColor(Color c);
    void ForceHurtBoxColor(Color c);
    void ForceHitHurtBoxColor(Color hit, Color hurt);
    void ForceHitBoxColor();
    void ForceHurtBoxColor();
    void ForceHitHurtBoxColor();

    void EraseHitBox(unsigned short i);
    void EraseHurtBox(unsigned short i);
    void EraseActionPoint(unsigned short i);

    void ClearHitBoxes();
    void ClearHurtBoxes();
    void ClearActionPoints();

    void AutoSetHotSpot(DrawPoint point);
    void SetColor(Color col);

    Color GetHitBoxColor();
    Color GetHurtBoxColor();

    void AdjustHitBoxes(Vector2<float> origin);
    void Update(float dur = 1.0f);
    void Update(Vector2f origin);

    void DrawHitBoxes();
    void DrawHurtBoxes();
    void DrawHitHurtBoxes();

    void SaveToFile(const char* filename);
    void DumpToFile(ofstream& outFile);
    void LoadFromFile(const char* filename);
};
}

#endif // FRAME_H_INCLUDED

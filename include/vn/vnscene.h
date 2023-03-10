#ifndef VNSCENE_H_INCLUDED
#define VNSCENE_H_INCLUDED

#include <vector>
#include <string>
#include "vnchar.h"

using std::string;

namespace solstice {
enum SceneCmdType{SCENECMD_TEXT = 0, SCENECMD_SPRITE, SCENECMD_EFFECT, SCENECMD_SOUND};
struct SceneCmd {
    SceneCmdType type;
    string command;
    int commandID;
};

class VNScene {
private:
    std::vector<string> mText;
    std::vector<VNCharacter> mCharacters;
    string mName;
    unsigned int mID;

public:
    VNScene();
    VNScene(int id);
    VNScene(string name, int id)
};
}

#endif // VNSCENE_H_INCLUDED

#ifndef WATERSHED_INTEROP_H
#define WATERSHED_INTEROP_H

#include "engine/watershed.h"
#include "loader/bone_loader.h"
#include "kaguya/kaguya.hpp"

namespace solstice {
class WatershedLuaBind : public WatershedEngine {
kaguya::State mState;
public:
    WatershedLuaBind(float height, float width, std::string script = "");
    void BindLua(kaguya::State& state);
    void LoadModel(std::string file, std::string ref);
    SkeletalModel LoadBoneModel(std::string file, std::string ref = "");
    SkeletalModel LoadModelAndAnimations(std::string prefix, std::string path, std::string ext);
    int GetTextureID(string tex) {return mTextures.GetTextureID(tex);}
    kaguya::State& GetState() {return mState;}
    void RunScripts();
    void UpdateEvents(SDL_Event& event);
    void UpdateEvent(SDL_Event& event);
    void LoadScript(std::string script);

};
}

#endif // WATERSHED_INTEROP_H

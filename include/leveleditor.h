#ifndef LEVELEDITOR_H_INCLUDED
#define LEVELEDITOR_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <functional>
#include "GL/glew.h"
#include "SDL2/sdl.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "GL/glu.h"

#include "graphics/quaddata.h"
#include "graphics/color.h"
#include "graphics/graphicglobals.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/shaderprogram.h"
#include "graphics/vertex3.h"
#include "graphics/plane.h"
#include "graphics/scenerenderer.h"

#include "graphics/vertex3.h"
#include "graphics/3d/mesh.h"
#include "graphics/3d/model.h"

#include "text/sol_text.h"
#include "text/sol_font.h"

#include "graphics/texarray.h"
#include "animation/frame.h"
#include "animation/animation.h"


#include "geometry/polygon.h"
#include "environment/backgroundtile.h"
#include "environment/room.h"

#include "physics/physicsrect.h"
#include "environment/watersurface.h"
#include "environment/waterbody.h"
#include "physics/chain.h"

#include "player/baseplayer2.h"
#include "player/playerwatershed.h"
#include "player/camera2d.h"

void LevelEditorControls(SDL_Event& event, solstice::BackgroundTile2* selectedTile, std::vector<solstice::BackgroundTile2>& level, size_t& selectedTileIndex, bool shiftHeld, Vector2f camPos) {
    if(event.key.keysym.sym == SDLK_t) {
        solstice::BackgroundTile2 tile(solstice::Rectanglef(-camPos, 50, 300));
        //tile.mColor = Color(0.0f, 0.5f, 0.5f, 0.5f);
        tile.GetHitBox().SetDrawFrom(solstice::DRAW_CENTER);
        level.push_back(tile);
        if(level.size() >= 1 && selectedTileIndex < level.size()) {
            selectedTile = &level[selectedTileIndex];
        }
        else {
            selectedTile = NULL;
        }
    }
    if(event.key.keysym.sym == SDLK_y) {
        solstice::BackgroundTile2 tile(solstice::Rectanglef(-camPos, 300, 50));
        //tile.color = Color(0.0f, 0.5f, 0.5f, 0.5f);
        tile.GetHitBox().SetDrawFrom(solstice::DRAW_CENTER);
        level.push_back(tile);
        if(level.size() >= 1 && selectedTileIndex < level.size()) {
            selectedTile = &level[selectedTileIndex];
        }
        else {
            selectedTile = NULL;
        }
    }
    if(event.key.keysym.sym == SDLK_k) {
        if(selectedTile) {
            level.erase(level.begin() + selectedTileIndex);
            selectedTileIndex = 0;
            selectedTile = NULL;
        }
    }
    if(event.key.keysym.sym == SDLK_q) {
        if(selectedTile) {
            selectedTile->GetHitBox().SetAngle(selectedTile->GetHitBox().GetAngle() + solstice::DegreesToRadians(1.0f));
        }
    }
    if(event.key.keysym.sym == SDLK_e) {
        if(selectedTile) {
            selectedTile->GetHitBox().SetAngle(selectedTile->GetHitBox().GetAngle() - solstice::DegreesToRadians(1.0f));
        }
    }
    if(event.key.keysym.sym == SDLK_w) {
        if(selectedTile) {
            if(!shiftHeld) {
                selectedTile->GetHitBox().AddY(1);
            }
            else {
                selectedTile->GetHitBox().SetHeight(selectedTile->GetHitBox().GetHeight() + 5);
            }
        }
    }
    if(event.key.keysym.sym == SDLK_a) {
        if(selectedTile) {
            if(!shiftHeld) {
                selectedTile->GetHitBox().AddX(-1);
            }
            else {
                selectedTile->GetHitBox().SetWidth(selectedTile->GetHitBox().GetWidth() - 5);
            }
        }
    }
    if(event.key.keysym.sym == SDLK_s) {
        if(selectedTile) {
            if(!shiftHeld) {
                selectedTile->GetHitBox().AddY(-1);
            }
            else {
                selectedTile->GetHitBox().SetHeight(selectedTile->GetHitBox().GetHeight() - 5);
            }
        }
    }
    if(event.key.keysym.sym == SDLK_d) {
        if(selectedTile) {
            if(!shiftHeld) {
                selectedTile->GetHitBox().AddX(1);
            }
            else {
                selectedTile->GetHitBox().SetWidth(selectedTile->GetHitBox().GetWidth() + 5);
            }
        }
    }
    solstice::LevelRoom levelSave;
    if(event.key.keysym.sym == SDLK_F5) {
        levelSave.SetGroundTiles(level);
        levelSave.Write("testlevel3.slv");
    }
    if(event.key.keysym.sym == SDLK_F6) {
        levelSave.Read("testlevel3.slv");
        level = levelSave.GetGroundTiles();
    }
}

#endif // LEVELEDITOR_H_INCLUDED

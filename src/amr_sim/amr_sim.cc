#include "amr_sim/amr_sim.h"

AMR_Sim::AMR_Sim() {
    // name of simulator
    sAppName = "AMR Simulator";

    // no offset between world space and screen space
    _offset = olc::vf2d(0,0);
    // one-to-one mapping between world space and screen space
    _scale = 1.0;
}

bool AMR_Sim::OnUserCreate()
{
    // Called once at the start, so create things here
    return true;
}

bool AMR_Sim::OnUserUpdate(float fElapsedTime)
{
    // called once per frame
    for (int x = 0; x < ScreenWidth(); x++)
        for (int y = 0; y < ScreenHeight(); y++)
            Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
    return true;
}

void AMR_Sim::WorldToScreen(const olc::vf2d &worldCoord, olc::vf2d &screenCoord){
    screenCoord = worldCoord - _offset;
}

void AMR_Sim::ScreenToWorld(const olc::vf2d &screenCoord, olc::vf2d &worldCoord){
    worldCoord = screenCoord + _offset;
}

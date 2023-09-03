#include "arv_sim/arv_sim.h"

bool Example::OnUserCreate()
{
    // Called once at the start, so create things here
    return true;
}

bool Example::OnUserUpdate(float fElapsedTime)
{
    // called once per frame
    for (int x = 0; x < ScreenWidth(); x++)
        for (int y = 0; y < ScreenHeight(); y++)
            Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
    return true;
}
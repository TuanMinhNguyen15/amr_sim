#include "amr_sim/amr_sim.h"

AMR_Sim::AMR_Sim():Base("AMR Simulator")
{

}

bool AMR_Sim::OnUserCreate()
{
    // Called once at the start, so create things here
    return true;
}

bool AMR_Sim::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::BLACK);
    PanAndZoom();

    // called once per frame
    olc::vf2d pos(0,0);
    olc::vd2d size(ScreenWidth()/3,ScreenHeight()/3);
    WorldToScreen(pos,pos);
    FillRect(pos,size*scale_,olc::BLUE);
    // FillCircle(pos+olc::vd2d(50,50),30);

    return true;
}

int main()
{
	AMR_Sim demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
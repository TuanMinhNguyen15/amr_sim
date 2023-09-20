#include "amr_sim/map_creator.h"

MapCreator::MapCreator():Base("Map Creator")
{
    controlPoints_.clear();
}


bool MapCreator::OnUserCreate()
{
    // Called once at the start, so create things here

    // testing only
    controlPoints_.push_back(olc::vf2d(100,200));
    controlPoints_.push_back(olc::vf2d(200,200));
    controlPoints_.push_back(olc::vf2d(200,400));
    controlPoints_.push_back(olc::vf2d(100,400));
    controlPoints_.push_back(olc::vf2d(500,200));
    controlPoints_.push_back(olc::vf2d(700,200));
    spline_.controlPoints = controlPoints_;

    return true;
}

bool MapCreator::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::BLACK);
    PanAndZoom();

    // select control point
    if (GetKey(olc::Key::TAB).bPressed)
    {
        indexSelected_++;
        indexSelected_ %= controlPoints_.size();
    }

    // move selected control point
    olc::vf2d &PSselected = spline_.controlPoints[indexSelected_];
    if (GetKey(olc::Key::W).bHeld)
    {
        PSselected.y -= fElapsedTime*vel_;
    }
    if (GetKey(olc::Key::S).bHeld)
    {
        PSselected.y += fElapsedTime*vel_;
    }
    if (GetKey(olc::Key::A).bHeld)
    {
        PSselected.x -= fElapsedTime*vel_;
    }
    if (GetKey(olc::Key::D).bHeld)
    {
        PSselected.x += fElapsedTime*vel_;
    }

    // draw control points
    for (int i = 0; i < spline_.controlPoints.size(); i++)
    {
        float r;
        olc::vf2d P = spline_.controlPoints[i];
        WorldToScreen(P,P);
        WorldToScreen(r_,r);

        if (i == indexSelected_)
        {
            FillCircle(P,r,olc::RED);
        }
        else
        {
            FillCircle(P,r,olc::BLUE);
        }
    }

    // draw interpolated points
    for (float t = 0; t < spline_.GetMaxParam(); t += 0.1)
    {
        olc::vf2d P;
        spline_.Interpolate(t,P);
        WorldToScreen(P,P);
        Draw(P);
    }

    return true;
}

int main()
{
	MapCreator mapCreator;
	if (mapCreator.Construct(700, 500, 4, 4))
		mapCreator.Start();

	return 0;
}
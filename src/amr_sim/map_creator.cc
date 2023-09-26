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

    // testing shapes
    Triangle::Params triangleParams;
    triangleParams.p1 = olc::vf2d {0,0};
    triangleParams.p2 = olc::vf2d {20,0};
    triangleParams.p3 = olc::vf2d {10,10};
    triangle_.SetParams(triangleParams);

    Rectangle::Params recParams;
    recParams.pCenter = olc::vf2d{0,0};
    recParams.W = 100.;
    recParams.H = 50.;
    recParams.theta = 0.;
    rectangle_.SetParams(recParams);

    return true;
}


/* Spline */
// bool MapCreator::OnUserUpdate(float fElapsedTime)
// {
//     Clear(olc::BLACK);
//     PanAndZoom();

//     // select control point
//     if (GetKey(olc::Key::TAB).bPressed)
//     {
//         indexSelected_++;
//         indexSelected_ %= controlPoints_.size();
//     }

//     // move selected control point
//     olc::vf2d &PSselected = spline_.controlPoints[indexSelected_];
//     if (GetKey(olc::Key::W).bHeld)
//     {
//         PSselected.y -= fElapsedTime*vel_;
//     }
//     if (GetKey(olc::Key::S).bHeld)
//     {
//         PSselected.y += fElapsedTime*vel_;
//     }
//     if (GetKey(olc::Key::A).bHeld)
//     {
//         PSselected.x -= fElapsedTime*vel_;
//     }
//     if (GetKey(olc::Key::D).bHeld)
//     {
//         PSselected.x += fElapsedTime*vel_;
//     }

//     // draw control points
//     for (int i = 0; i < spline_.controlPoints.size(); i++)
//     {
//         float r;
//         olc::vf2d P = spline_.controlPoints[i];
//         WorldToScreen(P,P);
//         WorldToScreen(r_,r);

//         if (i == indexSelected_)
//         {
//             FillCircle(P,r,olc::RED);
//         }
//         else
//         {
//             FillCircle(P,r,olc::BLUE);
//         }
//     }

//     // draw interpolated points
//     for (float t = 0; t < spline_.GetMaxParam(); t += 0.1)
//     {
//         olc::vf2d P;
//         spline_.Interpolate(t,P);
//         WorldToScreen(P,P);
//         Draw(P);
//     }

//     return true;
// }

bool MapCreator::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::CYAN);
    PanAndZoom();

    olc::vf2d mousePos;
    mousePos = GetMousePos();

    Triangle upperTriangle,lowerTriangle;
    Triangle::Params triangleParams;
    Rectangle::Params recParams;

    rectangle_.GetParams(recParams);
    if (GetMouse(0).bPressed)
    {
        recParams.pCenter = ScreenToWorld(mousePos);
    }
    if (GetKey(olc::O).bPressed)
    {
        recParams.theta += 0.1;
    }
    if (GetKey(olc::P).bPressed)
    {
        recParams.theta -= 0.1;
    }
    rectangle_.SetParams(recParams);

    rectangle_.GetInternalTriangles(upperTriangle,lowerTriangle);
    
    olc::Pixel color;
    if (rectangle_.isInside(ScreenToWorld(mousePos)))
    {
        color = olc::RED;
    }
    else
    {
        color = olc::BLUE;
    }

    // std::cout << "p1: x = " << WorldToScreen(triangleParams.p1).x << " , y = " << WorldToScreen(triangleParams.p1).y << std::endl;
    upperTriangle.GetParams(triangleParams);
    FillTriangle(WorldToScreen(triangleParams.p1),
                 WorldToScreen(triangleParams.p2),
                 WorldToScreen(triangleParams.p3),
                 color);

    lowerTriangle.GetParams(triangleParams);
    FillTriangle(WorldToScreen(triangleParams.p1),
                 WorldToScreen(triangleParams.p2),
                 WorldToScreen(triangleParams.p3),
                 color);

    // FillRect(olc::vf2d{0,0},olc::vi2d{100,100});

    return true;
}


int main()
{
	MapCreator mapCreator;
	if (mapCreator.Construct(700, 500, 4, 4))
		mapCreator.Start();

	return 0;
}
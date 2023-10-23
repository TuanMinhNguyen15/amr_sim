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
    controlPoints_.push_back(olc::vf2d(300,200));
    controlPoints_.push_back(olc::vf2d(300,300));
    controlPoints_.push_back(olc::vf2d(300,400));
    controlPoints_.push_back(olc::vf2d(200,400));
    controlPoints_.push_back(olc::vf2d(100,400));
    controlPoints_.push_back(olc::vf2d(100,300));
    spline_.controlPoints = controlPoints_;

    Road::Params roadParams;
    roadParams.controlPoints = controlPoints_;
    roadParams.w = 20;
    road_.SetParams(roadParams);

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
bool MapCreator::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::BLACK);
    PanAndZoom();

    // select control point
    // if (GetKey(olc::Key::TAB).bPressed)
    // {
    //     indexSelected_++;
    //     indexSelected_ %= controlPoints_.size();
    // }

    if (GetMouse(0).bHeld)
    {
        olc::vf2d mousePos;
        mousePos.x = GetMouseX();
        mousePos.y = GetMouseY();
        mousePos = ScreenToWorld(mousePos);

        // determine selected point
        if (!isSelected_)
        {
            for (int i = 0; i < controlPoints_.size(); i++)
            {
                olc::vf2d p = controlPoints_[i];
                olc::vf2d d = p - mousePos;
                float mag = d.mag();

                if (mag <= 5)
                {
                    indexSelected_ = i;
                    isSelected_ = true;
                    break;
                }
            }
        }
        else
        {
            controlPoints_[indexSelected_] = mousePos;
        }
    }
    else
    {
        isSelected_ = false;
    }

    spline_.controlPoints = controlPoints_;

    Road::Params roadParams;
    roadParams.controlPoints = controlPoints_;
    roadParams.w = 20;
    road_.SetParams(roadParams);

    // // move selected control point
    // olc::vf2d &PSselected = spline_.controlPoints[indexSelected_];
    // if (GetKey(olc::Key::W).bHeld)
    // {
    //     PSselected.y -= fElapsedTime*vel_;
    // }
    // if (GetKey(olc::Key::S).bHeld)
    // {
    //     PSselected.y += fElapsedTime*vel_;
    // }
    // if (GetKey(olc::Key::A).bHeld)
    // {
    //     PSselected.x -= fElapsedTime*vel_;
    // }
    // if (GetKey(olc::Key::D).bHeld)
    // {
    //     PSselected.x += fElapsedTime*vel_;
    // }


    // draw triangles
    // std::vector<Triangle> triangles;
    // road_.GetTriangles(triangles);
    // for (Triangle triangle : triangles)
    // {
    //     Triangle::Params triangleParams;
    //     triangle.GetParams(triangleParams);
    //     DrawTriangle(WorldToScreen(triangleParams.p1),
    //                     WorldToScreen(triangleParams.p2),
    //                     WorldToScreen(triangleParams.p3),
    //                     olc::GREY);
    // }

    // draw control points
    for (int i = 0; i < controlPoints_.size(); i++)
    {
        float r = 5;
        olc::vf2d P = controlPoints_[i];

        if (i == indexSelected_)
        {
            FillCircle(WorldToScreen(P),WorldToScreen(r),olc::RED);
        }
        else
        {
            FillCircle(WorldToScreen(P),WorldToScreen(r),olc::BLUE);
        }
    }

    // draw interpolated points
    float w = 20;
    for (float t = 0; t <= spline_.GetMaxT(); t += 1)
    {
        olc::vf2d p,pLeft,pRight,pGradient;
        std::cout << "t = " << t << std::endl;
        if (t == spline_.GetMaxT())
        {
            std::cout << "---------- Last Control Point ------------\n";
        }

        // center
        spline_.Interpolate(t,p);
        Draw(WorldToScreen(p));

        spline_.GetGradient(t,pGradient);
        // right
        pRight = p + w*pGradient;
        Draw(WorldToScreen(pRight),olc::RED);
        // left
        pLeft = p - w*pGradient;
        Draw(WorldToScreen(pLeft),olc::BLUE);
    }

    // olc::vf2d pMax;
    // spline_.Interpolate(7.9,pMax);
    // std::cout << "pMax.x: " << pMax.x << " , pMax.y: " << pMax.y << std::endl;

    return true;
}

// bool MapCreator::OnUserUpdate(float fElapsedTime)
// {
//     Clear(olc::CYAN);
//     PanAndZoom();

//     olc::vf2d mousePos;
//     mousePos = GetMousePos();

//     Triangle upperTriangle,lowerTriangle;
//     Triangle::Params triangleParams;
//     Rectangle::Params recParams;

//     rectangle_.GetParams(recParams);
//     if (GetMouse(0).bPressed)
//     {
//         recParams.pCenter = ScreenToWorld(mousePos);
//     }
//     if (GetKey(olc::O).bPressed)
//     {
//         recParams.theta += 0.1;
//     }
//     if (GetKey(olc::P).bPressed)
//     {
//         recParams.theta -= 0.1;
//     }
//     rectangle_.SetParams(recParams);

//     rectangle_.GetTriangles(upperTriangle,lowerTriangle);
    
//     olc::Pixel color;
//     if (rectangle_.isInside(ScreenToWorld(mousePos)))
//     {
//         color = olc::RED;
//     }
//     else
//     {
//         color = olc::BLUE;
//     }

//     // std::cout << "p1: x = " << WorldToScreen(triangleParams.p1).x << " , y = " << WorldToScreen(triangleParams.p1).y << std::endl;
//     upperTriangle.GetParams(triangleParams);
//     FillTriangle(WorldToScreen(triangleParams.p1),
//                  WorldToScreen(triangleParams.p2),
//                  WorldToScreen(triangleParams.p3),
//                  color);

//     lowerTriangle.GetParams(triangleParams);
//     FillTriangle(WorldToScreen(triangleParams.p1),
//                  WorldToScreen(triangleParams.p2),
//                  WorldToScreen(triangleParams.p3),
//                  color);

//     // FillRect(olc::vf2d{0,0},olc::vi2d{100,100});

//     return true;
// }


int main()
{
	MapCreator mapCreator;
	if (mapCreator.Construct(700, 500, 4, 4))
		mapCreator.Start();

	return 0;
}
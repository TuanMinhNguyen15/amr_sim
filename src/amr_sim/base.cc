#include "amr_sim/base.h"

Base::Base(std::string appName)
{
    // name of simulator
    sAppName = appName;
}

void Base::PanAndZoom()
{
    // update current mouse position
    mousePosCurr_.x = GetMouseX();
    mousePosCurr_.y = GetMouseY();

    /* Pan */
    if (GetKey(olc::Key::X).bHeld && GetMouse(0).bHeld)
    {
        if (isInitPan_)
        {
            // get current offset
            offsetCurr_ = offset_;
            // get starting mousing position
            mousePosStart_ = mousePosCurr_;
            
            isInitPan_ = false;
        }
        else
        {
            // update difference vector
            olc::vf2d offsetDiff = mousePosStart_ - mousePosCurr_;
            // update offset
            offset_ = offsetCurr_ + offsetDiff;
        }
    }
    else if (GetMouse(0).bReleased)
    {
        // reset
        isInitPan_ = true;
    }

    /* Zoom */
    // start zooming when z key is held and mouse-wheel is rolling
    if (GetKey(olc::Key::Z).bHeld && (GetMouseWheel() != 0))
    {    
        // save current mouse position in world space
        olc::vf2d mousePosBefore = mousePosCurr_;
        olc::vf2d mousePosBefore_World;
        mousePosBefore_World = ScreenToWorld(mousePosBefore);

        // update scale
        if (GetMouseWheel() > 0)
        {
            // zoom in
            scale_ *= 1.5;
        }
        else
        {
            // zoom out
            scale_ /= 1.5;
        }

        // update offset
        olc::vf2d mousePosAfter;
        mousePosAfter = WorldToScreen(mousePosBefore_World);
        olc::vf2d mousePosDiff = mousePosAfter - mousePosBefore;
        offset_ += mousePosDiff;
    }
}

olc::vf2d Base::WorldToScreen(const olc::vf2d &worldCoord)
{
    return (worldCoord*scale_) - offset_;
}

olc::vf2d Base::ScreenToWorld(const olc::vf2d &screenCoord)
{
    return (screenCoord + offset_)/scale_;
}

float Base::WorldToScreen(const float &unitIn)
{
    return unitIn*scale_;
}

float Base::ScreenToWorld(const float &pixelIn)
{
    return pixelIn/scale_;
}

// Base::Spline::Spline()
// {
//     type_ = Type::Normal;
//     controlPoints.clear();
// }

// Base::Spline::Spline(Type type):type_(type)
// {
//     controlPoints.clear();
// }

// int Base::Spline::GetMaxParam()
// {
//     if (type_ == Type::Normal)
//     {
//         return controlPoints.size()-3;
//     }
//     else
//     {
//         return controlPoints.size();
//     }
// }

// Base::Spline::Type Base::Spline::GetType()
// {
//     return type_;
// }

// void Base::Spline::ChangeType(Type type)
// {
//     type_ = type;
// }

// bool Base::Spline::Interpolate(const float &t, olc::vf2d &P)
// {
//     if (t < 0.)
//     {
//         std::cerr << "Error: Spline parameter t must be non-negative\n";
//         return false;
//     }

//     if (controlPoints.size() < 4)
//     {
//         std::cerr << "Error: Catmull-Rom spline needs at least 4 control points\n";
//         return false;
//     }

//     if (t > GetMaxParam())
//     {
//         std::cerr << "Error: Given " << ((type_ == Type::Normal)?"Normal":"Loop") << " mode, t is out-of-range\n";
//         return false;
//     }

//     olc::vf2d P0,P1,P2,P3;
//     float t_;
//     int index = static_cast<int>(t);
//     if (type_ == Type::Normal)
//     {
//         /* Normal Mode */
//         // control points P0,P1,P2,P3,P4,P5
//         // t = [0,1] => P0,P1,P2,P3
//         // t = [1,2] => P1,P2,P3,P4
//         // t = [2,3] => P2,P3,P4,P5

//         P0 = controlPoints[index];
//         P1 = controlPoints[index+1];
//         P2 = controlPoints[index+2];
//         P3 = controlPoints[index+3];
//     }
//     else
//     {   
//         /* Loop Mode */
//         P0 = controlPoints[(((index-1) < 0) ? (controlPoints.size()-1) : (index-1))];
//         P1 = controlPoints[index];
//         P2 = controlPoints[(index+1)%controlPoints.size()];
//         P3 = controlPoints[(index+2)%controlPoints.size()];
//     }
//     t_ = t - static_cast<float>(index);

//     P = 0.5*((2*P1) +
//              (-P0+P2)*t_ + 
//              (2*P0-5*P1+4*P2-P3)*std::pow(t_,2) + 
//              (-P0+3*P1-3*P2+P3)*std::pow(t_,3));

//     return true;
// }
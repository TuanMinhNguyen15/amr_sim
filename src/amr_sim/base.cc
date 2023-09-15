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
    if (GetMouse(0).bHeld)
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
        ScreenToWorld(mousePosBefore,mousePosBefore_World);

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
        WorldToScreen(mousePosBefore_World,mousePosAfter);
        olc::vf2d mousePosDiff = mousePosAfter - mousePosBefore;
        offset_ += mousePosDiff;
    }
}

void Base::WorldToScreen(const olc::vf2d &worldCoord, olc::vf2d &screenCoord)
{
    screenCoord = (worldCoord*scale_) - offset_;
}

void Base::ScreenToWorld(const olc::vf2d &screenCoord, olc::vf2d &worldCoord)
{
    worldCoord = (screenCoord + offset_)/scale_;
}

void Base::WorldToScreen(const float &unitIn, float &pixelOut)
{
    pixelOut = unitIn*scale_;
}

void Base::ScreenToWorld(const float &pixelIn, float &unitOut)
{
    unitOut = pixelIn/scale_;
}

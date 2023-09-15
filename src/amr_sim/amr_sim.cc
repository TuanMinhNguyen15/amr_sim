#include "amr_sim/amr_sim.h"

AMR_Sim::AMR_Sim() 
{
    // name of simulator
    sAppName = "AMR Simulator";

    // no offset between world space and screen space
    offset_ = olc::vf2d(0,0);
    // one-to-one mapping between world space and screen space
    scale_ = 1.0;
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

void AMR_Sim::PanAndZoom()
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
    // if (GetKey(olc::Key::Z).bHeld && (GetMouseWheel() != 0))
    // {    
    //     if (isInitZoom_)
    //     {
    //         // we want current mouse position to be fixed when zooming
    //         mousePosFixed_ = mousePosCurr_;
    //         isInitZoom_ = false;
    //     }

    //     // update scale
    //     if (GetMouseWheel() > 0)
    //     {
    //         // zoom in
    //         scale_ *= 1.5;
    //     }
    //     else
    //     {
    //         // zoom out
    //         scale_ /= 1.5;
    //     }

    //     // update offset
    //     olc::vf2d mousePosZoomed = static_cast<olc::vf2d>(mousePosFixed_)*scale_;
    //     olc::vf2d mousePosDiff = mousePosZoomed - static_cast<olc::vf2d>(mousePosFixed_);
    //     offset_ = mousePosDiff;
    // }
    // else
    // {
    //     isInitZoom_ = true;
    // }

    bool isZoomed = false;
    olc::vf2d mousePosWorld;
    ScreenToWorld(mousePosCurr_,mousePosWorld);
    if (GetKey(olc::Key::I).bPressed)
    {    
        isZoomed = true;
        scale_ *= 1.5;
    }
    else if (GetKey(olc::Key::O).bPressed)
    {
        isZoomed = true;
        scale_ /= 1.5;
    }

    if (isZoomed)
    {
        olc::vf2d mousePosScreen;
        WorldToScreen(mousePosWorld,mousePosScreen);
        olc::vf2d mousePosDiff = mousePosScreen - mousePosCurr_;
        offset_ += mousePosDiff;

        std::cout << "MouseScreen: x = " << mousePosScreen.x << " , y = " << mousePosScreen.y << std::endl;
        std::cout << "Scale: " << scale_ << std::endl;
        std::cout << "MouseCurrent: x = " << mousePosCurr_.x << " , y = " << mousePosCurr_.y << std::endl;
        std::cout << "Offset: x = " << offset_.x << " , y = " << offset_.y << std::endl;
    }
}

void AMR_Sim::WorldToScreen(const olc::vf2d &worldCoord, olc::vf2d &screenCoord)
{
    screenCoord = (worldCoord*scale_) - offset_;
}

void AMR_Sim::ScreenToWorld(const olc::vf2d &screenCoord, olc::vf2d &worldCoord)
{
    worldCoord = (screenCoord + offset_)/scale_;
}

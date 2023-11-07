#pragma once

#include "amr_sim/base.h"
#include "amr_sim/shapes.h"

class MapCreator : public Base 
{
  public:
    MapCreator();

  public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

  private:
    enum class StateMachine
    {
      INIT,
      HOME,
      CREATE,
      EDIT,
      EXPORT
    };

    enum class ShapeOption
    {
      TRIANGLE,
      RECTANGLE,
      CIRCLE,
      TRACK
    };

    StateMachine stateMachine_ = StateMachine::INIT;
    ShapeOption shapeOption_;
    std::vector<Shape*> shapesPtr_;
    float r_ = 4;

    olc::Pixel backgroundColor_ = olc::GREEN;
    olc::Pixel obstacleColor_ = olc::BLACK;
    olc::Pixel controlColor_ = olc::RED;

    olc::vf2d p1Prev_,p2Prev_,p3Prev_,p4Prev_;

    // shape to be editted in the EDIT state
    Shape *shapeEditPtr_;

    // select from vector of points
    bool isSeleted_ = false;
    int indexSelected_ = -1;

    olc::vf2d mousePos_;
    olc::vf2d mousePosPrev_;

    // triangle related
    int numPoint_Triangle_ = 0;
    olc::vf2d controlPoints_Triangle_[3];

    // rectangle related
    Rectangle::Params recParams_;
    olc::vf2d pCenterPrev_;
    olc::vf2d pPivot_;

  private:
    void Home();
    void Create();
    void Edit();
    void Draw();

    // utilities
    bool IsSelected(const olc::vf2d &pIn, const std::vector<olc::vf2d> &pVec, const float &r, int &indexOut);
};
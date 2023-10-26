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
    float r_ = 5;
    olc::Pixel backgroundColor_ = olc::GREEN;
    olc::Pixel obstacleColor_ = olc::BLACK;
    olc::Pixel controlColor_ = olc::RED;
    Shape *shapeEditPtr_;

    /* Triangle */
    int numPoint_Triangle_ = 0;
    olc::vf2d controlPoints_Triangle_[3];

  private:
    void Home();
    void Create();
    void Edit();
    void Draw();

    // utilities
};
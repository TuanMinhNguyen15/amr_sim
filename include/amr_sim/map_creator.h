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
    // test splines
    Spline spline_{Spline::Type::Loop};
    std::vector<olc::vf2d> controlPoints_;
    float r_ = 5;
    int indexSelected_ = 0;
    float vel_ = 100.;

  private:
    // test shapes
    Triangle triangle_;

};
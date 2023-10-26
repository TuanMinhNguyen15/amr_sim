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
    Spline spline_{true};
    Road road_;
    std::vector<olc::vf2d> controlPoints_;
    float r_ = 5;
    bool isSelected_ = false;
    int indexSelected_ = 0;
    float vel_ = 100.;

  private:
    // test shapes
    Triangle triangle_;
    Rectangle rectangle_;

};
#include "amr_sim/base.h"

/*
Primitive shapes
1. Spline (track) - only one
2. Rectangle (obstacle)
3. Circle (obstacle)
*/

class MapCreator : public Base 
{
	public:
		MapCreator();

	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

    private:
        Spline spline_{Spline::Type::Loop};
        std::vector<olc::vf2d> controlPoints_;
        float r_ = 5;
        int indexSelected_ = 0;
        float vel_ = 100.;
};
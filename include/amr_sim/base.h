#include "amr_sim/olcPixelGameEngine.h"


class Base : public olc::PixelGameEngine {
	public:
		Base(std::string appName);

	protected:
		olc::vf2d offset_ = olc::vf2d(0.,0.);
		olc::vf2d mousePosStart_,mousePosCurr_;
		olc::vf2d offsetCurr_;
		float scale_ = 1.;	
		bool isInitPan_ = true;

	protected:
		// utility functions
		void PanAndZoom();
		void WorldToScreen(const olc::vf2d &worldCoord, olc::vf2d &screenCoord);
		void ScreenToWorld(const olc::vf2d &screenCoord, olc::vf2d &worldCoord);
		void WorldToScreen(const float &unitIn, float &pixelOut);
		void ScreenToWorld(const float &pixelIn, float &unitOut);


};
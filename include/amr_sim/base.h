#include "amr_sim/olcPixelGameEngine.h"
#include <cmath>


class Base : public olc::PixelGameEngine {
	public:
		Base(std::string appName);

	protected:
		struct Spline 
		{
			// control points are added in order
			// need at least 4 control points
			// spline not goes through the first and last control points

			public:
				enum Type
				{
					Normal,
					Loop
				};
				Spline(Type type);
				Spline();

			public:
				bool Interpolate(const float &t, olc::vf2d &P);
				
				Type GetType();
				void ChangeType(Type type);
				int GetMaxParam();

			public:
				std::vector<olc::vf2d> controlPoints;

			private:
				Type type_;
		};		

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
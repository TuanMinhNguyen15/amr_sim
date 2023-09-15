#include "amr_sim/olcPixelGameEngine.h"


class AMR_Sim : public olc::PixelGameEngine {
	public:
		AMR_Sim();

	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

	private:
		olc::vf2d offset_;
		float scale_;	
		olc::vf2d mousePosStart_,mousePosCurr_,mousePosFixed_;
		olc::vf2d offsetCurr_;
		bool isInitPan_ = true;
		bool isInitZoom_ = true;

	private:
		// utility functions
		void PanAndZoom();
		void WorldToScreen(const olc::vf2d &worldCoord, olc::vf2d &screenCoord);
		void ScreenToWorld(const olc::vf2d &screenCoord, olc::vf2d &worldCoord);


};
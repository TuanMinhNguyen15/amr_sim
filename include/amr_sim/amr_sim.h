#include "amr_sim/olcPixelGameEngine.h"


class AMR_Sim : public olc::PixelGameEngine {
	public:
		AMR_Sim();

	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

	private:
		olc::vf2d _offset;
		float _scale;

	private:
		// utility functions
		void PanAndZoom();
		void WorldToScreen(const olc::vf2d &worldCoord, olc::vf2d &screenCoord);
		void ScreenToWorld(const olc::vf2d &screenCoord, olc::vf2d &worldCoord);


};
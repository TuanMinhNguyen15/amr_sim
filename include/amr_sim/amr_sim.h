#include "amr_sim/base.h"

class AMR_Sim : public Base 
{
	public:
		AMR_Sim();

	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
};
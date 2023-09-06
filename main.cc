#include "amr_sim/amr_sim.h"

int main()
{
	AMR_Sim demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}

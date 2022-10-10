#include <stdio.h>
#include <Threading/ThreadFunctions.h>

int main(void)
{
	int64_t cpuFrequency = Threading_GetCpuFrequency();
	double	ticksToMicroSeconds = (1.0 / cpuFrequency) * 1000 * 1000;

	int64_t timerDelta = 0;
	int64_t timerTicks = Threading_GetCpuTicks();
	int64_t timerFreqs = cpuFrequency / 60;	// Ticks per frame
	while (true)
	{
		printf("Frame: %lf\n", timerDelta * ticksToMicroSeconds / (1000 * 1000));

		int64_t currentTicks = Threading_GetCpuTicks();
		timerDelta = currentTicks - timerTicks;
		if (timerDelta < timerFreqs)
		{
			Threading_MicroSleep((int64_t)((timerFreqs - timerDelta) * ticksToMicroSeconds));

			timerDelta = timerFreqs;
			timerTicks = timerTicks + timerDelta;
		}
		else
		{
			timerTicks = currentTicks;
		}
	}

    return 0;
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++

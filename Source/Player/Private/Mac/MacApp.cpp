#include "Precompiled.h"
#include "SoftRenderer.h"
#include "MacUtil.h"
#include "MacPlayer.h"

int main(int argc, char** argv)
{
	ScreenPoint defScreenSize(800, 600);
	
	if (!MacPlayer::Create(argc, argv, defScreenSize))
	{
		return -1;
	}

	SoftRenderer instance(GameEngineType::DDD, new MacRSI());
	MacPlayer::gOnResizeFunc = [&instance](const ScreenPoint& InNewScreenSize) { 
		if (InNewScreenSize.HasZero()) {
			return;
		}
		instance.OnResize(InNewScreenSize); 
	};
    
	instance._PerformanceInitFunc = MacUtil::GetCyclesPerMilliSeconds;
	instance._PerformanceMeasureFunc = MacUtil::GetCurrentTimeStamp;
	instance._InputBindingFunc = MacUtil::BindInput;
	MacUtil::BindSystemInput(instance.GetSystemInput());

	MacPlayer::gOnTickFunc = [&instance]() {
		instance.OnTick();

		static float previousTimer = 0.f;
		static float updatePeriod = 500.f;
		float currentTime = instance.GetElapsedTime();
		if (currentTime - previousTimer > updatePeriod)
		{
			float frameFPS = instance.GetFrameFPS();
			MacPlayer::SetMacStatTitle(frameFPS);
			previousTimer = currentTime;
		}
	};

	MacPlayer::RunDefaultLoop();

	instance.OnShutdown();
	return 0;
}

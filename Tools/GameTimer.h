#pragma once

#include <algorithm>
#include <windows.h>

namespace Cefied
{
	namespace Game
	{
		struct GameTime
		{
			float totalTime;
			float deltaTime;
			uint64_t totalTicks;
			uint64_t deltaTicks;
		};

		class GameTimer {

		public:
			GameTime startNew();

			void update(GameTime & gameTime);

		private:
			int64_t m_currentTime  = 0;
			int64_t m_previousTime = 0;
			int64_t m_countsPerSec = 0;
		};
	}
}
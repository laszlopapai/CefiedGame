#pragma once
#include <map>
#include "GameTimer.h"

namespace Cefied
{
	namespace Game
	{
		class ConsoleFPS
		{
		public:
			ConsoleFPS();
			void update(const GameTime & gameTime);
			void write();

		private:
			const float m_renderFrequency;
			float       m_renderTime;
			uint32_t    m_frames;

			size_t      m_latestLength;
		};		
	}
}

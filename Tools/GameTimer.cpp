#include "GameTimer.h"


using namespace Cefied::Game;


GameTime GameTimer::startNew() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSec);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_previousTime);

	GameTime gameTime;
	ZeroMemory(&gameTime, sizeof(GameTime));

	return gameTime;
}

void GameTimer::update(GameTime & gameTime) {
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

	gameTime.deltaTicks = m_currentTime - m_previousTime;
	gameTime.totalTicks += gameTime.deltaTicks;
	gameTime.deltaTime = gameTime.deltaTicks / (float)m_countsPerSec;
	gameTime.totalTime += gameTime.deltaTime;
	m_previousTime = m_currentTime;
}
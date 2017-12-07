#include "ConsoleFPS.h"
#include <iostream>
#include <string>

using namespace Cefied::Game;

ConsoleFPS::ConsoleFPS() :
	m_renderFrequency(1),
	m_renderTime(0),
	m_frames(0),
	m_latestLength(0)
{ }

void ConsoleFPS::update(const GameTime& gameTime)
{
	m_renderTime += gameTime.deltaTime;

	m_frames++;
	if (m_renderTime >= m_renderFrequency) {
		write();
		m_frames = 0;
		m_renderTime -= m_renderFrequency;
	}
}

void ConsoleFPS::write()
{
	if (m_latestLength == 0)
	{
		std::cout << "FPS: ";
	}

	for (size_t i = 0; i < m_latestLength; i++) {
		std::cout << '\b';
	}

	auto fps = std::to_string(m_frames / m_renderFrequency);
	std::cout << fps;

	m_latestLength = fps.length();
}

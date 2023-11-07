#include "Timer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


static double qpc_delay;


Timer::Timer()
	: m_timeDilationFactor(1.0)
{
	__int64 qpc_frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&qpc_frequency);

	qpc_delay = 1.0 / (double) qpc_frequency;

	// Initial counter.
	this->reset();
}

void Timer::tick()
{
	if (m_paused)
	{
		m_deltaTime = 0.0;
		return;
	}

	__int64 qpc_frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&qpc_frequency);

	qpc_delay = m_timeDilationFactor / (double) qpc_frequency;

	m_lastTimeStamp = m_currentTimeStamp;

	QueryPerformanceCounter((LARGE_INTEGER*) &m_currentTimeStamp);

	m_deltaTime = (m_currentTimeStamp - m_lastTimeStamp) * qpc_delay;

	if (m_deltaTime < 0.0) {
		m_deltaTime = 0.0F;
	}
}

void Timer::reset()
{
	QueryPerformanceCounter((LARGE_INTEGER*) &m_currentTimeStamp);
	m_lastTimeStamp = m_currentTimeStamp;
	m_deltaTime = 0.0;

	m_paused = false;
}

void Timer::pause()
{
	m_paused = true;
}

void Timer::resume()
{
	this->reset();
}

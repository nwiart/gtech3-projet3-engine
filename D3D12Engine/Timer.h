#pragma once



class Timer
{
public:

	Timer();

	void tick();

	void reset();

	void pause();
	void resume();

	inline double getDeltaTime() const { return m_deltaTime; };


private:

	double m_deltaTime;

	__int64 m_currentTimeStamp;
	__int64 m_lastTimeStamp;

	bool m_paused;
};

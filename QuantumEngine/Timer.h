#pragma once



class Timer
{
public:

	Timer();

	void tick();

	void reset();

	void pause();
	void resume();

	void setTimeDilation(double f) { m_timeDilationFactor = f; }

	inline double getDeltaTime() const { return m_deltaTime; };

	inline bool IsPause() const { return m_paused; }

	
private:

	double m_deltaTime;

	double m_timeDilationFactor;

	__int64 m_currentTimeStamp;
	__int64 m_lastTimeStamp;

	bool m_paused;
};

#pragma once

#define MAX_SAMPLE_COUNT ULONG(50) // 50회의 프레임 처리시간을 누적하여 평균한다.

// 싱글톤 패턴
class CGameTimer
{
private:
	static CGameTimer* m_TimerInstance;
	CGameTimer();
	~CGameTimer();

public:
	static CGameTimer* GetInstance()
	{
		if (!m_TimerInstance) {
			m_TimerInstance = new CGameTimer();
		}
		return m_TimerInstance;
	}

	// 타이머의 시간을 갱신한다.
	void Tick(float fLockFPS = 0.0f);
	void Reset();
	void Stop();
	void Start();
	float GetTotalTime();

	// 프레임 레이트를 반환한다.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	// 프레임의 평균 경과 시간을 반환한다.
	float GetTimeElapsed();

private:
	double							m_fTimeScale;
	float							m_fTimeElapsed;

	__int64							m_nBasePerformanceCounter;
	__int64							m_nPausedPerformanceCounter;
	__int64							m_nStopPerformanceCounter;
	__int64							m_nCurrentPerformanceCounter;
	__int64							m_nLastPerformanceCounter;

	__int64							m_nPerformanceFrequencyPerSec;

	float							m_fFrameTime[MAX_SAMPLE_COUNT];
	ULONG							m_nSampleCount;

	unsigned long					m_nCurrentFrameRate;
	unsigned long					m_nFramesPerSecond;
	float							m_fFPSTimeElapsed;

	bool							m_bStopped;
};


#pragma once

#define MAX_SAMPLE_COUNT ULONG(50) // 50ȸ�� ������ ó���ð��� �����Ͽ� ����Ѵ�.

// �̱��� ����
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

	// Ÿ�̸��� �ð��� �����Ѵ�.
	void Tick(float fLockFPS = 0.0f);
	void Reset();
	void Stop();
	void Start();
	float GetTotalTime();

	// ������ ����Ʈ�� ��ȯ�Ѵ�.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	// �������� ��� ��� �ð��� ��ȯ�Ѵ�.
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


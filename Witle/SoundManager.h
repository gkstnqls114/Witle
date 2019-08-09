#pragma once
#include "FMOD\include\fmod.hpp"

#pragma comment (lib, "FMOD/lib/fmod64_vc.lib")

using namespace FMOD;

#define SOUND_TYPE 32
#define SOUND_MIN 1
#define SOUND_MAX 10000

class SoundManager
{
	static SoundManager* m_Instance;

public:
	System * pSystem;
	Sound * pSound[SOUND_TYPE];     // 사운드 하나하나를 의미
	Channel * pChannel[SOUND_TYPE]; // 채널은 사운드를 출력하는 스피커

public:
	SoundManager();
	~SoundManager();

	static SoundManager* GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new SoundManager();
		}
		return m_Instance;
	}

	int Volume = 10; // 볼륨
	int Dropper; // 진동수. 얼마나 멀리가나!
	int Distance; // 진동세기.

	void Play(int type);
	void Stop(int type);
};
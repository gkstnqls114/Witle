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
	Sound * pSound[SOUND_TYPE];     // ���� �ϳ��ϳ��� �ǹ�
	Channel * pChannel[SOUND_TYPE]; // ä���� ���带 ����ϴ� ����Ŀ

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

	int Volume = 10; // ����
	int Dropper; // ������. �󸶳� �ָ�����!
	int Distance; // ��������.

	void Play(int type);
	void Stop(int type);
};
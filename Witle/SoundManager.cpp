#include "stdafx.h"
#include "SoundManager.h"

SoundManager* SoundManager::m_Instance;

SoundManager::SoundManager()
{
	FMOD::System_Create(&pSystem);
	pSystem->init(
		FMOD_MAX_CHANNEL_WIDTH
		, FMOD_INIT_NORMAL
		, nullptr
	);

	for (auto& p : pSound) p = nullptr;

	// 사운드 추가
	pSystem->createSound(
		"Sound/Effect/select.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::EFFECT]
	);

	pSystem->createStream( // 배경음악
		"Sound/BGM/title.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::TITLE]
	);
}


SoundManager::~SoundManager()
{ 
	for (int i = 0; i < SOUND_TYPE; ++i)
	{
		pSound[i]->release();
	}

	pSystem->release();
	pSystem->close();
}

void SoundManager::Play(int type)
{
	pSystem->update();
	pSystem->playSound(pSound[(int)type]
		, nullptr, false, &pChannel[(int)type]);
}

void SoundManager::Stop(int type)
{ 
	pChannel[(int)type]->stop();
}

#include "stdafx.h"
#include "SoundManager.h"

SoundManager* SoundManager::m_Instance;

enum ENUM_SOUND
{
	INTRO,
	LOGO2
};
 

SoundManager::SoundManager()
{
	FMOD::System_Create(&pSystem);
	pSystem->init(
		FMOD_MAX_CHANNEL_WIDTH
		, FMOD_INIT_NORMAL
		, nullptr
	);

	for (auto& p : pSound) p = nullptr;

	// t사운드 추가
	//pSystem->createSound(
	//	"Sound/VIOLA_SKILL2.mp3"
	//	, FMOD_DEFAULT | FMOD_LOOP_OFF
	//	, nullptr
	//	, &pSound[(int)ENUM_SOUND::LOGO2]
	//);

	//pSystem->createStream( // 배경음악
	//	"Sound/Fez - Adventure.mp3"
	//	, FMOD_LOOP_NORMAL | FMOD_2D
	//	, nullptr
	//	, &pSound[(int)ENUM_SOUND::INTRO]
	//);
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
		, nullptr, false, nullptr);
}

void SoundManager::Stop(int type)
{
	pChannel[(int)type]->stop();
}

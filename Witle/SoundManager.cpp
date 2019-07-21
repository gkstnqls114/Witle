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

	FMOD::Channel *pChannel = 0;
	pChannel->setVolume(volume);

	for (auto& p : pSound) p = nullptr;

	// 사운드 추가

	///////////////////////////////////////////////// 반복 음악
	pSystem->createStream( // 시작음악
		"Sound/BGM/start.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::SATRT]
	);

	pSystem->createStream( // 배경음악
		"Sound/BGM/title.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::TITLE]
	);
	///////////////////////////////////////////////// 반복 음악

	///////////////////////////////////////////////// 캐릭터 효과음
	pSystem->createSound( // 매직 미사일 (타격)
		"Sound/Effect/Magic/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::MAGIC_MISIL]
	);

	pSystem->createSound( // 피격
		"Sound/Effect/damage.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::DAMAGE]
	);

	pSystem->createSound( // 죽음
		"Sound/Effect/dead.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::DEAD]
	);

	pSystem->createSound( // 빗자루 타기
		"Sound/Effect/Broom.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BROOM]
	);
	///////////////////////////////////////////////// 캐릭터 효과음

	///////////////////////////////////////////////// 몬스터 효과음
	// 크리피
	pSystem->createSound( // Move
		"Sound/Effect/Monster/creepy_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::CREEEPY_MOVE]
	);
	
	// 머시룸
	pSystem->createSound( // Move
		"Sound/Effect/Monster/mushroom_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::MUSHROOM_MOVE]
	);
	pChannel->set3DMinMaxDistance(50, 10000);
	
	// 스페이스
	 pSystem->createSound( // Move
		"Sound/Effect/Monster/space_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::SPACE_MOVE]
	);
	/////////////////////////////////////////////// 몬스터 효과음
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

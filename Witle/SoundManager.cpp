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

	// ���� �߰�

	///////////////////////////////////////////////// �ݺ� ����
	pSystem->createStream( // ��������
		"Sound/BGM/start.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::SATRT]
	);

	pSystem->createStream( // �������
		"Sound/BGM/title.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::TITLE]
	);
	///////////////////////////////////////////////// �ݺ� ����

	///////////////////////////////////////////////// ĳ���� ȿ����
	pSystem->createSound( // ���� �̻��� (Ÿ��)
		"Sound/Effect/Magic/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::MAGIC_MISIL]
	);

	pSystem->createSound( // �ǰ�
		"Sound/Effect/damage.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::DAMAGE]
	);

	pSystem->createSound( // ����
		"Sound/Effect/dead.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::DEAD]
	);

	pSystem->createSound( // ���ڷ� Ÿ��
		"Sound/Effect/Broom.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BROOM]
	);
	///////////////////////////////////////////////// ĳ���� ȿ����

	///////////////////////////////////////////////// ���� ȿ����
	// ũ����
	pSystem->createSound( // Move
		"Sound/Effect/Monster/creepy_move.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::CREEEPY_MOVE]
	);

	// �ӽ÷�
	pSystem->createSound( // Move
		"Sound/Effect/Monster/mushroom_move.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::MUSHROOM_MOVE]
	);

	// �����̽�
	pSystem->createSound( // Move
		"Sound/Effect/Monster/space_move.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::SPACE_MOVE]
	);
	///////////////////////////////////////////////// ���� ȿ����
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

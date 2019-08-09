/*
set3DAttributes �������� ��ġ�� ������ ���� �Լ�

	�Ű�����
		pos: �Ҹ� ������ ���� �������� ��ġ�� �޾ƿ;��Ѵ�. �װ��� ���� �Ű�����. (3d�������� ��ġ)
		vel: 3d ���� fps�� �ӵ�
		alt_pan_pos: ?? �������� ����
	��ȯ��
		�Լ� ����: FMOD_OK
		�Լ� ����: FMOD_RESULT ���� ���� ���� �� �� �� �ϳ�
*/

/*
���

	�Ÿ� ����
		system::set3DSettings�� ���� ����. �⺻ ���� Meters 'm,����'
	��Ʈ���� ����
		ChannelControl :: set3DSpread�� ����Ͽ� �����ʰ� ������ ���带 ���� �� �� �ִ�.
*/

// �÷��̾� ���� ��ġ ��������
// PlayerManager::GetMainPlayer();

// ���� ��ġ ��������
// PlayerManager::GetMainPlayer()->GetTransform().GetPosition();

#include "stdafx.h"
#include "MyConstants.h"
#include "SoundManager.h"

#include "Player.h"
#include "PlayerManager.h"

SoundManager* SoundManager::m_Instance;

const static bool isUsing = true;

SoundManager::SoundManager()
{
	if (!isUsing) return;

	FMOD::System_Create(&pSystem);
	pSystem->init(
		FMOD_MAX_CHANNEL_WIDTH
		, FMOD_INIT_NORMAL
		, nullptr
	);

	for (auto& p : pSound) p = nullptr;

	pSystem->set3DSettings(1.0f, 10, 10000.0);
	// pChannel[SOUND_TYPE]->set3DAttributes((FMOD_VECTOR*)&PlayerManager::GetMainPlayer()->GetTransform().GetPosition(),
	// 	(FMOD_VECTOR*)&PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), 0); // -> ���� (pos,vel,alt_pan_pos)
	// pChannel[SOUND_TYPE]->setPriority(); // 
	// pSystem->set3DListenerAttributes((int*)&PlayerManager::GetMainPlayer()->GetTransform().GetPosition());
	pChannel[SOUND_TYPE]->set3DAttributes((FMOD_VECTOR*)&PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), 0); // -> ���� (pos,vel,alt_pan_pos)
	pChannel[SOUND_TYPE]->setVolume(Volume); // -> ����

	pChannel[SOUND_TYPE]->set3DMinMaxDistance(SOUND_MIN, SOUND_MAX);

	// ���� �߰�

	// ��� //////////////////////////////////////////////////////////////////
	pSystem->createStream( // ����
		"Sound/BGM/main_sound.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::MAIN_SOUND]
	);

	pSystem->createStream( // ��ų���������� �����
		"Sound/BGM/skillpage_sound.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::SKILLPAGE_SOUND]
	);

	pSystem->createStream( // ���� �����
		"Sound/BGM/game_sound.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::GAME_SOUND]
	);

	pSystem->createStream( // Win
		"Sound/BGM/win.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::WIN_SOUND]
	);

	pSystem->createStream( // Lose
		"Sound/BGM/lose.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::LOSE_SOUND]
	);
	//4�� �ΰ� ���°�

	// ��� //////////////////////////////////////////////////////////////////

	// �÷��̾� //////////////////////////////////////////////////////////////////
	pSystem->createSound( // �÷��̾� �̵�
		"Sound/Effect/player_move.mp3"
		, FMOD_LOOP_NORMAL | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_MOVE]
	);

	pSystem->createSound( // �÷��̾� ��Ʈ
		"Sound/Effect/player_damage.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_DAMAGE]
	);

	pSystem->createSound( // �÷��̾� ����
		"Sound/Effect/player_damage.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_DEAD]
	);

	pSystem->createSound( // �÷��̾� ���ڷ�
		"Sound/Effect/Broom.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_BROOM]
	);

	pSystem->createSound( // �÷��̾� ���� �̻���
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_MAGIC_MISIL]
	);
	// 9�� �ΰ����Ͼ���
	pSystem->createSound( // �÷��̾� ���̾
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_FIRE_SOUND]
	);

	pSystem->createSound( // �÷��̾� ���̽� ��
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_ICE_SOUND]
	);

	pSystem->createSound( // �÷��̾� ����Ʈ�� ��
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_LIGHT_SOUND]
	);

	pSystem->createSound( // �÷��̾� ����
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_BLESSING_SOUND]
	);

	pSystem->createSound( // �÷��̾� �ǵ�
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_SHIELD_SOUND]
	);

	pSystem->createSound( // �÷��̾� ����
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_HEALING_SOUND]
	);
	// �÷��̾� //////////////////////////////////////////////////////////////////

	// �⺻ ���� //////////////////////////////////////////////////////////////////
	pSystem->createSound( // �⺻ ���� �̵�
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::MONSTER_MOVE_SOUND]
	);

	pSystem->createSound( // �⺻ ���� ��Ʈ
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::MONSTER_DAMAGE_SOUND]
	);

	pSystem->createSound( // �⺻ ���� ����
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::MONSTER_DEAD_SOUND]
	);
	// �⺻ ���� //////////////////////////////////////////////////////////////////

	// ���� ���� //////////////////////////////////////////////////////////////////
	pSystem->createSound( // ���� ���� �̵�
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_MOVE_SOUND]
	);

	pSystem->createSound( // ���� ���� ��Ʈ
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DAMAGE_SOUND]
	);

	pSystem->createSound( // ���� ���� ����
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DEAD_SOUND]
	);

	pSystem->createSound( // ���� ���� �극��
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_BREATH_SOUND]
	);

	pSystem->createSound( // ���� ���� �ٿƮ��
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DOWNSTROKE_SOUND]
	);

	pSystem->createSound( // ���� ���� ���Ͼ���
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_TAILATTACK_SOUND]
	);

	pSystem->createSound( // ���� ���� �뽬
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DASH_SOUND]
	);
	// ���� ���� //////////////////////////////////////////////////////////////////

	pChannel[SOUND_TYPE]->set3DMinMaxDistance(SOUND_MIN, SOUND_MAX);
}


SoundManager::~SoundManager()
{
	if (!isUsing) return;

	for (int i = 0; i < SOUND_TYPE; ++i)
	{
		pSound[i]->release();
	}

	pSystem->release();
	pSystem->close();
}

void SoundManager::Play(int type)
{
	if (!isUsing) return;

	pSystem->update();
	pSystem->playSound(pSound[(int)type]
		, nullptr, false, &pChannel[(int)type]);
}

void SoundManager::Stop(int type)
{
	if (!isUsing) return;

	pChannel[(int)type]->stop();
}

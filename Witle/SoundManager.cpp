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

#include "Monster.h"
#include "MonsterMovement.h"

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
		, FMOD_DEFAULT | FMOD_3D
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

	// ��� //////////////////////////////////////////////////////////////////

	// �÷��̾� //////////////////////////////////////////////////////////////////
	 pSystem->createSound( // �÷��̾� �̵�
	 	"Sound/Effect/player_move.mp3"
	 	, FMOD_DEFAULT | FMOD_2D
	 	, nullptr
	 	, &pSound[(int)ENUM_SOUND::PLAYER_MOVE]
	 );

	pSystem->createSound( // �÷��̾� ��Ʈ
		"Sound/Effect/player_damage.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_DAMAGE]
	);

	pSystem->createSound( // �÷��̾� ����
		"Sound/Effect/player_damage.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_DEAD]
	);

	pSystem->createSound( // �÷��̾� ���ڷ�
		"Sound/Effect/Broom.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_BROOM]
	);

	pSystem->createSound( // �÷��̾� ���� �̻���
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_MAGIC_MISIL]
	);

	pSystem->createSound( // �÷��̾� ���̾
		"Sound/Effect/fire.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_FIRE_SOUND]
	);

	pSystem->createSound( // �÷��̾� ���̽� ��
		"Sound/Effect/ice.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_ICE_SOUND]
	);

	pSystem->createSound( // �÷��̾� ����Ʈ�� ��
		"Sound/Effect/light.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_LIGHT_SOUND]
	);

	pSystem->createSound( // �÷��̾� ����
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_BLESSING_SOUND]
	);

	pSystem->createSound( // �÷��̾� �ǵ�
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_SHIELD_SOUND]
	);

	pSystem->createSound( // �÷��̾� ����
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_HEALING_SOUND]
	);
	// �÷��̾� //////////////////////////////////////////////////////////////////

	// �⺻ ���� //////////////////////////////////////////////////////////////////
	//pSystem->createSound( // �⺻ ���� �̵�
	//	"Sound/Effect/monster_move.mp3"
	//	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	//	, nullptr
	//	, &pSound[(int)ENUM_SOUND::MONSTER_MOVE_SOUND]
	//);

	//pSystem->createSound( // �⺻ ���� ��Ʈ
	//	"Sound/Effect/monster_move.mp3"
	//	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	//	, nullptr
	//	, &pSound[(int)ENUM_SOUND::MONSTER_DAMAGE_SOUND]
	//);

	//pSystem->createSound( // �⺻ ���� ����
	//	"Sound/Effect/monster_move.mp3"
	//	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	//	, nullptr
	//	, &pSound[(int)ENUM_SOUND::MONSTER_DEAD_SOUND]
	//);
	//// �⺻ ���� //////////////////////////////////////////////////////////////////

	//// ���� ���� //////////////////////////////////////////////////////////////////

	FMOD_VECTOR Boss_pos;
	Boss_pos.x = 15000.f;
	Boss_pos.y = 0.f;
	Boss_pos.z = 15000.f;
	FMOD_VECTOR Boss_vel = { 0.f,0.f,0.f };

	FMOD_VECTOR Player_pos;
	Player_pos.x = (float)PlayerManager::GetMainPlayer()->GetTransform().GetPosition().x;
	Player_pos.y = (float)PlayerManager::GetMainPlayer()->GetTransform().GetPosition().y;
	Player_pos.z = (float)PlayerManager::GetMainPlayer()->GetTransform().GetPosition().z;
	FMOD_VECTOR Player_vel = { 0.f,0.f,0.f };

	pChannel[(int)ENUM_SOUND::BOSS_MOVE_SOUND]->setVolume(0.5f); // -> ���� 
	pChannel[(int)ENUM_SOUND::BOSS_MOVE_SOUND]->set3DMinMaxDistance(SOUND_MIN, SOUND_MAX);
	pChannel[(int)ENUM_SOUND::BOSS_MOVE_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	pSystem->get3DListenerAttributes(0, &Player_pos, &Player_vel, 0, 0);
	pSystem->createSound( // ���� ���� �̵�
		"Sound/Effect/dragon_cry.mp3"
		, FMOD_3D | FMOD_DEFAULT
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_MOVE_SOUND]
	);

	// pChannel[(int)ENUM_SOUND::BOSS_DAMAGE_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	// pSystem->createSound( // ���� ���� ��Ʈ
	// 	"Sound/Effect/monster_move.mp3"
	// 	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	// 	, nullptr
	// 	, &pSound[(int)ENUM_SOUND::BOSS_DAMAGE_SOUND]
	// );
	// 
	// pChannel[(int)ENUM_SOUND::BOSS_DEAD_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	// pSystem->createSound( // ���� ���� ����
	// 	"Sound/Effect/monster_move.mp3"
	// 	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	// 	, nullptr
	// 	, &pSound[(int)ENUM_SOUND::BOSS_DEAD_SOUND]
	// );
	// 
	// pChannel[(int)ENUM_SOUND::BOSS_BREATH_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	// pSystem->createSound( // ���� ���� �극��
	// 	"Sound/Effect/monster_move.mp3"
	// 	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	// 	, nullptr
	// 	, &pSound[(int)ENUM_SOUND::BOSS_BREATH_SOUND]
	// );
	// 
	// pChannel[(int)ENUM_SOUND::BOSS_DOWNSTROKE_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	// pSystem->createSound( // ���� ���� �ٿƮ��
	// 	"Sound/Effect/monster_move.mp3"
	// 	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	// 	, nullptr
	// 	, &pSound[(int)ENUM_SOUND::BOSS_DOWNSTROKE_SOUND]
	// );
	// 
	// pChannel[(int)ENUM_SOUND::BOSS_TAILATTACK_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	// pSystem->createSound( // ���� ���� ���Ͼ���
	// 	"Sound/Effect/monster_move.mp3"
	// 	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	// 	, nullptr
	// 	, &pSound[(int)ENUM_SOUND::BOSS_TAILATTACK_SOUND]
	// );
	// 
	// pChannel[(int)ENUM_SOUND::BOSS_DASH_SOUND]->set3DAttributes(&Boss_pos, NULL); // -> ���� (pos,vel,alt_pan_pos
	// pSystem->createSound( // ���� ���� �뽬
	// 	"Sound/Effect/monster_move.mp3"
	// 	, FMOD_DEFAULT | FMOD_3D | FMOD_3D_LINEARROLLOFF
	// 	, nullptr
	// 	, &pSound[(int)ENUM_SOUND::BOSS_DASH_SOUND]
	// );
	// ���� ���� //////////////////////////////////////////////////////////////////
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

void SoundManager::Update(float fElapsedTime)
{
	pSystem->update();
}

void SoundManager::Play(int type)
{
	if (!isUsing) return;

	pSystem->update();
	pSystem->playSound(pSound[(int)type]
		, nullptr, false, &pChannel[(int)type]);

	// pSystem->playSound(FMOD_CHANNEL_FREE)
}

void SoundManager::Stop(int type)
{
	if (!isUsing) return;

	pChannel[(int)type]->stop();
}

void SoundManager::UpdateListenerPos(const Player* p)
{
	FMOD_VECTOR player_pos;
	FMOD_VECTOR player_up;
	FMOD_VECTOR player_look;
	FMOD_VECTOR player_velocity;

	player_pos.x = p->GetTransform().GetPosition().x;
	player_pos.y = p->GetTransform().GetPosition().y;
	player_pos.z = p->GetTransform().GetPosition().z;

	player_up.x = p->GetTransform().GetUp().x;
	player_up.y = p->GetTransform().GetUp().y;
	player_up.z = p->GetTransform().GetUp().z;

	player_look.x = p->GetTransform().GetLook().x;
	player_look.y = p->GetTransform().GetLook().y;
	player_look.z = p->GetTransform().GetLook().z;

	player_velocity.x = p->GetVelocity().x;
	player_velocity.y = p->GetVelocity().y;
	player_velocity.z = p->GetVelocity().z;

	pSystem->set3DListenerAttributes(0,
		&player_pos, &player_velocity, &player_look, &player_up);
}

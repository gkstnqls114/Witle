/*
set3DAttributes 리스너의 위치를 얻어오기 위한 함수

	매개변수
		pos: 소리 증감을 위해 리스너의 위치를 받아와야한다. 그것을 위한 매개변수. (3d공간상의 위치)
		vel: 3d 공간 fps의 속도
		alt_pan_pos: ?? 구현되지 않음
	반환값
		함수 성공: FMOD_OK
		함수 실패: FMOD_RESULT 열거 형에 정의 된 값 중 하나
*/

/*
비고

	거리 단위
		system::set3DSettings에 의해 지정. 기본 단위 Meters 'm,미터'
	스트레오 지정
		ChannelControl :: set3DSpread를 사용하여 오른쪽과 왼쪽의 사운드를 조절 할 수 있다.
*/

// 플레이어 현재 위치 가져오기
// PlayerManager::GetMainPlayer();

// 현재 위치 가져오기
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
	pChannel[SOUND_TYPE]->set3DAttributes((FMOD_VECTOR*)&PlayerManager::GetMainPlayer()->GetTransform().GetPosition(),
		(FMOD_VECTOR*)&PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), 0); // -> 인자 (pos,vel,alt_pan_pos)
	// pChannel[SOUND_TYPE]->setPriority(); // 
	pChannel[SOUND_TYPE]->setVolume(Volume); // -> 볼륨

	pChannel[SOUND_TYPE]->set3DMinMaxDistance(SOUND_MIN, SOUND_MAX);

	// 사운드 추가

	// 장면 //////////////////////////////////////////////////////////////////
	pSystem->createStream( // 메인
		"Sound/BGM/main_sound.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::MAIN_SOUND]
	);

	pSystem->createStream( // 스킬선택페이지 배경음
		"Sound/BGM/skillpage_sound.mp3"
		, FMOD_LOOP_NORMAL | FMOD_2D
		, nullptr
		, &pSound[(int)ENUM_SOUND::SKILLPAGE_SOUND]
	);

	pSystem->createStream( // 게임 배경음
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
	//4번 널값 들어가는거

	// 장면 //////////////////////////////////////////////////////////////////

	// 플레이어 //////////////////////////////////////////////////////////////////
	pSystem->createSound( // 플레이어 이동
		"Sound/Effect/player_move.mp3"
		, FMOD_LOOP_NORMAL | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_MOVE]
	);

	pSystem->createSound( // 플레이어 히트
		"Sound/Effect/player_damage.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_DAMAGE]
	);

	pSystem->createSound( // 플레이어 죽음
		"Sound/Effect/player_damage.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_DEAD]
	);

	pSystem->createSound( // 플레이어 빗자루
		"Sound/Effect/Broom.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_BROOM]
	);

	pSystem->createSound( // 플레이어 매직 미사일
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_MAGIC_MISIL]
	);
	// 9번 널값파일없음
	pSystem->createSound( // 플레이어 파이어볼
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_FIRE_SOUND]
	);

	pSystem->createSound( // 플레이어 아이스 볼
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_ICE_SOUND]
	);

	pSystem->createSound( // 플레이어 라이트닐 볼
		"Sound/Effect/magic_misil.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_LIGHT_SOUND]
	);

	pSystem->createSound( // 플레이어 블레싱
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_BLESSING_SOUND]
	);

	pSystem->createSound( // 플레이어 실드
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_SHIELD_SOUND]
	);

	pSystem->createSound( // 플레이어 힐링
		"Sound/Effect/shield.mp3"
		, FMOD_DEFAULT | FMOD_LOOP_OFF
		, nullptr
		, &pSound[(int)ENUM_SOUND::PLAYER_HEALING_SOUND]
	);
	// 플레이어 //////////////////////////////////////////////////////////////////

	// 기본 몬스터 //////////////////////////////////////////////////////////////////
	pSystem->createSound( // 기본 몬스터 이동
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::MONSTER_MOVE_SOUND]
	);

	pSystem->createSound( // 기본 몬스터 히트
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::MONSTER_DAMAGE_SOUND]
	);

	pSystem->createSound( // 기본 몬스터 죽음
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::MONSTER_DEAD_SOUND]
	);
	// 기본 몬스터 //////////////////////////////////////////////////////////////////

	// 보스 몬스터 //////////////////////////////////////////////////////////////////
	pSystem->createSound( // 보스 몬스터 이동
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_MOVE_SOUND]
	);

	pSystem->createSound( // 보스 몬스터 히트
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DAMAGE_SOUND]
	);

	pSystem->createSound( // 보스 몬스터 죽음
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DEAD_SOUND]
	);

	pSystem->createSound( // 보스 몬스터 브레스
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_BREATH_SOUND]
	);

	pSystem->createSound( // 보스 몬스터 다운스트록
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DOWNSTROKE_SOUND]
	);

	pSystem->createSound( // 보스 몬스터 테일어택
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_TAILATTACK_SOUND]
	);

	pSystem->createSound( // 보스 몬스터 대쉬
		"Sound/Effect/monster_move.mp3"
		, FMOD_DEFAULT | FMOD_3D
		, nullptr
		, &pSound[(int)ENUM_SOUND::BOSS_DASH_SOUND]
	);
	// 보스 몬스터 //////////////////////////////////////////////////////////////////
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

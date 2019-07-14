#pragma once

//// Animation Type /////////////////////////////////////////// 
#define ANIMATION_TYPE_ONCE			0
#define ANIMATION_TYPE_LOOP			1
#define ANIMATION_TYPE_PINGPONG		2

#define ANIMATION_CALLBACK_EPSILON	0.015f

//// Animation Type /////////////////////////////////////////// 

#define SECOND_PER_FRAME float(1.f/30.f) // 1 프레임당 몇초인가?

struct ANIMATION_INFO
{
	int ID{ -1 };
	float StartTime{ -1 };
	float EndTime{ -1 };
	int Type{ ANIMATION_TYPE_LOOP };
};

// Player ////////////////////////////////////
#define NUM_ANIMATIONE 11

const ANIMATION_INFO ANIMATION_IDLE{ 0, 0.f * SECOND_PER_FRAME,   30.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_FORWARD{ 1, 31.f * SECOND_PER_FRAME,  63.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_BACKWARD{ 2, 64.f * SECOND_PER_FRAME,  104.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_RIGHT{ 3, 105.f * SECOND_PER_FRAME, 137.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_LEFT{ 4, 138.f * SECOND_PER_FRAME, 170.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_ATTACK{ 5, 171.f * SECOND_PER_FRAME, 191.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_BROOMPREPARE{ 6, 192.f * SECOND_PER_FRAME, 230.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_BROOMIDLE{ 7, 231.f * SECOND_PER_FRAME, 261.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_BROOMFORWARD{ 8, 262.f * SECOND_PER_FRAME, 292.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_DEAD{ 9, 293.f * SECOND_PER_FRAME, 313.f * SECOND_PER_FRAME };
const ANIMATION_INFO ANIMATION_HIT{ 10, 314.f * SECOND_PER_FRAME, 334.f * SECOND_PER_FRAME };
// Player ////////////////////////////////////


// SpaceCat ////////////////////////////////////
#define SPACECAT_ANIMATIONE 5

const ANIMATION_INFO SPACECAT_IDLE{ 0, 0.f * SECOND_PER_FRAME,   30.f * SECOND_PER_FRAME };
const ANIMATION_INFO SPACECAT_MOVE{ 1, 31.f * SECOND_PER_FRAME,  60.f * SECOND_PER_FRAME };
const ANIMATION_INFO SPACECAT_ATTACK{ 2, 61.f * SECOND_PER_FRAME,  95.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
const ANIMATION_INFO SPACECAT_DEAD{ 3, 96.f * SECOND_PER_FRAME, 115.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
const ANIMATION_INFO SPACECAT_HIT{ 4, 116.f * SECOND_PER_FRAME, 135.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
// SpaceCat ////////////////////////////////////

// CreepyMonster ////////////////////////////////////
#define CREEPYMONSTER_ANIMATIONE 5

const ANIMATION_INFO CREEPYMONSTER_IDLE{ 0, 0.f * SECOND_PER_FRAME,   30.f * SECOND_PER_FRAME };
const ANIMATION_INFO CREEPYMONSTER_MOVE{ 1, 31.f * SECOND_PER_FRAME,  50.f * SECOND_PER_FRAME };
const ANIMATION_INFO CREEPYMONSTER_ATTACK{ 2, 51.f * SECOND_PER_FRAME,  88.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
const ANIMATION_INFO CREEPYMONSTER_DEAD{ 3, 89.f * SECOND_PER_FRAME, 110.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
const ANIMATION_INFO CREEPYMONSTER_HIT{ 4, 111.f * SECOND_PER_FRAME, 125.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
// CreepyMonster ////////////////////////////////////

// Boss ////////////////////////////////////
#define BOSSMONSTER_ANIMATIONE 15

const ANIMATION_INFO BOSS_IDLE{}; // 기본
const ANIMATION_INFO BOSS_MOVE{}; // 이동
const ANIMATION_INFO BOSS_CHASE{}; // 추격
const ANIMATION_INFO BOSS_SKILL0{};
const ANIMATION_INFO BOSS_SKILL1{};
const ANIMATION_INFO BOSS_SKILL2{};
const ANIMATION_INFO BOSS_SKILL3{};
const ANIMATION_INFO BOSS_SKILL4{};
const ANIMATION_INFO BOSS_SKILL5{};
const ANIMATION_INFO BOSS_SKILL6{};
const ANIMATION_INFO BOSS_SKILL7{};
const ANIMATION_INFO BOSS_SKILL8{};
const ANIMATION_INFO BOSS_SKILL9{};
const ANIMATION_INFO BOSS_DEAD{}; // 죽음
const ANIMATION_INFO BOSS_HIT{}; // 맞음
// Boss ////////////////////////////////////

// Altar ////////////////////////////////////
#define ALTARSPHERE_ANIMATIONE 2

const ANIMATION_INFO ALTARSPHERE_FLOW{ 0, 0.f * SECOND_PER_FRAME,   30.f * SECOND_PER_FRAME , ANIMATION_TYPE_ONCE };
const ANIMATION_INFO ALTARSPHERE_IDLE{ 1, 31.f * SECOND_PER_FRAME,  60.f * SECOND_PER_FRAME };
// Altar ////////////////////////////////////
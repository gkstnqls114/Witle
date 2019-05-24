#include "stdafx.h"
#include "MonsterMovement.h"
#include "Player.h"
#include "PlayerManager.h"
#include "ChaseAction.h"

void ChaseAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = 
		Vector3::Subtract(
			PlayerManager::GetMainPlayer()->GetTransform().GetPosition(),
			m_pOwner->GetTransform().GetPosition());
}

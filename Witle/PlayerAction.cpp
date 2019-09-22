#include "stdafx.h"
#include "Player.h"
#include "PlayerAction.h"

PlayerAction::PlayerAction(Player * pOwner, ENUM_PLAYER_ACTIONID actionID, ENUM_ANIMATIONID animeID)
	: Action(static_cast<GameObject*>(pOwner), animeID), m_ActionID(actionID) 
{
}

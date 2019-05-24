#pragma once

class Player;

// 현재 플레이어를 가리키는 포인터를 관리한다.
class PlayerManager
{
private:
	static Player* m_pMainPlayer; 

public:
	static void SetMainPlayer(Player* player) 
	{
		// 이미 플레이어가 설정되어있는데 다시 설정할 경우 오류
		assert(!(m_pMainPlayer != nullptr));
		m_pMainPlayer = player; 
	}
	static const Player* GetMainPlayer() { return m_pMainPlayer; }
};
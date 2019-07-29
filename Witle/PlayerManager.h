#pragma once

class Player;

// ���� �÷��̾ ����Ű�� �����͸� �����Ѵ�.
class PlayerManager
{
private:
	static Player* m_pMainPlayer; 

public:
	static void SetMainPlayer(Player* player) 
	{
		// �̹� �÷��̾ �����Ǿ��ִµ� �ٽ� ������ ��� ����
		assert(!(m_pMainPlayer != nullptr));
		m_pMainPlayer = player; 
	}
	static Player* GetMainPlayer() { return m_pMainPlayer; }

	static bool IsNearPlayer(const XMFLOAT3 pos, float distance);
};
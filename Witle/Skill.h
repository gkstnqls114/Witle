#pragma once

class Skill
{
	bool  m_isUsing        { false }; // ��ų�� ���� �������
	float m_CooldownTime  { 0.f }; // ���� ���ð�
	float m_SkillTime     { 0.f }; // ��ų ����ĭ

public:
	Skill();
	virtual ~Skill();

};
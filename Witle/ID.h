#pragma once


// ComponentID�� �����ϱ� ���� �ܼ��ϰ� ��Ӹ� ����
class EntityID : public std::string { 
public:
	EntityID(std::string s) : std::string(s) {};

};


// EntityID�� �����ϱ� ���� �ܼ��ϰ� ��Ӹ� ����
class ComponenetID : public std::string {
public:
	ComponenetID(std::string s) : std::string(s) {};
};

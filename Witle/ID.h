#pragma once


// ComponentID�� �����ϱ� ���� �ܼ��ϰ� ��Ӹ� ����
class EntityID : public std::string { 
public:
	EntityID(std::string s) : std::string(s) {};
	EntityID(const char *s) : std::string(s) {
#ifdef CONSOLE_TEST
		std::cout << s << std::endl; 
#endif // CONSOLE_TEST 
	};

};


// EntityID�� �����ϱ� ���� �ܼ��ϰ� ��Ӹ� ����
class ComponenetID : public std::string {
public:
	ComponenetID(std::string s) : std::string(s) {};
};
#pragma once


// ComponentID와 구분하기 위해 단순하게 상속만 진행
class EntityID : public std::string { 
public:
	EntityID(std::string s) : std::string(s) {};
	EntityID(const char *s) : std::string(s) {
#ifdef CHECK_CONSOLE_TEST
		std::cout << s << std::endl; 
#endif // CHECK_CONSOLE_TEST 
	};

};


// EntityID와 구분하기 위해 단순하게 상속만 진행
class ComponenetID : public std::string {
public:
	ComponenetID(std::string s) : std::string(s) {};
};

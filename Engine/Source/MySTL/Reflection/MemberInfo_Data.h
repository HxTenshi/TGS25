#pragma once

#include <string>
#include <functional>

//�o�^�p�̃����o�ϐ����
struct MemberInfo_Data{
public:
	using GetType = std::function < void*(void* post) >;
	using SetType = std::function < void(void*, void*) >;
	std::string typeName;
	std::string className;
	std::string memberName;
	GetType get;
	SetType set;
};

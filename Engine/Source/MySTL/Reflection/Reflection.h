#pragma once


#include <string>
#include <functional>
#include <map>

#include "MemberInfo_Data.h"


//�O���錾
struct Reflection;

//�����o�ϐ����
struct MemberInfo{
public:
	MemberInfo(void* post, MemberInfo_Data& data)
		:Post(post)
		, Data(data){}
	std::string GetName(){
		return Data.memberName;
	}
	std::string GetTypeName(){
		return Data.typeName;
	}

private:

	MemberInfo& operator =(const MemberInfo&);
	void* Post;
	MemberInfo_Data& Data;

	friend Reflection;
};
//�����o�ϐ����}�b�v
struct MemberInfos{
public:
	typedef std::map<std::string, MemberInfo_Data>::iterator iterator;

	MemberInfos(void* post, std::map<std::string, MemberInfo_Data>& data)
		:Post(post)
		, Data(data){}

	iterator begin(){
		return Data.begin();
	}
	iterator end(){
		return Data.end();
	}

private:
	MemberInfos& operator =(const MemberInfos&);

	void* Post;
	std::map<std::string, MemberInfo_Data>& Data;

	friend Reflection;
};



struct Reflection{

	//�N���X���ƕϐ������w�肵�ăN���X�̃����o�ϐ������擾
	static MemberInfo GetMemberInfo(void* post, const std::string& className, const std::string& memberName){
		return MemberInfo(post, (*map)[className][memberName]);
	}
	//�N���X���Ƃ��w�肵�ăN���X�̃����o�ϐ����}�b�v���擾
	static MemberInfos GetMemberInfos(void* post, const std::string& className){
		return MemberInfos(post, (*map)[className]);
	}
	//�����o�ϐ����}�b�v�ƃ}�b�v�v�f���w�肵�ăN���X�̃����o�ϐ������擾
	template<class Pair>
	static MemberInfo GetMemberInfo(MemberInfos& infos, Pair& info){
		return MemberInfo(infos.Post, info.second);
	}

	//�����o�ϐ���񂩂�ϐ����擾
	template<class T>
	static T* Get(MemberInfo& minfo){
		return (T*)minfo.Data.get(minfo.Post);
	}

	//�����o�ϐ���񂩂�ϐ���ݒ�
	template<class ValueType>
	static void Set(MemberInfo& minfo, ValueType&& Value){
		minfo.Data.set(minfo.Post, &Value);
	}

	static std::map<std::string, std::map<std::string, MemberInfo_Data>>* map;
private:
	Reflection();
};
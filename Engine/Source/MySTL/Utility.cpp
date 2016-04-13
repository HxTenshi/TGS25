#include "Utility.h"

//�Ō�Ɍ������������ʒu���O�����o��
std::string forward_than_find_last_of(const std::string& str, const std::string& findText){
	auto index = str.find_last_of(findText);
	if (index == str.npos){
		return "";
	}
	return str.substr(0, index);
}
//�Ō�Ɍ������������ʒu���������o��
std::string behind_than_find_last_of(const std::string& str, const std::string& findText){
	auto index = str.find_last_of(findText);
	if (index == str.npos){
		return "";
	}
	return str.substr(index + findText.length());
}
//�ŏ��Ɍ������������ʒu���O�����o��
std::string forward_than_find_first_of(const std::string& str, const std::string& findText){
	auto index = str.find_first_of(findText);
	if (index == str.npos){
		return "";
	}
	return str.substr(0, index);
}
//�ŏ��Ɍ������������ʒu���������o��
std::string behind_than_find_first_of(const std::string& str, const std::string& findText){
	auto index = str.find_first_of(findText);
	if (index == str.npos){
		return "";
	}
	return str.substr(index + findText.length());
}
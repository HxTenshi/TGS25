#pragma once


#include <unordered_map>
#include <functional>
#include "MySTL/ptr.h"

class Component;


class ComponentFactory{

public:
	//�N���X������R���|�[�l���g�쐬
	static
		shared_ptr<Component> Create(const std::string& ClassName);

	//�o�^����Ă���R���|�[�l���g�ꗗ���擾
	static
		std::unordered_map<std::string, std::function<shared_ptr<Component>()>>& GetComponents();
private:
	template<class T>
	static
		shared_ptr<Component> _Make(){
		return make_shared<T>();
	}

	template<class T>
	static
		void _NewFunc(){
		mFactoryComponents[typeid(T).name()] = &ComponentFactory::_Make<T>;
	}

	static
		shared_ptr<Component> _Create(std::string ClassName);

	//�o�^�R���|�[�l���g�ꗗ
	static
		std::unordered_map<std::string, std::function<shared_ptr<Component>()>> mFactoryComponents;

protected:
	ComponentFactory();
};
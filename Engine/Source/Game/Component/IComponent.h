#pragma once

#include <String>
#include "MySTL/File.h"
#include "MySTL/ioHelper.h"

class Actor;

class Component{
public:
	Component()
		:gameObject(NULL){

	}
	virtual ~Component(){

	}
	//�R���|�[�l���g���ʏ�����
	void _Initialize(Actor* obj){
		gameObject = obj;
	}
	//AddComponent��̏�����
	virtual void Initialize(){

	}
	//GameStart���̏�����
	virtual void Start(){

	}
	//�f�X�g���C���̏���
	virtual void Finish(){

	}
	virtual void EngineUpdate(){

	}
	virtual void Update(){

	}

	virtual void ChangeParentCallback(){

	}

#ifdef _ENGINE_MODE
	virtual void CreateInspector(){

	}
#endif

	void ExportClassName(File& f){
		std::string name = ClassName();
		f.Out(name);
	}
	std::string ClassName(){
		std::string name = typeid(*this).name();
		name.erase(0, 6);//"class "���폜
		return name;
	}
	virtual void IO_Data(I_ioHelper* io) = 0;

	Actor* gameObject;
};
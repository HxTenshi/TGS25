#pragma once

#include "IComponent.h"
#include "MySTL/ptr.h"

class ModelComponent;
class vmd;
class AnimationComponent :public Component{
public:
	AnimationComponent();
	void Initialize() override;
	void Finish() override;
	void Update() override;
	void CreateInspector() override;


	void IO_Data(I_ioHelper* io) override{
#define _KEY(x) io->func( x , #x)
		_KEY(mFileName);
#undef _KEY
	}

	weak_ptr<ModelComponent> mModel;

	//�A�j���[�V�����̍Đ��ʒu
	float mTime;
	//�A�j���[�V����VMD�t�@�C���̃p�X
	std::string mFileName;
	//�ǂݍ��񂾃t�@�C���f�[�^
	vmd* mVMD;
};
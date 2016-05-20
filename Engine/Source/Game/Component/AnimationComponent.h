#pragma once

#include "IComponent.h"
#include "MySTL/ptr.h"

#include "Graphic/Model/AnimeData.h"
#include "Graphic/Model/AnimationBind.h"


class BoneModel;
struct AnimeSet{

	~AnimeSet(){
		mAnimationBind = NULL;
		mAnimeData.Clear();
	}
	void Create(){
		mAnimationBind = NULL;
		if (!mFileName.empty()){
			mAnimeData.Create(mFileName);
		}
	}

	void Update(BoneModel* bone);
	//�A�j���[�V�����̍Đ��ʒu
	float mTime = 0.0f;
	//�A�j���[�V�����̍Đ����x
	float mTimeScale = 1.0f;
	//�A�j���[�V�����̃��[�v
	bool mLoop = true;
	//�A�j���[�V�����̃��[�v
	float mWeight = 0.0f;

	//�A�j���[�V����VMD�t�@�C���̃p�X
	std::string mFileName = "";

	AnimeData mAnimeData;
	shared_ptr<AnimationBind> mAnimationBind = NULL;
};


class ModelComponent;
//class vmd;
class AnimationComponent :public Component{
public:
	AnimationComponent();
	void Initialize() override;
	void Finish() override;
	void Update() override;
	void CreateInspector() override;


	void IO_Data(I_ioHelper* io) override;

	weak_ptr<ModelComponent> mModel;

	int mCurrentSet;
	AnimeSet mView;
	std::vector<AnimeSet> mAnimationSets;
};
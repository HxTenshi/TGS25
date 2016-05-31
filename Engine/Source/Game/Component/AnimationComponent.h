#pragma once

#include "IComponent.h"
#include "MySTL/ptr.h"

#include "Graphic/Model/AnimeData.h"
#include "Graphic/Model/AnimationBind.h"

struct AnimeParam{
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
};

class BoneModel;
struct AnimeSet{

	~AnimeSet(){
		mAnimationBind = NULL;
		mAnimeData.Clear();
	}
	void Create(){
		mAnimationBind = NULL;
		if (!Param.mFileName.empty()){
			mAnimeData.Create(Param.mFileName);
		}
	}

	void Update(BoneModel* bone);

	AnimeParam Param;

	AnimeData mAnimeData;
	shared_ptr<AnimationBind> mAnimationBind = NULL;
};

class IAnimationComponent{
public:
	virtual ~IAnimationComponent(){}
	virtual void ChangeAnimetion(int id) = 0;
	virtual AnimeParam GetAnimetionParam(int id) = 0;
	virtual void SetAnimetionParam(int id, const AnimeParam& param) = 0;
};

class ModelComponent;
//class vmd;
class AnimationComponent :public IAnimationComponent, public Component{
public:
	AnimationComponent();
	void Initialize() override;
	void Finish() override;
	void Update() override;
	void CreateInspector() override;

	void ChangeAnimetion(int id)override;
	AnimeParam GetAnimetionParam(int id) override;
	void SetAnimetionParam(int id,const AnimeParam& param) override;

	void IO_Data(I_ioHelper* io) override;

	weak_ptr<ModelComponent> mModel;

	int mCurrentSet;
	AnimeSet mView;
	std::vector<AnimeSet> mAnimationSets;
	int mAnimetionCapacity;
};
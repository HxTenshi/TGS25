#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "ModelStructs.h"

class AnimeData{
public:


	typedef std::vector<KeyFrame> Motion;
	typedef Motion::iterator KeyFrameIte;

	AnimeData()
		:mUseBoneNum(0)
		, mLastKeyNo(0)
	{
	}
	~AnimeData()
	{}

	//ID�̎擾
	int GetBoneID(const std::string& boneName){
		if (mBoneName2ID.count(boneName) == 0)return -1;
		return  mBoneName2ID[boneName].id;
	}
	//�{�[�����[�V�����̎擾
	Motion* GetBoneMotion(const std::string& boneName){
		int id = GetBoneID(boneName);
		if (id == -1)return NULL;
		return &mBoneMoution[id];
	}

	float GetLastFrameTime(){
		return mLastKeyNo;
	}

	void Clear(){
		mBoneMoution.clear();
		mBoneName2ID.clear();
		mUseBoneNum = 0;
		mLastKeyNo = 0;
	}

	void Create(const std::string& fileName){

		mBoneMoution.clear();
		mBoneName2ID.clear();
		mUseBoneNum = 0;
		mLastKeyNo = 0;

		vmd Anime(fileName.c_str());
		if (!Anime.mLoadResult)return;

		//�g�p����{�[���̐􂢏o��
		for (unsigned int i = 0; i < Anime.mKeyNum; i++){
			auto& key = Anime.mKeyFrame[i];
			//�{�[������ID�̊���U��
			SettingBoneID(key.BoneName);


			//�Ō�̃L�[�t���[�����A�j���[�V�����̏I���
			if (key.FrameNo > (unsigned int)mLastKeyNo){
				mLastKeyNo = key.FrameNo;
			}
		}

		//�g�p����{�[�������p��
		mBoneMoution.resize(mUseBoneNum);

		//�{�[�����ƂɃ��[�V�����o�^
		for (unsigned int i = 0; i < Anime.mKeyNum; i++){
			auto& key = Anime.mKeyFrame[i];
			PushBoneMotion(key);
		}

		//�{�[�����[�V�����̃\�[�g
		for (auto& motion : mBoneMoution){
			std::sort(motion.begin(), motion.end());
		}
	}

private:
	struct BoneId{
		int id = -1;
	};

	//�{�[������ID�̊���U��
	void SettingBoneID(const std::string& boneName){

		auto& boneid = mBoneName2ID[boneName];
		//�o�^�ς�
		if (boneid.id != -1){
			return;
		}

		boneid.id = mUseBoneNum;
		mUseBoneNum++;
	}


	void PushBoneMotion(const vmd::VMDKeyFrame& key){

		auto moution = GetBoneMotion(key.BoneName);

		moution->push_back(KeyFrame(key));

	}

	std::vector<Motion> mBoneMoution;	//�{�[�����̃L�[�t���[��
	std::unordered_map<std::string, BoneId>  mBoneName2ID;	//�{�[����ID�̊֘A�t��
	int mUseBoneNum;	//�g�p�{�[����
	int mLastKeyNo;		//�Ō�̃L�[�t���[��

};

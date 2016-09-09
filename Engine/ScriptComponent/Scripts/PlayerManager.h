
#pragma once
#include "main.h"
#include<vector>


class PlayerManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

	void CreditDown();
	void CreditUp();
	void ItemGet();
	// �T�E���h�{�b�N�X���Đ����܂�(�X�e�[�W)
	void StageSoundBox(const std::string name);
	// �T�E���h�{�b�N�X���Đ����܂�(�v���C���[)
	void PlayerSoundBox(const std::string name);
	// �w�肵���T�E���h�{�b�N�X���Đ����܂�
	void PlaySoundBox(Actor* soundBox, std::string name);
	// �t�H���g�̐����֐��ł�
	void CreateFontObj(const std::string name, Actor* obj);
	void FadeFontObj();
	bool IsGameStart();

private:
	void GameStart();
	void GameClear();
	void GameOver();
	void WingUI();
	bool IsClear();
	bool GameEnd();

private:
	float mAlpha;
	bool mGameStart;
	//�����o�ϐ�
	SERIALIZE
	int mCredit; //�v���C���[�̎c�@
	SERIALIZE
	int mMaxPoint; //�������A�C�e���̐�
	int mPoint;    //��������A�C�e���̐�
 	SERIALIZE
	float mFadeOutSecond;
	SERIALIZE
	float mFadeInSecond;
	SERIALIZE
	float mTitleChangeTime;
	float mGameStartTime;
	float mFadeAlpha;			// �t�F�[�h�̓����x
	float mStageNumberAlpha;	// �X�e�[�W�i���o�[�̓����l
	float mGatherAlpha;
	// �X�e�[�W��
	SERIALIZE
	std::string mStageName;
	SERIALIZE
	std::string mNextStageName;	// �N���A���̎��Ɉړ�����V�[����
	bool mIsWarning;
	bool mIsWarningSign;
	bool mIsFont;
	Actor* mFadeOutObj;
	Actor* mStageNumberObj;
	Actor* mGatherObj;
	Actor* mStageSoundBox;
	Actor* mPlayerSoundBox;
};
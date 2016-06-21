
#pragma once
#include "main.h"
#include <vector>

class Actor;

class SceneCursor :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �{�^���̔z��ɒǉ����܂�
	void AddButtonContainer(const char* buttonName);
	// �V�[���̔z��ɒǉ����܂�
	void AddSceneContainer(const std::string sceneName);
	// �����ꂽ�{�^���ɂ���ăV�[�����ړ����܂�
	void OnChangeScene();
	// �{�^���̒l���擾���܂�
	int GetButtonCount();
	// �V�[�����ς��������Ԃ��܂�
	bool IsChangeScene();

private:
	//�����o�ϐ�
	int mButtonCount;			// ���ݎw�肵�Ă���{�^���̒l
	SERIALIZE
	float mCursorScaleX;		// �J�[�\���̑傫��(X)
	SERIALIZE
	float mCursorScaleY;		// �J�[�\���̑傫��(Y)
	SERIALIZE
	float mAddCursorPositionX;	// �J�[�\���̈ʒu�����Z����(X)
	SERIALIZE
	float mAddCursorPositionY;	// �J�[�\���̈ʒu�����Z����(Y)
	bool mIsChangeScene;		// �V�[�����ς������
	Actor* mCursorRightTop;		// �E�㕔���̃J�[�\���I�u�W�F�N�g
	Actor* mCursorRightBottom;	// �E�������̃J�[�\���I�u�W�F�N�g
	Actor* mCursorLeftTop;		// ���㕔���̃J�[�\���I�u�W�F�N�g
	Actor* mCursorLeftBottom;	// ���������̃J�[�\���I�u�W�F�N�g
	// �{�^���R���e�i
	typedef std::vector<Actor*> ButtonContainer;
	ButtonContainer mButtonContainer;
	// �J�[�\���R���e�i
	typedef std::vector<Actor*> CursorContainer;
	CursorContainer mCursorContainer;
	// �J�[�\�����W�R���e�i
	typedef std::vector<XMVECTOR> CursorVelocityContainer;
	CursorVelocityContainer mCursorVelocityContainer;
	// �V�[���R���e�i
	typedef std::vector<std::string> SceneContainer;
	SceneContainer mSceneContainer;
};
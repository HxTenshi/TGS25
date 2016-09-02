#pragma once

class System{
public:
	virtual ~System(){}
	//�J�[�\�����E�B���h�̒��S�ɌŒ�
	virtual void LockCursorPositionToWindowCenter(bool flag) = 0;
	//�J�[�\���̃��b�N�ʒu
	virtual XMVECTOR GetLockCursorPosition() = 0;
	//�J�[�\���̕\��
	virtual void ShowCursor(bool flag) = 0;
	//�J�[�\������ʓ��ɕ����߂�
	virtual void ClipCursor(bool flag) = 0;
};

struct Cursor{
	bool mLockCursorPos;
	XMVECTOR mLockPos;
	bool mShowCursor;
	bool mClipCursor;
};

class SystemHelper : public System{
public:
	SystemHelper();
	~SystemHelper(){}
	//�J�[�\�����E�B���h�̒��S�ɌŒ�
	void LockCursorPositionToWindowCenter(bool flag) override;
	//�J�[�\���̃��b�N�ʒu
	XMVECTOR GetLockCursorPosition() override;
	//�J�[�\���̕\��
	void ShowCursor(bool flag) override;
	//�J�[�\������ʓ��ɕ����߂�
	void ClipCursor(bool flag) override;

	void Initialize();
	void Update();

private:
	Cursor mCursor;
};
/*
! @file movietex.h
! @bref	���[�r�[�e�N�X�`���N���X
! @author	Masafumi TAKAHASHI
*/

#ifndef MOVIETEX_H
#define MOVIETEX_H

#define _MOVETEX_ENABLE 0
#if _MOVETEX_ENABLE

#include <d3d11.h>

#pragma push_macro("new")
#undef new
#include <atlbase.h>
#include <stdio.h>
#include <Streams.h>
#include <dshow.h>
#pragma pop_macro("new")
#include <vector>


class Texture;

//-----------------------------------------------------------------------------
// Define GUID for Texture Renderer
// {71771540-2017-11cf-AE26-0020AFD79767}
//-----------------------------------------------------------------------------
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

//! @class CMovieTexture 
//! @brief �r�f�I��ID3D11Texture2D�ɏ������ޏ������s���N���X
class CMovieTexture : public CBaseVideoRenderer
{
	CComPtr<ID3D11Device>	m_pd3dDevice;
	CComPtr<ID3D11Texture2D>	m_pTexture;

	typedef std::vector<BYTE> RenderBuffer;
	RenderBuffer mRenderBuffer[2];
	int mLockBuffer;
	int mFrontBuffer;
	int mBackBuffer;

	//D3DFORMAT		m_TextureFormat;

public:
	CMovieTexture(LPUNKNOWN pUnk, HRESULT *phr);
	~CMovieTexture();

	VOID SetDevice(ID3D11Device * pd3dDevice){ m_pd3dDevice = pd3dDevice; };

	HRESULT CheckMediaType(const CMediaType *pmt);     // Format acceptable?
	HRESULT SetMediaType(const CMediaType *pmt);       // Video format notification
	HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample

	//! @fn ID3D11Resource * GetTexture()
	//! @brief ���[�r�[�e�N�X�`���̎擾
	//! @return ���[�r�[�������_�����O���ꂽ�e�N�X�`��
	ID3D11Texture2D * GetTexture(){ return m_pTexture; };

	void TextureRendering();

	//! @fn VOID GetVideoDesc(LONG* plVidWidth, LONG* plVidHeight, LONG* plVidPitch)
	//! @brief �r�f�I�̕��A�����A�s�b�`�̎擾
	//! @param *plVidWidth (out) �r�f�I�̕�
	//! @param *plVidHeigh (out) �r�f�I�̍�
	//! @param *plVidPitch (out) �r�f�I�̃s�b�`
	VOID GetVideoDesc(LONG* plVidWidth, LONG* plVidHeight, LONG* plVidPitch)
	{
		*plVidWidth = m_lVidWidth;
		*plVidHeight = m_lVidHeight;
		*plVidPitch = m_lVidPitch;
	};

	//! @param �_�C�i�~�b�N�e�N�X�`�����g�����ǂ����ǂ����̃t���O
	BOOL m_bUseDynamicTextures;
	//! @param �r�f�I�̕�
	LONG m_lVidWidth;
	//! @param �r�f�I�̍���
	LONG m_lVidHeight;
	//! @param �r�f�I�̃s�b�`
	LONG m_lVidPitch;
};

//! @class CMovieToTexture
//! @brief ���[�r�[�e�N�X�`���N���X
class CMovieToTexture
{
	CComPtr<ID3D11Device>       m_pd3dDevice;   // Our rendering device
	CComPtr<ID3D11Texture2D>      m_pTexture;     // Our texture

	CComPtr<IGraphBuilder>  m_pGB;          // GraphBuilder
	CComPtr<IMediaControl>  m_pMC;          // Media Control
	CComPtr<IMediaPosition> m_pMP;          // Media Position
	CComPtr<IMediaEvent>    m_pME;          // Media Event
	CComPtr<IBaseFilter>    m_pRenderer;    // our custom renderer
	CMovieTexture* mCMovieTexture;
	Texture *m_Texture;

	//! @param ��  
	LONG m_lWidth;
	//! @param ����
	LONG m_lHeight;
	//! @param �s�b�`
	LONG m_lPitch;

	//! @param ���̃��[�r�[�̃T�C�Y(���A����) / �e�N�X�`���̃T�C�Y(���A����)�ŎZ�o����UV�l
	FLOAT m_fu, m_fv;

	void CheckMovieStatus(void);
	void CleanupDShow(void);

public:
	CMovieToTexture();	//�R���X�g���N�^
	~CMovieToTexture();	//�f�X�g���N�^

	//! @fn HRESULT InitDShowTextureRenderer(WCHAR* wFileName, const BOOL bSound)
	//! @brief DirectShow����e�N�X�`���ւ̃����_�����O�ւ̏�����
	//! @param *pd3dDevice (in) Direct3D�f�o�C�X
	//! @param *wFileName (in) ���[�r�[�t�@�C���̃p�X
	//! @param bSound (in) �T�E���h�Đ��t���O
	//! @return �֐��̐���
	HRESULT InitDShowTextureRenderer(ID3D11Device * pd3dDevice, const WCHAR* wFileName, const BOOL bSound);

	//! @fn IDirect3DTexture9 * GetTexture()
	//! @brief ���[�r�[�e�N�X�`���̎擾
	//! @return ���[�r�[�������_�����O���ꂽ�e�N�X�`��
	Texture* GetTexture(){ return m_Texture; };

	void TextureRendering(){
		if (mCMovieTexture)mCMovieTexture->TextureRendering();
	}

	//! @fn VOID Play()
	//! @brief ���[�r�[�̍Đ�
	VOID Play(){ m_pMC->Run(); };

	//! @fn VOID Stop()
	//! @brief ���[�r�[�̒�~
	VOID Stop(){ m_pMC->Stop(); };

	//! @fn VOID SetSpeed(double time)
	//! @brief �Đ��X�s�[�h�̕ύX
	//! @param time (in) �Đ��X�s�[�h�̔{��
	VOID SetSpeed(double time){ m_pMP->put_Rate(time); };

	//! @fn double GetStopTime()
	//! @brief �I�����Ԃ̎擾
	//! @return �I������
	double GetStopTime()
	{
		REFTIME time;
		m_pMP->get_StopTime(&time);
		return time;
	};

	//! @fn double GetDuration()
	//! @brief �X�g���[���̎��ԕ��̎擾
	//! @return �S�X�g���[����
	double GetDuration()
	{
		REFTIME time;
		m_pMP->get_Duration(&time);
		return time;
	}

	//! @fn double GetCurrentPosition()
	//! @brief ���݂̍Đ��ʒu�̎擾
	//! @return ���݂̍Đ��ʒu
	double GetCurrentPosition()
	{
		REFTIME time;
		m_pMP->get_CurrentPosition(&time);
		return time;
	}

	//! @fn VOID SetTime(double time)
	//! @brief ���݂̍Đ��ʒu���w��ʒu�ɃZ�b�g
	//! @param time (in) �Z�b�g�������Đ��ʒu
	VOID SetTime(double time){ m_pMP->put_CurrentPosition(time); };

	//! @fn VOID GetUV(FLOAT* u, FLOAT* v)
	//! @brief �Đ�����e�N�X�`���̃T�C�Y�ƃ��[�r�[�̃T�C�Y������Ȃ��ꍇ�̗]�蕪���J�b�g���邽�߂�UV�l�擾
	//! @param *u (out) �e�N�X�`�����WU
	//! @param *v (out) �e�N�X�`�����WV
	VOID GetUV(FLOAT* u, FLOAT* v)
	{
		*u = m_fu;
		*v = m_fv;
	}

	//! @fn VOID GetEvent(long* lEventCode, long* lParam1, long* lParam2, long msTimeout)
	//! @brief ���[�r�[�̃C�x���g�擾
	//! @param lEventCode (out) �C�x���g �R�[�h���󂯎��ϐ��ւ̃|�C���^
	//! @param lParam1 (out) �� 1 �C�x���g�������󂯎��ϐ��ւ̃|�C���^�B
	//! @param lParam2 (out) �� 2 �C�x���g�������󂯎��ϐ��ւ̃|�C���^�B
	//! @param msTimeout (in) �^�C���A�E�g���� (�~���b�P��)�B�C�x���g����������܂œ�����~����ɂ́AINFINITE ���g���B
	VOID GetEvent(long* lEventCode, long* lParam1, long* lParam2, long msTimeout);
};


#else
class Texture;
class CMediaType;
class IMediaSample;
//-----------------------------------------------------------------------------
// Define GUID for Texture Renderer
// {71771540-2017-11cf-AE26-0020AFD79767}
//-----------------------------------------------------------------------------
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

//! @class CMovieTexture 
//! @brief �r�f�I��ID3D11Texture2D�ɏ������ޏ������s���N���X
class CMovieTexture
{

public:
	CMovieTexture(LPUNKNOWN pUnk, HRESULT *phr){}
	~CMovieTexture(){}

	VOID SetDevice(ID3D11Device * pd3dDevice){};

	HRESULT CheckMediaType(const CMediaType *pmt){ return E_FAIL; }     // Format acceptable?
	HRESULT SetMediaType(const CMediaType *pmt){ return E_FAIL; }       // Video format notification
	HRESULT DoRenderSample(IMediaSample *pMediaSample){ return E_FAIL; } // New video sample

	//! @fn ID3D11Resource * GetTexture()
	//! @brief ���[�r�[�e�N�X�`���̎擾
	//! @return ���[�r�[�������_�����O���ꂽ�e�N�X�`��
	ID3D11Texture2D * GetTexture(){};

	void TextureRendering();

	//! @fn VOID GetVideoDesc(LONG* plVidWidth, LONG* plVidHeight, LONG* plVidPitch)
	//! @brief �r�f�I�̕��A�����A�s�b�`�̎擾
	//! @param *plVidWidth (out) �r�f�I�̕�
	//! @param *plVidHeigh (out) �r�f�I�̍�
	//! @param *plVidPitch (out) �r�f�I�̃s�b�`
	VOID GetVideoDesc(LONG* plVidWidth, LONG* plVidHeight, LONG* plVidPitch)
	{
	};
};

//! @class CMovieToTexture
//! @brief ���[�r�[�e�N�X�`���N���X
class CMovieToTexture
{
	void CheckMovieStatus(void){}
	void CleanupDShow(void){}

public:
	CMovieToTexture(){}	//�R���X�g���N�^
	~CMovieToTexture(){}	//�f�X�g���N�^

	//! @fn HRESULT InitDShowTextureRenderer(WCHAR* wFileName, const BOOL bSound)
	//! @brief DirectShow����e�N�X�`���ւ̃����_�����O�ւ̏�����
	//! @param *pd3dDevice (in) Direct3D�f�o�C�X
	//! @param *wFileName (in) ���[�r�[�t�@�C���̃p�X
	//! @param bSound (in) �T�E���h�Đ��t���O
	//! @return �֐��̐���
	HRESULT InitDShowTextureRenderer(ID3D11Device * pd3dDevice, const WCHAR* wFileName, const BOOL bSound){ return E_FAIL; }

	//! @fn IDirect3DTexture9 * GetTexture()
	//! @brief ���[�r�[�e�N�X�`���̎擾
	//! @return ���[�r�[�������_�����O���ꂽ�e�N�X�`��
	Texture* GetTexture(){ return NULL; };

	void TextureRendering(){}

	//! @fn VOID Play()
	//! @brief ���[�r�[�̍Đ�
	VOID Play(){};

	//! @fn VOID Stop()
	//! @brief ���[�r�[�̒�~
	VOID Stop(){};

	//! @fn VOID SetSpeed(double time)
	//! @brief �Đ��X�s�[�h�̕ύX
	//! @param time (in) �Đ��X�s�[�h�̔{��
	VOID SetSpeed(double time){};

	//! @fn double GetStopTime()
	//! @brief �I�����Ԃ̎擾
	//! @return �I������
	double GetStopTime()
	{
		return 0.0;
	};

	//! @fn double GetDuration()
	//! @brief �X�g���[���̎��ԕ��̎擾
	//! @return �S�X�g���[����
	double GetDuration()
	{
		return 0.0;
	}

	//! @fn double GetCurrentPosition()
	//! @brief ���݂̍Đ��ʒu�̎擾
	//! @return ���݂̍Đ��ʒu
	double GetCurrentPosition()
	{
		return 0.0;
	}

	//! @fn VOID SetTime(double time)
	//! @brief ���݂̍Đ��ʒu���w��ʒu�ɃZ�b�g
	//! @param time (in) �Z�b�g�������Đ��ʒu
	VOID SetTime(double time){ };

	//! @fn VOID GetUV(FLOAT* u, FLOAT* v)
	//! @brief �Đ�����e�N�X�`���̃T�C�Y�ƃ��[�r�[�̃T�C�Y������Ȃ��ꍇ�̗]�蕪���J�b�g���邽�߂�UV�l�擾
	//! @param *u (out) �e�N�X�`�����WU
	//! @param *v (out) �e�N�X�`�����WV
	VOID GetUV(FLOAT* u, FLOAT* v)
	{
	}

	//! @fn VOID GetEvent(long* lEventCode, long* lParam1, long* lParam2, long msTimeout)
	//! @brief ���[�r�[�̃C�x���g�擾
	//! @param lEventCode (out) �C�x���g �R�[�h���󂯎��ϐ��ւ̃|�C���^
	//! @param lParam1 (out) �� 1 �C�x���g�������󂯎��ϐ��ւ̃|�C���^�B
	//! @param lParam2 (out) �� 2 �C�x���g�������󂯎��ϐ��ւ̃|�C���^�B
	//! @param msTimeout (in) �^�C���A�E�g���� (�~���b�P��)�B�C�x���g����������܂œ�����~����ɂ́AINFINITE ���g���B
	VOID GetEvent(long* lEventCode, long* lParam1, long* lParam2, long msTimeout){}
};
#endif




#endif // MOVIETEX_H
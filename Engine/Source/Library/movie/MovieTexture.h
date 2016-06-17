
/*
	! @file movietex.h
	! @bref	���[�r�[�e�N�X�`���N���X
	! @author	Masafumi TAKAHASHI
*/

#ifndef MOVIETEX_H
#define MOVIETEX_H

#include <tchar.h>
typedef TCHAR* PTCHAR;

// DirectX
#include <d3d9.h>
#include <d3dx9.h>

// DirectShow
#include <stdio.h>
#include "DirectShow/baseclasses/streams.h"
#include "DirectShow/common/smartptr.h"
#include <d3d9types.h>
#include <dshow.h>

// DirectShow Library
#ifdef _DEBUG
#pragma comment( lib, "DirectShow/strmbasd.lib" )
#else
#pragma comment( lib, "DirectShow/strmbase.lib" )
#endif

// Define GUID for Texture Renderer
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;


//-----------------------------------------------------------------------------
// �e�N�X�`���ɓ�����������ލ�Ƃ����ۂɍs�����߂̃N���X
// ���������ɗ��p�������
//-----------------------------------------------------------------------------
class TextureRenderer : public CBaseVideoRenderer
{
	SmartPtr<IDirect3DDevice9>	m_pd3dDevice;
	SmartPtr<IDirect3DTexture9>	m_pTexture;

	D3DFORMAT		m_TextureFormat;
 
	BOOL	m_bUseDynamicTextures;	//! @param �_�C�i�~�b�N�e�N�X�`�����g�����ǂ����ǂ����̃t���O

    LONG	m_lVidWidth;	//! @param �r�f�I�̕�
    LONG	m_lVidHeight;	//! @param �r�f�I�̍���
    LONG	m_lVidPitch;	//! @param �r�f�I�̃s�b�`



public:
    TextureRenderer(LPUNKNOWN pUnk,HRESULT *phr);
    ~TextureRenderer();
 
	void SetDevice(IDirect3DDevice9 * pd3dDevice){ m_pd3dDevice = pd3dDevice; };

	HRESULT CheckMediaType(const CMediaType *pmt );     // Format acceptable?
    HRESULT SetMediaType(const CMediaType *pmt );       // Video format notification
    HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample

	IDirect3DTexture9 * GetTexture(){ return m_pTexture; };

	void GetVideoDesc(LONG* plVidWidth, LONG* plVidHeight, LONG* plVidPitch)
	{
		*plVidWidth		= m_lVidWidth;
		*plVidHeight	= m_lVidHeight;
		*plVidPitch		= m_lVidPitch;
	};
};



//-----------------------------------------------------------------------------
// ����e�N�X�`������肾��������s���N���X
// �v���O���}�́A���̃N���X���g��
//-----------------------------------------------------------------------------
class MovieTexture
{
private:
	SmartPtr<IDirect3DDevice9>	m_pd3dDevice;   // Our rendering device
	SmartPtr<IDirect3DTexture9>	m_pTexture;     // Our texture

	SmartPtr<IGraphBuilder>		m_pGB;          // GraphBuilder
	SmartPtr<IMediaControl>		m_pMC;          // Media Control
	SmartPtr<IMediaPosition>	m_pMP;          // Media Position
	SmartPtr<IMediaEvent>		m_pME;          // Media Event

	SmartPtr<IBaseFilter>		m_pRenderer;    // our custom renderer
	
	LONG	m_lWidth;			//! @param ��  
	LONG	m_lHeight;			//! @param ����
	LONG	m_lPitch;			//! @param �s�b�`

	FLOAT	m_fu, m_fv;			//! @param ���̃��[�r�[�̃T�C�Y(���A����) / �e�N�X�`���̃T�C�Y(���A����)�ŎZ�o����UV�l

	bool	m_loop;



public:
	MovieTexture();			// �R���X�g���N�^
	MovieTexture( IDirect3DDevice9 * pd3dDevice, WCHAR* wFileName, BOOL en_sound=TRUE, BOOL begin_play=TRUE );		// �R���X�g���N�^
	~MovieTexture();		// �f�X�g���N�^

private:
	void	CheckMovieStatus( void );			// �i�����֐��j����̏�Ԃ𒲂ׂ�
	void	CleanupDShow( void );				// �i�����֐��j�j������

public:
	HRESULT	Create( IDirect3DDevice9 * pd3dDevice, WCHAR* wFileName, bool en_sound=true, bool loop=true, bool begin_play=true );	// �������֐�
	HRESULT	Init( IDirect3DDevice9 * pd3dDevice, WCHAR* wFileName, const BOOL bSound );	// �������֐�

	IDirect3DTexture9*	GetTexture();			// ���[�r�[�e�N�X�`���̎擾

	void	Play() { m_pMC->Run(); };			// ���[�r�[�̍Đ�
	void	Stop() { m_pMC->Stop(); };			// ���[�r�[�̒�~

	double	GetStopTime() { REFTIME time; m_pMP->get_StopTime(&time); return time; };	// �I�����Ԃ̎擾
	double	GetDuration() { REFTIME time; m_pMP->get_Duration(&time); return time; };	// �X�g���[���̎��ԕ��̎擾
	double	GetCurrentPosition() { REFTIME time; m_pMP->get_CurrentPosition(&time); return time; }	// ���݂̍Đ��ʒu�̎擾
	bool	IsLoop( void )	{ return m_loop; };

	void	SetTime( double time ) { m_pMP->put_CurrentPosition(time); };				// ���݂̍Đ��ʒu���w��ʒu�ɃZ�b�g
	void	SetSpeed( double time ) { m_pMP->put_Rate(time); };							// �Đ��X�s�[�h�̕ύX
	void	SetLoop( bool loop )	{ m_loop = loop; };

	void	GetUV(FLOAT* u, FLOAT* v) { *u = m_fu; *v = m_fv; };						// �Đ�����e�N�X�`���̃T�C�Y�ƃ��[�r�[�̃T�C�Y������Ȃ��ꍇ�̗]�蕪���J�b�g���邽�߂�UV�l�擾
	void	GetEvent( long* lEventCode, long* lParam1, long* lParam2, long msTimeout );	// ���[�r�[�̃C�x���g�擾

	bool	IsEndPos( void )	{ return GetCurrentPosition() >= GetStopTime(); };
};

#endif // MOVIETEX_H
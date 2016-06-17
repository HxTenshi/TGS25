
#pragma once

#include "Common.h"
#include "AnimationLinear.h"
#include "MovieTexture.h"


class	SampleMovie
{
// �����o�ϐ�
private:
	// ���f���̒��_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9	m_vb;

	// �e�N�X�`���摜�o�b�t�@
	LPDIRECT3DTEXTURE9		m_tex;

	// ���_�o�b�t�@�̍\���́i�e�N�X�`�����W���j
	struct  MY_VERTEX_VDT{
		D3DXVECTOR3 p;      // �ʒu
		DWORD       color;  // �F
		D3DXVECTOR2 t;      // �e�N�X�`���[���W
	};
	 
	// ���_�o�b�t�@�[�̃t�H�[�}�b�g�̒�`
	enum{ MY_VERTEX_VDT_FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	
	// �A�j���[�V����
	AnimationLinear		m_anim_rot;		// �t����]

	// ������e�N�X�`��
	MovieTexture		m_movie_tex;


// �R���X�g���N�^�E�f�X�g���N�^
public:
	SampleMovie();
	~SampleMovie();


// �����o�֐�
public:
	HRESULT	Create( LPDIRECT3DDEVICE9 d3d_dev );	// �V�[���̐���
	HRESULT	Destroy( void );						// �V�[���̔j��
	HRESULT	Transform( LPDIRECT3DDEVICE9 d3d_dev );	// ���W�ϊ�����
	HRESULT	Draw( LPDIRECT3DDEVICE9 d3d_dev );		// �V�[���̕`��
};
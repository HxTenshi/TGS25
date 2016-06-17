
#include "SampleMovie.h"


//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
SampleMovie::SampleMovie()
	: m_vb( NULL )
	, m_tex( NULL )
{
}



//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
SampleMovie::~SampleMovie()
{
	Destroy();
}



//----------------------------------------------------------
// �V�[���̐���
//----------------------------------------------------------
HRESULT SampleMovie::Create( LPDIRECT3DDEVICE9 d3d_dev )
{
	Destroy();
	if( d3d_dev == NULL )	return	E_FAIL;

	//----------------------------------------------------------
	// �����f���̐���
	//----------------------------------------------------------
	// ���f���̒��_�o�b�t�@�𐶐�
	if( FAILED( d3d_dev->CreateVertexBuffer(
			4 * sizeof(MY_VERTEX_VDT),  // ���_��
			D3DUSAGE_WRITEONLY,
			MY_VERTEX_VDT_FVF,          // ���_�f�[�^�萔�l
			D3DPOOL_MANAGED,
			&m_vb,                      // ���_�o�b�t�@�|�C���^�ւ̃A�h���X
			NULL ) ) )
	{
		return  E_FAIL;
	}

	// ���_�o�b�t�@���̃f�[�^��ݒ�
	MY_VERTEX_VDT   *v;

	// �ݒ���J�n
	m_vb->Lock( 0, 0, (void**)&v, 0 );
	 
	// �e���_�̈ʒu
	v[0].p = D3DXVECTOR3( -1.5f,  1.5f,  0.0f );
	v[1].p = D3DXVECTOR3(  1.5f,  1.5f,  0.0f );
	v[2].p = D3DXVECTOR3( -1.5f, -1.5f,  0.0f );
	v[3].p = D3DXVECTOR3(  1.5f, -1.5f,  0.0f );
	 
	// �e���_�̐F
	v[0].color = v[1].color = v[2].color = v[3].color 
		= D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
	 
	// �e�N�X�`�����W�̐ݒ�
	v[0].t = D3DXVECTOR2( 0.0f, 0.0f );
	v[1].t = D3DXVECTOR2( 1.0f, 0.0f );
	v[2].t = D3DXVECTOR2( 0.0f, 1.0f );
	v[3].t = D3DXVECTOR2( 1.0f, 1.0f );

	// �ݒ���I��
	m_vb->Unlock();

	//----------------------------------------------------------
	// ���e�N�X�`���̐���
	//----------------------------------------------------------
	// ������e�N�X�`���摜�̓ǂݍ���
	m_movie_tex.Create( d3d_dev, L"movie/E_Map_TYPE01a.avi", true, true, true );

	//----------------------------------------------------------
	// ���A�j���[�V�����̐ݒ�
	//----------------------------------------------------------
	// �t����]
	m_anim_rot.CreateAnimation( 3, timeGetTime(), true );
	m_anim_rot.SetKeyValue( 0, 0, -45.0f );
	m_anim_rot.SetKeyValue( 1, 2000, 45.0f );
	m_anim_rot.SetKeyValue( 2, 4000, -45.0f );


	return	S_OK;
}



//-------------------------------------------------------------
// �V�[���̔j��
//-------------------------------------------------------------
HRESULT	SampleMovie::Destroy()
{
	// ���_�o�b�t�@�̉��
	SAFE_RELEASE( m_vb );
	SAFE_RELEASE( m_tex );

	return	S_OK;
}



//-------------------------------------------------------------
// ���W�ϊ�
//-------------------------------------------------------------
HRESULT	SampleMovie::Transform( LPDIRECT3DDEVICE9 d3d_dev )
{
	// ���f���̔z�u
	D3DXMATRIX  mat_world;
	D3DXMATRIX  mat_trans, mat_scale, mat_rot_y;
	D3DXMATRIX  mat_mul;

	float	angle	= D3DXToRadian( m_anim_rot.GetValue( timeGetTime() ) );

	D3DXMatrixIdentity( &mat_world );
	D3DXMatrixTranslation( &mat_trans, 0.0f, 0.0f, 0.0f );
	D3DXMatrixScaling( &mat_scale, 1.6f, 0.9f, 1.0f );
	D3DXMatrixRotationY( &mat_rot_y, angle );
	D3DXMatrixMultiply( &mat_mul, &mat_scale, &mat_rot_y );
	D3DXMatrixMultiply( &mat_world, &mat_mul, &mat_trans );

	d3d_dev->SetTransform( D3DTS_WORLD, &mat_world );

	return	S_OK;
}



//-------------------------------------------------------------
// �I�u�W�F�N�g���̕`��
//-------------------------------------------------------------
HRESULT	SampleMovie::Draw( LPDIRECT3DDEVICE9 d3d_dev )
{
	if( d3d_dev == NULL )	return	E_FAIL;
	if( m_vb == NULL )		return	E_FAIL;

	// �e�N�X�`���̐ݒ�
	// �e�N�X�`���X�e�[�W�̐ݒ� 
	d3d_dev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	d3d_dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    d3d_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	// ���W�ϊ�
	Transform( d3d_dev );

	// ���쐬�����e�N�X�`�����p�C�v���C���ɃZ�b�g
	d3d_dev->SetTexture( 0, m_movie_tex.GetTexture() );

	// ���_�o�b�t�@�̕`��
	d3d_dev->SetStreamSource( 0, m_vb, 0, sizeof(MY_VERTEX_VDT) );
	d3d_dev->SetFVF( MY_VERTEX_VDT_FVF );
	d3d_dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	// �e�N�X�`���������Z�b�g
	d3d_dev->SetTexture( 0, NULL );

	return	S_OK;
}

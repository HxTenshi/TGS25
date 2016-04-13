//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register(s0);

cbuffer cbNeverChanges : register(b0)
{
	matrix View;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
};

cbuffer CBBillboard  : register(b8)
{
	matrix mBillboardMatrix;
};

// �W�I���g���V�F�[�_�[�̓��̓p�����[�^
struct VS_IN
{
	float3 pos : POSITION;  // ���W
	float3 v0  : NORMAL;    // �����x
	float time : TEXCOORD;  // ����
};

typedef VS_IN VS_OUT;

// ���_�V�F�[�_�[�ł͂��̂܂܏o��
VS_OUT VS0_Main(VS_IN In)
{
	return In;
}

typedef VS_OUT GS_IN;
typedef GS_IN GS0_OUT;

[maxvertexcount(6)]   // �W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
// �W�I���g���V�F�[�_�[
void GS0_Main(point GS_IN In[1],                   // �|�C���g �v���~�e�B�u�̓��͏��
	inout PointStream<GS0_OUT> PStream   // �|�C���g �v���~�e�B�u�̏o�̓X�g���[��
	)
{
	GS0_OUT Out;

	float g_LimitTime = 10.0f;

	// �p�[�e�B�N���̎��Ԃ� g_LimitTime �𒴂����̂ŏ���������
	if (In[0].time > g_LimitTime)
	{
		Out.pos = float3(0, 0, 0);
		Out.v0 = In[0].v0;
		Out.time = 0;
	}
	else
	{
		// X �ʒu = �����x �~ ����
		Out.pos.x = In[0].v0.x * In[0].time;

		// ���R�����^���̌����ɂ��AY ���W���v�Z����B
		// 9.80665f �͏d�͉����x�B
		Out.pos.y = -0.1f * 9.80665f * In[0].time * In[0].time + In[0].v0.y * In[0].time;

		// Z �ʒu = �����x �~ ����
		Out.pos.z = In[0].v0.z * In[0].time;

		Out.v0 = In[0].v0;

		// ���Ԃ�i�߂�
		Out.time = In[0].time + 0.1f;
	}

	PStream.Append(Out);
	PStream.RestartStrip();
}

struct GS1_OUT
{
	float4 pos    : SV_POSITION;  // �p�[�e�B�N���̈ʒu
	float4 color  : COLOR;        // �p�[�e�B�N���̐F
	float2 texel  : TEXCOORD0;    // �e�N�Z��
};

[maxvertexcount(4)]   // �W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
// �W�I���g���V�F�[�_�[
void GS1_Main(point GS_IN In[1],                       // �|�C���g �v���~�e�B�u�̓��͏��
	inout TriangleStream<GS1_OUT> TriStream  // �g���C�A���O�� �v���~�e�B�u�̏o�̓X�g���[��
	)
{
	GS1_OUT Out;

	float g_LimitTime = 10.0f;
	float g_Scale = 0.2f;

	// ���Ԃ� 0.0f �` g_LimitTime �͈͓̔��ŕω�����
	float c = 1.0f - In[0].time / g_LimitTime;

	// ���_�J���[�́A���Ԃ̌o�߂ɔ������X�ɉ��F����Ԃ��ۂ��������ɂȂ�悤�ɂ��Ă���B
	float4 color = float4(1, c * c, 0, c);


	matrix matWVP = mBillboardMatrix;
	matWVP._41_42_43_44 = World._14_24_34_44;
	matWVP = mul(matWVP, View);
	matWVP = mul(matWVP, Projection);
	//matrix matWVP = World;

	Out.pos = mul(float4(In[0].pos.x + g_Scale, In[0].pos.y + g_Scale, In[0].pos.z, 1.0f), matWVP);
	Out.color = color;
	Out.texel = float2(1, 0);
	TriStream.Append(Out);

	Out.pos = mul(float4(In[0].pos.x - g_Scale, In[0].pos.y + g_Scale, In[0].pos.z, 1.0f), matWVP);
	Out.color = color;
	Out.texel = float2(0, 0);
	TriStream.Append(Out);

	Out.pos = mul(float4(In[0].pos.x + g_Scale, In[0].pos.y - g_Scale, In[0].pos.z, 1.0f), matWVP);
	Out.color = color;
	Out.texel = float2(1, 1);
	TriStream.Append(Out);

	Out.pos = mul(float4(In[0].pos.x - g_Scale, In[0].pos.y - g_Scale, In[0].pos.z, 1.0f), matWVP);
	Out.color = color;
	Out.texel = float2(0, 1);
	TriStream.Append(Out);

	TriStream.RestartStrip();
}

typedef GS1_OUT PS_IN;

// �s�N�Z���V�F�[�_
float4 PS1_Main(PS_IN In) : SV_TARGET
{
	return txDiffuse.Sample(samLinear, In.texel) * In.color;
	//return float4(In.color.xyz,1);
}
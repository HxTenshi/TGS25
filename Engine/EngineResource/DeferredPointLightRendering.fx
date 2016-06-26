//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D NormalTex : register( t0 );
SamplerState NormalSamLinear : register(s0);
Texture2D DepthTex : register(t1);
SamplerState DepthSamLinear : register(s1);
Texture2D SpecularTex : register(t2);
SamplerState SpecularSamLinear : register(s2);

cbuffer cbNeverChanges : register(b0)
{
	matrix View;
	matrix ViewInv;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
	matrix ProjectionInv;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
};

cbuffer cbChangesPointLight : register(b8)
{
	float4 PtLVPos;
	float4 PtLCol;
	float4 PtLPrm;//������ = Prm.x*r^Prm.w + Prm.y, Prm.z = �e�����a
};

cbuffer cbNearFar : register(b12)
{
	float Near;
	float Far;
	float2 NULLnf;
};

cbuffer cbScreen : register(b13)
{
	float2 ScreenSize;
	float2 NULLss;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos		: POSITION;
	//float3 Normal	: NORMAL;
	//float2 Tex		: TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float4 VPos		: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.VPos = mul(output.Pos, View);
	output.Pos = mul(output.VPos, Projection);

	return output;
}
struct PS_OUTPUT_1
{
	float4 Diffuse : SV_Target0;
	float4 Specular : SV_Target1;
};


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

//projection depth 0~1
float3 DepthToPosition2(float2 textureCoord, float depth)
{
	float3 vpos;
	// �X�N���[�����W�@����(-1,+1) �E��(+1,-1)
	float2 spos = textureCoord * float2(2, -2) + float2(-1, 1);
	vpos.z = Projection._m23 / (depth/*- Projection._m22*/);
	vpos.xy = (spos.xy*vpos.z) / float2(Projection._m00, Projection._m11);

	return vpos;
}
//view depth
float3 DepthToPosition(float2 textureCoord, float depth)
{
	depth *= Far;
	float4 projectedPosition = float4(textureCoord.xy * float2(2, -2) + float2(-1, 1), 0.0, 1.0);

	float3 viewPosition = mul(projectedPosition, ProjectionInv).xyz;
	float3 viewRay = float3(viewPosition.xy / viewPosition.z, 1.0);
	return viewRay * depth;
}

//view depth 0~1
float ToPerspectiveDepth(float viewDepth)
{
	float4 projectedPosition = mul(float4(0, 0, viewDepth, 1.0), Projection);
		return 1.0 - (projectedPosition.z / projectedPosition.w);
}
//view depth 0~1
float ToPerspectiveDepth2(float viewDepth)
{

	///@note Left-hand coordinate system
	float a = Far / (Far - Near);
	float b = (Near * Far) / (Near - Far);
	float z = viewDepth * a + b;
	float w = viewDepth;
	return 1.0 - (z / w);
}


// V ���v�Z���邽�߂֗̕��֐�
float G1V(float dotNV, float k)
{
	return 1.0f / (dotNV * (1.0f - k) + k);
}

// GGX ���g�����X�y�L�����V�F�[�f�B���O�̒l��Ԃ��֐�
float LightingFuncGGX_REF(float3 N, float3 V, float3 L,
	// ���t�l�X
	float roughness,
	// �t���l�����˗� F(0��)
	float F0)
{
	// �� = ���t�l�X^2
	float alpha = roughness * roughness;

	// �n�[�t�x�N�g��
	float3 H = normalize(V + L);

		// �x�N�g���̓��ϊ֘A
		float dotNL = saturate(dot(N, L));
	float dotNV = saturate(dot(N, V));
	float dotNH = saturate(dot(N, H));
	float dotLH = saturate(dot(L, H));

	float F, D, vis;

	// D
	float alphaSqr = alpha * alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH * (alphaSqr - 1.0) + 1.0f;
	D = alphaSqr / (pi * denom * denom);

	// F
	float dotLH5 = pow(1.0f - dotLH, 5);
	F = F0 + (1.0 - F0) * (dotLH5);

	// V
	float k = alpha / 2.0f;
	vis = G1V(dotNL, k) * G1V(dotNV, k);

	float specular = dotNL * D * F * vis;

	return specular;
}


PS_OUTPUT_1 main(PS_INPUT input,float normalVec){

	PS_OUTPUT_1 Out;
	Out.Diffuse = float4(0, 0, 0, 0);
	Out.Specular = float4(0, 0, 0, 0);

	//�W�I���g���o�b�t�@�̃e�N�X�`�����W�i�@���A�f�v�X�l�擾�p�j
	float2 tex = input.Pos.xy / ScreenSize;


		float4 norCol = NormalTex.Sample(NormalSamLinear, tex);
		// �@��(view���W�n) 0~1��-1�`+1�ɕϊ�
		float3 nor = norCol.xyz * 2 - 1;
		nor *= normalVec;
		if (nor.x == 0 && nor.y == 0 && nor.z == 0){
		return Out;
		}

	// �ʒu(view���W�n)
	float dep = DepthTex.Sample(DepthSamLinear, tex).r;
	//dep = ToPerspectiveDepth(dep);
	float3 vpos = DepthToPosition(tex, dep);


		//float3 lpos = mul(PtLVPos, View).xyz;
		//lpos.xyz = lpos.w;
		float3 lpos = PtLVPos.xyz;

		//�_�����܂ł̋���
		float3 dir = lpos - vpos;
		float r = length(dir);

	// �����̕���(view���W�n)
	dir = dir / r;

	// ����  A*r^n + B
	float atte = saturate(PtLPrm.x*pow(r, PtLPrm.w) + PtLPrm.y);

	//float n = 1;//�C�ӂ̒l(��0)�@�����Ȑ��𐧌�
	//float rmin = 0;// �����J�n�����@������ = 1
	//float rmax = 2.5;// �e���͈́@������ = 0
	//float _rmin = pow(rmin, n);
	//float _rmax = pow(rmax, n);
	//float A = -1 / (_rmax - _rmin);
	//float B = _rmax / (_rmax - _rmin);
	//
	//atte = saturate(A*pow(r, n) + B);

	// �g�U
	float df = saturate(dot(nor, dir));

	// ����
	//float3 h = normalize(dir - normalize(vpos));//�n�[�t�x�N�g��
	//float spec_pow = 255 * txSpecPow.Sample(samSpecPow, tex).x;
	//float3 spec = pow(saturate(dot(nor, h)), spec_pow);
	float roughness = norCol.a - 1;
	roughness = max(roughness, 0.1f);
	float spec = LightingFuncGGX_REF(nor, -normalize(vpos), dir, roughness, 1);

	Out.Diffuse = float4(atte*df*PtLCol.xyz, 1);
	Out.Specular = float4(atte*spec*PtLCol.xyz, 1);
	//Out.rgb += atte*spec*PtLCol.xyz;

	return Out;

}


PS_OUTPUT_1 PS(PS_INPUT input)
{
	PS_OUTPUT_1 Out;
	Out = main(input,1);

	float2 tex = input.Pos.xy / ScreenSize;
	float rebirth = SpecularTex.Sample(SpecularSamLinear, tex).a;

	//[branch]
	if (rebirth > 0.01){
		PS_OUTPUT_1 reb;
		reb = main(input, -1);
		Out.Diffuse.rgb = ((1 - rebirth) * Out.Diffuse.rgb) + reb.Diffuse.rgb * rebirth;
		Out.Specular.rgb = ((1 - rebirth) * Out.Specular.rgb) + reb.Specular.rgb * rebirth;
	}
	else{

	}

	return Out;
}
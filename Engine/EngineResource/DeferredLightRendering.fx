//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D NormalTex : register( t0 );
SamplerState NormalSamLinear : register(s0);
Texture2D DepthTex : register(t1);
SamplerState DepthSamLinear : register(s1);
Texture2D SpecularTex : register(t2);
SamplerState SpecularSamLinear : register(s2);

Texture2D LightDepthTex1 : register(t3);
SamplerState LightDepthSamLinear1 : register(s3);
Texture2D LightDepthTex2 : register(t4);
SamplerState LightDepthSamLinear2 : register(s4);
Texture2D LightDepthTex3 : register(t5);
SamplerState LightDepthSamLinear3 : register(s5);
Texture2D LightDepthTex4 : register(t6);
SamplerState LightDepthSamLinear4 : register(s6);

cbuffer cbNeverChanges : register(b0)
{
	matrix View;
};
cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
	matrix ProjectionInv;
};
cbuffer cbChangesLight : register(b3)
{
	float4 LightVect;
	float4 LightColor;
};

cbuffer cbChangesLightCamera : register(b10)
{
	matrix LViewProjection[4];
	float4 SplitPosition;
};

cbuffer cbNearFar : register(b12)
{
	float Near;
	float Far;
	float2 NULLnf;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos		: POSITION;
	float2 Tex		: TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	output.Pos = input.Pos;
	output.Tex = input.Tex;
	
	return output;
}
struct PS_OUTPUT_1
{
	float4 Diffuse : SV_Target0;
	float4 Specular : SV_Target1;
};


//projection depth 0~1
inline float3 DepthToPosition2(float2 textureCoord, float depth)
{
	float3 vpos;
	// �X�N���[�����W�@����(-1,+1) �E��(+1,-1)
	float2 spos = textureCoord * float2(2, -2) + float2(-1, 1);
		vpos.z = Projection._m23 / (depth/* - Projection._m22*/);
	vpos.xy = (spos.xy*vpos.z) / float2(Projection._m00, Projection._m11);

	return vpos;
}

//view depth 0~1
inline float ToPerspectiveDepth(float viewDepth)
{
	float4 projectedPosition = mul(float4(0, 0, viewDepth, 1.0), Projection);
	return 1.0 - (projectedPosition.z / projectedPosition.w);
}

// V ���v�Z���邽�߂֗̕��֐�
inline float G1V(float dotNV, float k)
{
	return 1.0f / (dotNV * (1.0f - k) + k);
}

const static float PI = 3.141592653589793f;

// GGX ���g�����X�y�L�����V�F�[�f�B���O�̒l��Ԃ��֐�
inline float LightingFuncGGX_REF(float3 N, float3 V, float3 L,
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
	float denom = dotNH * dotNH * (alphaSqr - 1.0) + 1.0f;
	D = alphaSqr / (PI * denom * denom);

	// F
	float dotLH5 = pow(1.0f - dotLH, 5);
	F = F0 + (1.0 - F0) * (dotLH5);

	// V
	float k = alpha / 2.0f;
	vis = G1V(dotNL, k) * G1V(dotNV, k);

	float specular = dotNL * D * F * vis;

	return specular;
}



inline PS_OUTPUT_1 main(PS_INPUT input,float normalVec){
	PS_OUTPUT_1 Out;
	Out.Diffuse = float4(1, 1, 1, 1);
	Out.Specular = float4(0, 0, 0, 1);

	float4 norCol = NormalTex.Sample(NormalSamLinear, input.Tex);
		// �@���̏���
		float3 N = norCol.xyz;
		N = N * 2 - 1;
	N *= normalVec;

	if (N.x == 0 && N.y == 0 && N.z == 0){
		return Out;
	}

	// ���C�g�̃x�N�g��
	float3 L = LightVect.xyz;
		L = mul(L, (float3x3)View).xyz;
	L = normalize(L);


	float NLDot = dot(N, -L);


	float offset = 0.000000035;// +(NLDot)* 0.00000005;
	// ���C�g�f�v�X�̏���
	float4 LVPos = DepthTex.Sample(DepthSamLinear, input.Tex).zwyx;

	// �ʒu(view���W�n)
	float dep = LVPos.w;
	dep = ToPerspectiveDepth(dep);
	float3 vpos = DepthToPosition2(input.Tex, dep);

		//float4 debugColor = float4(0, 0, 0, 1);

	float dist = vpos.z * 10;
	float LD = 0;
	if (dist < SplitPosition.x)
	{
		LD = LightDepthTex1.Sample(LightDepthSamLinear1, LVPos.xy).x;
	}
	else if (dist < SplitPosition.y)
	{
		LD = LightDepthTex2.Sample(LightDepthSamLinear2, LVPos.xy).x;
	}
	else if (dist < SplitPosition.z)
	{
		LD = LightDepthTex3.Sample(LightDepthSamLinear3, LVPos.xy).x;
	}
	else
	{
		LD = LightDepthTex4.Sample(LightDepthSamLinear4, LVPos.xy).x;
	}

	float shadow = step(LD, LVPos.z + offset);

	// �f�B�t���[�Y�p�x�������v�Z
	float DifGen = saturate(NLDot);
	DifGen *= shadow * 0.75;
	DifGen = DifGen + 0.25;


	float roughness = norCol.a - 1;
	float spec = LightingFuncGGX_REF(N, -normalize(vpos), -L, roughness, 0.1);

	float r = 0.75f;
	float g = 0.83f;
	float b = 0.95f;
	if (shadow == 1){
		Out.Diffuse = LightColor * (DifGen* float4(1 - r, 1 - g, 1 - b, 1) + float4(r, g, b, 1));
	}
	else{
		Out.Diffuse = LightColor * float4(r, g, b, 1);
	}
	//Out.Diffuse = LightColor * DifGen;
	Out.Diffuse.a = LightColor.a;

	Out.Specular = LightColor * spec;
	Out.Specular.a = LightColor.a;

	return Out;

}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT_1 PS(PS_INPUT input)
{
	PS_OUTPUT_1 Out;
	Out = main(input,1);

	float rebirth = SpecularTex.Sample(SpecularSamLinear, input.Tex).a;

	//[branch]
	if (rebirth > 0.1){
		PS_OUTPUT_1 reb;
		reb = main(input,-1);
		Out.Diffuse.rgb += reb.Diffuse.rgb * rebirth;
		Out.Specular.rgb += reb.Specular.rgb * rebirth;
	}
	else{

	}

	return Out;
}

//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D AlbedoTex : register( t0 );
SamplerState AlbedoSamLinear : register(s0);
Texture2D NormalTex : register(t1);
SamplerState NormalSamLinear : register(s1);
Texture2D HeightTex : register(t2);
SamplerState HeightSamLinear : register(s2);

cbuffer cbNeverChanges : register( b0 )
{
	matrix View;
	matrix ViewInv;
};

cbuffer cbChangeOnResize : register( b1 )
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
	matrix World;
	matrix BeforeWorld;
};

cbuffer cbChangesLight : register( b3 )
{
	float4 LightVect;
	float4 LightColor;
};
cbuffer cbChangesMaterial : register( b4 )
{
	float4 MDiffuse;
	float4 MSpecular;
	float4 MAmbient;
	float2 MTexScale;
	float2 MHightPower;
};
cbuffer cbChangesLightMaterial : register( b5 )
{
	float4 LDiffuse;
	float4 LSpecular;
	float4 LAmbient;
};

cbuffer cbChangesUseTexture : register( b6 )
{
	float4 UseTexture;
};
cbuffer cbBoneMatrix : register(b7)
{
	//float4x3 BoneMatrix[153];
	float4x3 BoneMatrix[2] :  packoffset(c0);
	float4x3 BoneMatrixDmmy[253] :  packoffset(c6);
	float4x3 BoneMatrixEnd :  packoffset(c765);
}

cbuffer cbChangesLightCamera : register(b10)
{
	matrix LViewProjection[4];
	float4 SplitPosition;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float3 Bin	: BINORMAL;
	float3 Tan	: TANGENT;
	float2 Tex		: TEXCOORD0;
	uint4 BoneIdx		: BONEINDEX;
	uint4 BoneWeight	: BONEWEIGHT;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD2;
	float4 VPos		: TEXCOORD3;
};

struct PS_OUTPUT_1
{
	float4 ColorAlbedo : SV_Target0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.VPos = mul( output.Pos, View );
	output.Pos = mul( output.VPos, Projection);

	output.Tex = input.Tex * MTexScale;
	
	return output;
}



float4x3 getBoneMatrix(int idx)
{
	return BoneMatrix[idx];
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VSSkin(VS_INPUT input)
{

	float4 pos = input.Pos;


	float3 bpos = float3(0, 0, 0);
	[unroll]
	for (uint i = 0; i < 4; i++){
		float4x3 bm = getBoneMatrix(input.BoneIdx[i]);
		bpos += input.BoneWeight[i] * mul(pos, bm).xyz;
	}

	pos.xyz = bpos / 100.0;



	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(pos, World);

	output.VPos = mul(output.Pos, View);
	output.Pos = mul(output.VPos, Projection);


	output.Tex = input.Tex;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT_1 PS(PS_INPUT input)
{
	PS_OUTPUT_1 Out;

	float2 texcood = input.Tex;
	float4 DifColor = float4(1.0, 1.0, 1.0, 1.0);
		if (UseTexture.x != 0.0)
			DifColor = AlbedoTex.Sample(AlbedoSamLinear, texcood);

	Out.ColorAlbedo = DifColor * MDiffuse;
	return Out;
}

//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D PostTex : register(t0);
SamplerState PostSamLinear : register(s0);
Texture2D ScreenTex : register(t1);
SamplerState ScreenSamLinear : register(s1);
Texture2D AlbedoTex : register(t2);
SamplerState AlbedoSamLinear : register(s2);
Texture2D SpecularTex : register(t3);
SamplerState SpecularSamLinear : register(s3);
Texture2D NormalTex : register(t4);
SamplerState NormalSamLinear : register(s4);
Texture2D DepthTex : register(t5);
SamplerState DepthSamLinear : register(s5);
Texture2D LightTex : register(t6);
SamplerState LightSamLinear : register(s6);
Texture2D LightSpeTex : register(t7);
SamplerState LightSpeSamLinear : register(s7);
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
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos;
	output.Tex = input.Tex;
	
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{

	float dim_size = 0.42f;
	float dim_strength = 0.38;
	float aberration = 0.005;

	float2 delta = input.Tex*2 - 1;
	float dist = length(delta);

	float2 t = delta*float2(1, 0);
	float2 rtex = t * -aberration;
	float2 btex = t * aberration;

	float pr = ScreenTex.Sample(ScreenSamLinear, saturate(input.Tex + rtex)).r;
	float pg = ScreenTex.Sample(ScreenSamLinear, input.Tex).g;
	float pb = ScreenTex.Sample(ScreenSamLinear, saturate(input.Tex + btex)).b;

	float dim = 1.0;
	if (dist > dim_size)
	{
		dim = 1.0 - (dist - dim_size) / (dim_size) * dim_strength;
	}

	float4 col = float4(pr,pg,pb,1);
	col.rgb *= pow(dim,0.75);
	return col;
}

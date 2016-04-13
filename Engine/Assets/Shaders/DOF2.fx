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

	float4 col = ScreenTex.Sample(ScreenSamLinear, input.Tex);
	float count = 1;


	float depth = DepthTex.Sample(DepthSamLinear, input.Tex).r;
	float centerDepth = DepthTex.Sample(DepthSamLinear, float2(0.5,0.5)).r;
	if (depth == 0){
		depth = 1;
	}
	if (centerDepth == 0){
		centerDepth = 1;
	}

	float c = 0.02 / 2;
	float d = depth;
	d = d - centerDepth;
	if (d > c || d < -c){
		d = abs(d) - c;
		d = d * 100;
		d = min(d, 3);
		[unrool]
		for (float i = 0; i < 10; i++){

			float s = d * i;

			float x = s / 1200;
			float y = s / 800;

			col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(0, y));
			col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(0, -y));

			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(x, y));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(-x, y));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(x, -y));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(-x, -y));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(0, y));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(0, -y));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(x, 0));
			//col += ScreenTex.Sample(ScreenSamLinear, input.Tex + float2(-x, 0));
			count += 2;
		}
		col /= count;

	}

	col.a = 1;
	return col;
}

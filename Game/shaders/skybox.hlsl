struct VS_INPUT
{
	float3 pos : POSITION;
}

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 dir : TEXCOORD0;
}

cbuffer cb_frameData : register(b2)
{
	row_major float4x4 viewProjection;

	float4 cameraPos;
	float4 cameraDir;

	float4 dirLightColor;
	float4 dirLightDir;
}


TextureCube textureSkybox : register(t0);

SamplerState samplerLinear : register(s0);


PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;

	output.pos = mul(float4(input.pos, 0.0F), viewProjection).xyww;

	output.dir = input.pos;

	return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
	return textureSkybox.Sample(samplerLinear, input.dir);
}

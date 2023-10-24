struct VS_INPUT
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	uint4 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR;
};



cbuffer cb_objectData : register(b0)
{
	row_major float4x4 world;
};

cbuffer cb_materialData : register(b1)
{
	float4 albedo;
	float specularPower;
	float specularIntensity;
};

cbuffer cb_frameData : register(b2)
{
	row_major float4x4 viewProjection;
};

Texture2D textureDiffuse : register(t0);

SamplerState samplerLinear : register(s0);



PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;

	output.pos = float4(input.pos, 1.0F);

	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, viewProjection);

	output.texCoord = input.texCoord;

	output.color = input.color / 255.0F;

	return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
	float4 finalColor = input.color;

	finalColor *= textureDiffuse.Sample(samplerLinear, input.texCoord);

	return finalColor;
}

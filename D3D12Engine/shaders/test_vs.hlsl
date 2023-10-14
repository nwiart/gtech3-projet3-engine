struct VS_INPUT
{
	float3 pos : POSITION;
	uint4 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

cbuffer cb_frameData : register(b0)
{
	row_major float4x4 viewProjection;
};

cbuffer cb_objectData : register(b1)
{
	row_major float4x4 world;
};


PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;

	output.pos = float4(input.pos, 1.0F);

	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, viewProjection);

	output.color = input.color / 255.0F;

	return output;
}

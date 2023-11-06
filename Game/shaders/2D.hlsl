struct VS_INPUT
{
	float2 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	uint4 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR;
};

cbuffer generic : register(b1)
{
    row_major float4x4 projection;
};

cbuffer dimension : register(b0)
{
    float2 position;
    float2 size;
};

Texture2D textureDiffuse : register(t0);

SamplerState samplerLinear : register(s0);



PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;

	output.pos = float4(input.pos, 0, 1.0F);

    output.pos.xy *= size;
    output.pos.xy += position;
	output.pos = mul(output.pos, projection);

	output.texCoord = input.texCoord;

	output.color = input.color / 255.0F;
	
	return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
	float4 finalColor = float4(0.0F, 0.0F, 0.0F, 1.0F);
	
    finalColor = input.color;
	float4 albedo = textureDiffuse.Sample(samplerLinear, input.texCoord);
	

	return finalColor;
}

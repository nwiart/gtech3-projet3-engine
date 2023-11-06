struct VS_INPUT
{
	float3 pos : POSITION0;
	float2 texCoord : TEXCOORD0;
	float3 particlePos : POSITION1;
	float2 particleSize : TEXCOORD1;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float2 texCoord : TEXCOORD;
};



cbuffer cbFrameData : register(b1)
{
	float4x4 view;
	float4x4 projection;
};

// Maximum number of particles in a single draw call.
#define MAX_PARTICLES 64

/*cbuffer cbParticleData : register(b0)
{
	float4 particlePositions[MAX_PARTICLES];
	float2 particleSizes[MAX_PARTICLES];
};*/

Texture2D textureDiffuse : register(t0);

SamplerState samplerLinear : register(s0);



PS_INPUT vs_main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	PS_INPUT output;
	
	//output.pos = mul(float4(particlePositions[instanceID].xyz, 1.0F), view);
	//output.pos.xy += input.pos.xy * particleSizes[instanceID];
	//output.pos = mul(output.pos, projection);
	
	output.pos = mul(float4(input.particlePos, 1.0F), view);
	output.pos.xy += input.pos.xy * input.particleSize;
	output.pos = mul(output.pos, projection);
	
	output.texCoord = input.texCoord;
	
	return output;
}

float4 ps_main(PS_INPUT input) : SV_Target
{
	float4 color = float4(1, 1, 1, 1); //textureDiffuse.Sample(samplerLinear, input.texCoord);
	//if (color.a == 0.0F)
		//discard;
	
	return color;
}

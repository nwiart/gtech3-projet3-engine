struct VS_INPUT
{
	// Quad geometry.
	float3 pos : POSITION0;
	float2 texCoord : TEXCOORD0;
	
	// Per-particle data.
	float3 particlePos : POSITION1;
	float2 particleSize : TEXCOORD1;
	float4 particleColor : COLOR1;
	float4 particleRotation : TEXCOORD2;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float2 texCoord : TEXCOORD;
	
	float4 particleColor : COLOR1;
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
	
	float2x2 rotationMatrix = float2x2(input.particleRotation.xy, input.particleRotation.zw);
	
	output.pos = mul(float4(input.particlePos, 1.0F), view);
	output.pos.xy += mul(input.pos.xy * input.particleSize, rotationMatrix);;
	output.pos = mul(output.pos, projection);
	
	output.texCoord = input.texCoord;
	
	output.particleColor = input.particleColor;
	
	return output;
}

float4 ps_main(PS_INPUT input) : SV_Target
{
	float4 color = textureDiffuse.Sample(samplerLinear, input.texCoord);
	color *= input.particleColor;
	
	if (color.a == 0.0F)
		discard;
	
	return color;
}

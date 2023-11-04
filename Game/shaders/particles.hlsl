struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD;
};


// Maximum number of particles in a single draw call.
#define MAX_PARTICLES 64


cbuffer cbFrameData : register(b2)
{
    
};

cbuffer cbParticleData : register(b0)
{
    float4 particlePositions[MAX_PARTICLES];
    float2 particleSizes[MAX_PARTICLES];
};



PS_INPUT vs_main(VS_INPUT input, int instanceID : SV_InstanceID)
{
    PS_INPUT output;
    
    output.pos = mul(float4(particlePositions[instanceID].xyz, 1.0F), view);
    output.pos.xy += input.pos.xy * particleSizes[instanceID];
    output.pos = mul(output.pos, projection);
    
    output.texCoord = input.texCoord;
    
    return output;
}

float4 ps_main(PS_INPUT input) : SV_Target
{
    return float4(1, 1, 1, 1);
}

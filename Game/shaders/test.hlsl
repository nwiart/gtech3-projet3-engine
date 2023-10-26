struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	uint4 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR;
	
    float3 pixelWorldPos : TEXCOORD1;
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
	
    float4 cameraPos;
    float4 cameraDir;
	
    float4 dirLightColor;
    float4 dirLightDir;
};

Texture2D textureDiffuse : register(t0);

SamplerState samplerLinear : register(s0);



PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;

	output.pos = float4(input.pos, 1.0F);

	output.pos = mul(output.pos, world);
    output.pixelWorldPos = output.pos.xyz;
	output.pos = mul(output.pos, viewProjection);

    float4 rotatedNormal = mul(float4(input.normal, 0.0F), world);
    output.normal = normalize(rotatedNormal.xyz);

	output.texCoord = input.texCoord;

	output.color = input.color / 255.0F;

	return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
	float4 finalColor = float4(0.0F, 0.0F, 0.0F, 1.0F);

    float3 normal = normalize(input.normal);
	
	float4 albedo = textureDiffuse.Sample(samplerLinear, input.texCoord);
	finalColor += albedo;

    
    float brightness = max(0.2F, dot(normal, -dirLightDir));
	finalColor.rgb *= brightness * dirLightColor.rgb;
	
    float3 V = normalize(cameraPos - input.pixelWorldPos).xyz;
    float3 R = reflect(dirLightDir, normal);
	float3 F = pow(1- dot(normal, V),2) * 0.6F;



    finalColor.rgb += pow(max(0.0F, dot(R, V)), 10.0F) * 0.5F * dirLightColor.rgb;
	finalColor.rgb += F;

	return finalColor;
}

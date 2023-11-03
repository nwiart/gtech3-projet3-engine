#define MAX_POINT_LIGHT 8

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

	float4 ambientColor;
	float4 dirLightColor;
	float4 dirLightDir;
	
    float4 PointLightPos[MAX_POINT_LIGHT];
    float4 PointLightColor[MAX_POINT_LIGHT];
    float4 PointLightAttenuation[MAX_POINT_LIGHT];

	
};

Texture2D textureDiffuse : register(t0);

SamplerState samplerLinear : register(s0);



void calcPointLightDiffuse(inout float3 color, float3 pixelWorldPos, float3 normal, float3 pointLightPos, float3 pointLightColor)
{
    float3 pxToLight = pointLightPos - pixelWorldPos;
    float dist = length(pxToLight);
    pxToLight = normalize(pxToLight);
	
    float b = max(0, dot(normal, pxToLight)) / (dist * dist);
	
    color += pointLightColor * b;
}

void calcPointLightSpecular(inout float3 color, float3 V, float3 pixelWorldPos, float3 normal, float3 pointLightPos, float3 pointLightColor)
{
    float3 lightToPx = pixelWorldPos - pointLightPos;
    float dist = length(lightToPx);
    lightToPx = normalize(lightToPx);
	
    float3 R = reflect(lightToPx, normal);
	
    float s = pow(max(0.0F, dot(R, V)), 10.0F) * 0.5F / (dist * dist);
	
	color += pointLightColor * s;
}



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

	// Directional light diffuse component.
	float brightness = max(0.0F, dot(normal, -dirLightDir.xyz));
	finalColor.rgb += brightness * dirLightColor.rgb;
	finalColor.rgb += ambientColor.rgb;
	
	// Point light diffuse.
    float3 pl = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
        calcPointLightDiffuse(pl, input.pixelWorldPos, normal, PointLightPos[i].xyz, PointLightColor[i].xyz);	
    }
	finalColor.rgb += pl;
	
    finalColor.rgb *= albedo.rgb;


	float3 V = normalize(cameraPos.xyz - input.pixelWorldPos).xyz;

	// Directional light specular.
	float3 R = reflect(dirLightDir.xyz, normal);
	finalColor.rgb += pow(max(0.0F, dot(R, V)), 10.0F) * 0.5F * dirLightColor.rgb;
	
    pl = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
        calcPointLightSpecular(pl, V, input.pixelWorldPos, normal, PointLightPos[i].xyz, PointLightColor[i].xyz);
    }
    finalColor.rgb += pl;
	
	
	// Fresnel.
    float3 F = pow(1 - dot(normal, V), 3) * 0.4F;
	finalColor.rgb += F;
	

	return finalColor;
}

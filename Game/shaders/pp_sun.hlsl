struct VS_INPUT
{
	float2 pos : POSITION;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 pixelScreenPos : TEXCOORD0;
	float4 dirLightScreenPos : TEXCOORD1;
};



cbuffer cb_data : register(b0)
{
	float4x4 viewProjection;
	float4 dirLightDirection;
};



PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;

	output.pos = float4(input.pos, 0.0F, 1.0F);
	output.pixelScreenPos = input.pos;
	
	output.dirLightScreenPos = mul(float4(-dirLightDirection.xyz, 0.0F), viewProjection);	
	output.dirLightScreenPos.xy /= output.dirLightScreenPos.w;

	return output;
}


float4 ps_main(PS_INPUT input) : SV_TARGET
{
	if (input.dirLightScreenPos.z < 0.0F)
		discard;
	
	float2 uv_step = float2(1.0F / 32.0F, 1.0F / 32.0F);

	float intensity = 0.0F;

	const int RADIUS = 3;
	
	[unroll(7)]
	for (float y = -RADIUS; y <= RADIUS; ++y) {
		[unroll(7)]
		for (float x = -RADIUS; x <= RADIUS; ++x) {
			float2 screenPos = input.dirLightScreenPos.xy + float2(x, y) * uv_step;
			if (screenPos.x >= -1.0F && screenPos.x <= 1.0F && screenPos.y >= -1.0F && screenPos.y <= 1.0F) {
				intensity += 1.0F;
			}
		}
	}

	intensity /= (RADIUS * 2 + 1) * (RADIUS * 2 + 1);

	float dist = length((input.dirLightScreenPos.xy - input.pixelScreenPos) * float2(16.0F / 9.0F, 1.0F)) * 4.0F;
	float a = intensity / dist * 0.5F;

	return float4(float3(a, a, a) * float3(1.0F, 0.9F, 0.8F), 1.0F);
}

cbuffer perLightChange : register(b0) {
	float3 Strength;
	float FalloffStart;
	float3 Direction;
	float FalloffEnd;
	float3 Position; 
	float SpotPower; 
};

struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

float4 main(PSInput pin) : SV_TARGET {
	float ambientStrength = 0.1f;
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float3 ambient = ambientStrength * lightColor;

	return float4(Position, 1.0f);
}

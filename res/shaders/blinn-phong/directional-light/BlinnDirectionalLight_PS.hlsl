#include "Common.hlsli"

cbuffer perLightChange : register(b0) {
    Light dirLight;
};

struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

float4 main(PSInput pin) : SV_TARGET {
    // Ambient
	float ambientStrength = 0.1f;
	float3 ambient = dirLight.Color * ambientStrength;

    // Diffuse
    float diffuseFactor = dot(normalize(pin.Normal), -dirLight.Direction);
	float3 diffuse = float3(0, 0, 0);
	if (diffuseFactor > 0) {
		diffuse = dirLight.Color * diffuseFactor * dirLight.Intensity;
	}

	return float4(ambient + diffuse, 1.0f);
}

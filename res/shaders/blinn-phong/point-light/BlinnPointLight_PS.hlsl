#include "Common.hlsli"

cbuffer perLightChange : register(b0) {
    Light pointLight;
};

struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

float4 main(PSInput pin) : SV_TARGET {
    // Ambient
	float ambientStrength = 0.1f;
	float3 ambient = ambientStrength * pointLight.Color;

    // Diffuse
    float3 fragPos = float3(pin.Position.x, pin.Position.y, pin.Position.z);
    float3 lightDir = normalize(pointLight.Position - fragPos);
    float diff = max(dot(pin.Normal, lightDir), 0.0);
    float3 diffuse = diff * pointLight.Color;

    float3 result = ambient + diffuse;
    return float4(result, 1.0f);
}

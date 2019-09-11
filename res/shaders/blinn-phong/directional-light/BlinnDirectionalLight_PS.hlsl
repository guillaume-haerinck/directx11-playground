#include "Common.hlsli"

cbuffer perLightChange : register(b0) {
    Light dirLight;
};

struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 CameraWorldPos : POSITION0;
	float3 WorldPos : POSITION1;
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

	// Specular
	// FIXME decals
	float specularStrength = 1.0f;
	float3 specular = float3(0, 0, 0);
	if (diffuseFactor > 0) {
		float3 vertexToCamera = normalize(pin.CameraWorldPos - pin.WorldPos);
		float3 lightReflect = normalize(reflect(dirLight.Direction, pin.Normal));
		float specularFactor = dot(vertexToCamera, lightReflect);

		if (specularFactor > 0) {
			specularFactor = pow(specularFactor, specularStrength);
			specular = dirLight.Color * dirLight.Intensity * specularFactor;
		}
	}

	// return float4(ambient + diffuse + specular, 1.0f);
	return float4(specular, 1.0f);
}

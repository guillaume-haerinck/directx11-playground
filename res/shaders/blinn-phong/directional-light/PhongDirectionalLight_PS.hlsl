#include "Common.hlsli"

Texture2D tex : register(t0);
SamplerState samplr : register(s0);

cbuffer perLightChange : register(b0) {
    Light dirLight;
};

cbuffer perPropertyChange : register(b1) {
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularAttenuation;
}

struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	float3 CameraWorldPos : POSITION0;
	float3 WorldPos : POSITION1;
};

float4 main(PSInput pin) : SV_TARGET {
	float3 diffuse = float3(0, 0, 0);
	float3 specular = float3(0, 0, 0);

    // Ambient
	float ambientStrength = 0.1f;
    float3 ambient = dirLight.Color * ambientStrength * ambientIntensity;

    // Diffuse
    float diffuseFactor = dot(normalize(pin.Normal), -dirLight.Direction);
	if (diffuseFactor > 0) {
        diffuse = dirLight.Color * diffuseFactor * diffuseIntensity;

		// Specular
		float3 vertexToCamera = normalize(pin.CameraWorldPos - pin.WorldPos);
		float3 lightReflect = normalize(reflect(dirLight.Direction, pin.Normal));
		float specularFactor = dot(vertexToCamera, lightReflect);

		if (specularFactor > 0) {
			specularFactor = pow(specularFactor, specularAttenuation);
            specular = dirLight.Color * specularFactor * specularIntensity;
        }
	}

    return tex.Sample(samplr, pin.TexCoord) * float4(ambient + diffuse + specular, 1.0f) * dirLight.Intensity;
}

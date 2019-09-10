struct PSInput {
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 main(PSInput pin) : SV_TARGET {
	float ambientStrength = 0.1;
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float3 ambient = ambientStrength * lightColor;

	return float4(ambient, 1.0f);
}

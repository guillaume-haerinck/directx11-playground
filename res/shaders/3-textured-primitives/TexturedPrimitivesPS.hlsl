cbuffer cbPerFrame : register(b0) {
	float4 faceColor[6];
};

struct PSInput {
	uint tid : SV_PrimitiveID;
};

float4 main(PSInput pin) : SV_TARGET
{
	// return faceColor[pin.tid / 2];
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

cbuffer cbPerFrame : register(b0) {
	float4x4 matVP;
	float4x4 matGeo;
};

struct VSInput {
	float3 Position : POSITION;
};

float4 main(VSInput vin) : SV_POSITION {
	return mul(mul(float4(vin.Position, 1.0f), matGeo), matVP);
}

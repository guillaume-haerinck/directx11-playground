cbuffer cbPerFrame : register(b0) {
	float4x4 matVP;
	float4x4 matGeo;
};

struct VSInput {
	float3 Position : POSITION;
	uint InstanceID : SV_InstanceID;
};

struct VSOutput {
	float4 Position : SV_POSITION;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	vout.Position = mul(mul(float4(vin.Position, 1.0f), matGeo), matVP);
	vout.Position.x += vin.InstanceID;
	return vout;
}

cbuffer cbCamera : register(b0) {
	float4x4 matViewProj;
};

cbuffer cbMeshVariable : register(b1) {
	float4x4 matModel;
}

struct VSInput {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VSOutput {
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	
	vout.Position = mul(mul(float4(vin.Position, 1.0f), matModel), matViewProj);
	// vout.Position.x += vin.InstanceID * 3;

	vout.TexCoord = vin.TexCoord;

	return vout;
}

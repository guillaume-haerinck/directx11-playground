cbuffer perMesh : register(b0) {
	float4x4 matModel;
}

cbuffer perFrame : register(b1) {
	float4x4 matViewProj;
};

struct VSInput {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	// float4 Tangent : TANGENT;
};

struct VSOutput {
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(mul(float4(vin.Position, 1.0f), matModel), matViewProj);
	vout.TexCoord = vin.TexCoord;
	
	return vout;
}

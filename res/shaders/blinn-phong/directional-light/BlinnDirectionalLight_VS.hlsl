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
};

struct VSOutput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	vout.Position = mul(mul(float4(vin.Position, 1.0f), matModel), matViewProj);
	vout.Normal = mul(float4(vin.Normal, 1.0f), matModel); // We suppose that the model matrix is orthogonal (if scale, it does not skew)
	return vout;
}

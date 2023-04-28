cbuffer ConstData:register(b0)
{
	float4 color;
	float2 uv;
	matrix mat[1000];
};

struct VS_Input
{
	float4 pos:POSITION;
	float2 uv:TEXCOORD;

	uint InstanceId:SV_InstanceID;
};

struct BasicType {
	float4 svpos:SV_POSITION;
	float2 uv:TEXCOORD;
};

BasicType main(VS_Input vs) {
	BasicType output;
	output.svpos = mul(mat[vs.InstanceId], vs.pos);
	output.uv = vs.uv;
	return output;
}
struct EnemyUnitShaderData {
	matrix mat;
	float rate;
};

cbuffer ConstData:register(b0)
{
	float4 color;
	float2 uv;
	EnemyUnitShaderData Unit[800];
};

struct VS_Input
{
	float4 pos:POSITION;
	float2 uv:TEXCOORD;

	uint InstanceId:SV_InstanceID;
};

struct BasicType {
	float4 svpos:SV_POSITION;
	float3 uv:TEXCOORD;
};

BasicType main(VS_Input vs) {
	BasicType output;
	output.svpos = mul(Unit[vs.InstanceId].mat, vs.pos);
	output.uv.xy = vs.uv.xy;
	output.uv.z = vs.InstanceId;
	return output;
}
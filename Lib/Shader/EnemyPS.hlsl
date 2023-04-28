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

struct BasicType
{
	float4 svpos:SV_POSITION;
	float3 uv:TEXCOORD;
};

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 main(BasicType input) :SV_TARGET{
	float2 texuv = input.uv.xy;
	float4 ret = float4(tex.Sample(smp, texuv));
	float r = Unit[input.uv.z].rate;
	/*ret = (ret * (1.0f - r)) + (float4(1, 0, 0, ret.a) * r);*/

	return ret;
}
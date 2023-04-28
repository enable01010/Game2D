struct BasicType
{
	float4 svpos:SV_POSITION;
	float2 uv:TEXCOORD;
};

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 main(BasicType input) :SV_TARGET{
	float2 texuv = input.uv;
	return float4(tex.Sample(smp, texuv));
}
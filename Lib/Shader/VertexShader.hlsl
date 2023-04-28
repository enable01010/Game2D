#include"Header.hlsli"

struct VS_Input
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	uint InstanceId : SV_InstanceID;
};

BasicType main(VS_Input vs) {
	BasicType output;//�s�N�Z���V�F�[�_�֓n���l
	output.svpos = mul(mat, vs.pos);
	output.svpos.x += ((vs.InstanceId) % 30) * 0.05f;
	output.svpos.y += ((vs.InstanceId) / 30) * 0.05f;
	output.uv = vs.uv;
	return output;
}

// ------------------------
//
// FxPS.hlsl
// �G�t�F�N�g�p�̃s�N�Z���V�F�[�_�[
// �ݒ��VS�Q�l�ɁB
//
// ------------------------
#include"Fx_Header.hlsli"

float4 main(BasicType input) : SV_TARGET{
	float2 texuv = input.uv;
	float4 ret = float4(tex.Sample(smp, texuv));
	return ret;
}
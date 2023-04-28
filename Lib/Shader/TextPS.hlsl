
struct BasicType {
	float4 svpos:SV_POSITION;//�V�X�e���p���_���W
	float2 uv:TEXCOORD;//UV�l
};

Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v��
Buffer<uint> srcCommand : register(t2);

float4 main(BasicType input) : SV_TARGET{
	float2 texuv = input.uv ;
	float4 ret = float4(tex.Sample(smp, texuv));
	return ret;
}
// ------------------------
//
// Fx_Header.hlsli(�Ō��i������̂Œ��ӁB�V�K�쐬�ŁuHLSL�w�b�_�[�t�@�C���v��I��)
// �G�t�F�N�g�p�̃s�N�Z���V�F�[�_�[
//
// ------------------------
Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v��


// �萔�o�b�t�@
cbuffer ConstData : register(b0)
{
	float4 color;
	matrix mat[100];// �e�̈ʒu�������Ă�
};

struct BasicType {
	float4 svpos:SV_POSITION;//�V�X�e���p���_���W
	float2 uv:TEXCOORD;//UV�l
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	uint InstanceId : SV_InstanceID;// 
};

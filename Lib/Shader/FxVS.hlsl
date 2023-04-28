// ------------------------
//
// FxVS.hlsl
// �G�t�F�N�g�p�̒��_�V�F�[�_�[
// �P�D�v���p�e�B�Łu�\���v�Ɓu�v���b�g�t�H�[���v�����ׂĂɕύX
// �Q�D�G���g���|�C���g����mainVS�ɕύX
// �R�DShaderModel 5.0�A�V�F�[�_�[�̎�ނ�������Β��_�V�F�[�_�[�ɕύX
//
// ------------------------
#include"Fx_Header.hlsli"

BasicType main(VS_Input vs) {
	BasicType output;//�s�N�Z���V�F�[�_�֓n���l

	matrix wmat = mat[vs.InstanceId];

	// �A�j���[�V�����t���[����UV��ݒ�
	float2 flame[] = {// Vector2 �Ɠ���
		float2(0.0f,0.0f),
		float2(0.25f,0.0f),
		float2(0.5f,0.0f),
		float2(0.75f,0.0f),

		float2(0.0f,0.333f),
		float2(0.25f,0.333f),
		float2(0.5f,0.333f),
		float2(0.75f,0.333f),

		float2(0.0f,0.666f),
		float2(0.25f,0.666f),
		float2(0.5f,0.666f),
		float2(0.75f,0.666f),
	};
	float anime = wmat[2][3];// ���̃A�j���̃R�}�̐���

	wmat[2][3] = 0;

	output.svpos = mul(wmat, vs.pos);
	output.uv = vs.uv + flame[(int)anime];
	return output;
}

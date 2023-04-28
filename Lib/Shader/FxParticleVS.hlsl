// ------------------------
//
// FxVS.hlsl
// �G�t�F�N�g�p�̒��_�V�F�[�_�[
// �P�D�v���p�e�B�Łu�\���v�Ɓu�v���b�g�t�H�[���v�����ׂĂɕύX
// �Q�D�G���g���|�C���g����mainVS�ɕύX
// �R�DShaderModel 5.0�A�V�F�[�_�[�̎�ނ�������Β��_�V�F�[�_�[�ɕύX
//
// ------------------------
#include"FxParticle_Header.hlsli"

BasicType main(VS_Input vs) {
	BasicType output;//�s�N�Z���V�F�[�_�֓n���l

	matrix wmat = data[vs.InstanceId].mat;
	output.uv.xy = vs.uv;// 
	output.uv.z = vs.InstanceId;

	// �ʒu���
	wmat[2][3] = 0;// Z���ɃA�j���[�V�����t���[�������Ă�̂ŃN���A
	output.svpos = mul(wmat, vs.pos);

	return output;
}

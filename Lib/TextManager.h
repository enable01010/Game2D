#pragma once

#include "..\stdafx.h"

#include <vector>

#include<iostream>
#include<fstream>
#define TexSize 2048
#define fontSize 40
#define RELEASE(val) if (val != NULL){ val->Release(); val = NULL;}
class TextManager
{
private:
	static const int TextSize = 512;// �����镶���̍ő吔
	float uvRate = (float)fontSize / (float)TexSize;
	const Image* img;
	ID3D12Resource* pTexture;


	bool InitFlag = false;
	Vector2 Pos;
	Vector2 Scale;
	Vector2 Size;


	// ���_���
	Vertex* vertMaps[TextSize];
	int ids[TextSize];
	int TextCount = 0;// �����̃J�E���^
	int count;// ���_�`��p�̃J�E���^
	int drawID[TextSize];
	std::vector<std::pair<int, UINT>> charID;

	// �e�N�X�`��
	D3D12_VERTEX_BUFFER_VIEW vbViews[TextSize];
	Vertex* vertMap = nullptr;
	ID3D12Resource* texbuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	ID3D12DescriptorHeap* texDescHeap = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;
	ID3D12PipelineState* _pipelinestate = nullptr;

	TexMetadata metadata = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};

	bool startflg = false;

	ID3DBlob* _vsBlob = nullptr;
	ID3DBlob* _psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	// DrawImage���킴�킴�p������K�v�͖������ǐF�X�����o�g���Ėʓ|�Ȃ̂łƂ肠�����g���B
	// ���͎g���Ȃ��̂Ŗ��������邽�߂�Private�ɒ�`������
	void SetTransform(Vector2 pos, Vector2 scale = Vector2(1.0f, 1.0f)) {}
	void SetPosSize(Vector2 pos, Vector2 size) {}
	void SetPosSizeUV(Vector2 pos, Vector2 size, Vector2 tl, Vector2 br) {}
	void Draw(ID3D12GraphicsCommandList* cmdList) {}
public:

	/// <summary>
	/// ���������܂�
	/// </summary>
	/// <param name="dev">DirectX�̃f�o�C�X</param>
	/// <param name="textSet">�t�H���g�Ƃ��Ďg������</param>
	/// <param name="cacheFileName">�L���b�V����t�H���g�摜��ۑ�����t�@�C�����i�g���q�����j</param>
	void Init(ID3D12Device* dev, const char* textSet, const char* cacheFileName, int r = 0, int g = 0, int b = 0);

	/// <summary>
	/// Draw���ĂԈ�ԍŏ��ɌĂт܂��B
	/// </summary>
	/// <param name="cmdList"></param>
	void DrawStart(ID3D12GraphicsCommandList* cmdList);
	void SetPos(Vector2 pos) { Pos = pos; }

	/// <summary>
	/// �������\�����܂��B�������̎��ɐݒ肵�����������g���܂���B
	/// ��Ƀt���[���ň�x����DrawStart���Ă�ł��������B������Ă�ŕʂ̕�������o�������o���܂��B
	/// </summary>
	/// <param name="cmdList">DirectX�̃R�}���h���X�g</param>
	/// <param name="str">�\�����镶����</param>
	/// <param name="pos">�\���ʒu�i���オ0�C0�j</param>
	/// <param name="scale">�\������X�P�[���B�ȗ�����Ɠ��{�\��</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, const char* str, Vector2 pos, float scale = 1.0f, bool isLeftUp = true)
	{
		SetText(str);
		SetPos(pos);
		DrawInstance(cmdList, scale, isLeftUp);
	}
	void Destroy() {
		RELEASE(pTexture);
		RELEASE(texbuff);
		RELEASE(texDescHeap);
		RELEASE(rootsignature);
		RELEASE(_pipelinestate);
		RELEASE(_vsBlob);
		RELEASE(_psBlob);
		RELEASE(errorBlob);
		//delete vertMaps;
		//delete vbViews;
		//delete vertMap;

	}

	
private:
	void DrawInstance(ID3D12GraphicsCommandList* cmdList, float scale = 1.0f , bool isLeftUp = true);
	UINT CheckCode(const char* str, int* h)
	{
		UINT code = 0;
		if (IsDBCSLeadByte(str[*h]))
		{
			code = (BYTE)str[*h] << 8 | (BYTE)str[*h + 1];
			*h += 2;
		}
		else
		{
			code = str[*h];
			*h++;
		}
		return code;
	}
	void SetText(const char* str)
	{
		int strsize = (int)strlen(str);
		TextCount = 0;
		for (int h = 0; h < strsize; )
		{
			UINT code;
			if (IsDBCSLeadByte(str[h]))
			{
				code = (BYTE)str[h] << 8 | (BYTE)str[h + 1];
				h += 2;
			}
			else
			{
				code = str[h];
				h++;
			}
			for (int c = 0; c < charID.size(); c++)
			{
				if (code == charID[c].second)
				{
					drawID[TextCount] = charID[c].first;
					break;
				}
			}
			TextCount++;
		}
	}
};

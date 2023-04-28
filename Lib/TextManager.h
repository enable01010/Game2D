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
	static const int TextSize = 512;// 扱える文字の最大数
	float uvRate = (float)fontSize / (float)TexSize;
	const Image* img;
	ID3D12Resource* pTexture;


	bool InitFlag = false;
	Vector2 Pos;
	Vector2 Scale;
	Vector2 Size;


	// 頂点情報
	Vertex* vertMaps[TextSize];
	int ids[TextSize];
	int TextCount = 0;// 文字のカウンタ
	int count;// 頂点描画用のカウンタ
	int drawID[TextSize];
	std::vector<std::pair<int, UINT>> charID;

	// テクスチャ
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
	// DrawImageをわざわざ継承する必要は無いけど色々メンバ使って面倒なのでとりあえず使う。
	// ↓は使えないので無効化するためにPrivateに定義し直す
	void SetTransform(Vector2 pos, Vector2 scale = Vector2(1.0f, 1.0f)) {}
	void SetPosSize(Vector2 pos, Vector2 size) {}
	void SetPosSizeUV(Vector2 pos, Vector2 size, Vector2 tl, Vector2 br) {}
	void Draw(ID3D12GraphicsCommandList* cmdList) {}
public:

	/// <summary>
	/// 初期化します
	/// </summary>
	/// <param name="dev">DirectXのデバイス</param>
	/// <param name="textSet">フォントとして使う文字</param>
	/// <param name="cacheFileName">キャッシュやフォント画像を保存するファイル名（拡張子無し）</param>
	void Init(ID3D12Device* dev, const char* textSet, const char* cacheFileName, int r = 0, int g = 0, int b = 0);

	/// <summary>
	/// Drawを呼ぶ一番最初に呼びます。
	/// </summary>
	/// <param name="cmdList"></param>
	void DrawStart(ID3D12GraphicsCommandList* cmdList);
	void SetPos(Vector2 pos) { Pos = pos; }

	/// <summary>
	/// 文字列を表示します。初期化の時に設定した文字しか使えません。
	/// 先にフレームで一度だけDrawStartを呼んでください。複数回呼んで別の文字列を出す事も出来ます。
	/// </summary>
	/// <param name="cmdList">DirectXのコマンドリスト</param>
	/// <param name="str">表示する文字列</param>
	/// <param name="pos">表示位置（左上が0，0）</param>
	/// <param name="scale">表示するスケール。省略すると等倍表示</param>
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

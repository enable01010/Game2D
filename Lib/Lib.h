#pragma once
#define COUNT 1000

#include "..\stdafx.h"


// #define DEBUG_MODE true
#define DEBUG_MODE
#define RELEASE(val) if (val != NULL){ val->Release(); val = NULL;}


const float WINDOW_WIDTH = 1280;
const float WINDOW_HEIGHT = 720;

using namespace DirectX;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void DebugLog(std::wstring* str);
bool CheckResult(HRESULT result, const char* mes);
void DebugLayer();

/// <summary>
/// 頂点が必要とする情報
/// </summary>
struct Vertex {
	XMFLOAT3 pos;//XYZ座標
	XMFLOAT2 uv;//UV座標
	void Set(float x, float y, float z, float u, float v) { pos = XMFLOAT3(x, y, z); uv = XMFLOAT2(u, v); }
};

// 良い感じのColorの構造体が無くなってるっぽいので自作
struct Color {
	float r;
	float g;
	float b;
	float a;
	Color()
	{
		r = 1;
		g = 1;
		b = 1;
		a = 1;
	}
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(float _a)
	{
		r = 1;
		g = 1;
		b = 1;
		a = _a;
	}
};

/// <summary>
/// 位置や大きさ等を扱うためのクラス
/// </summary>
struct Vector2 {
	float x;
	float y;

	Vector2() { x = 0; y = 0; }
	Vector2(float num) { x = num; y = num; }
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	
	XMFLOAT2 toXM() { return XMFLOAT2(x, y); }

	Vector2 operator +(Vector2 vec) 
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator +=(Vector2 vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator =(XMFLOAT2 vec) 
	{
		x = vec.x;
		y = vec.y;
	}

	Vector2 operator -(Vector2 vec) 
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	void operator -=(Vector2 vec) {
		x -= vec.x;
		y -= vec.y;
	}

	Vector2 operator *(float num)
	{
		return Vector2(x * num, y * num);
	}

	Vector2 operator *=(float num)
	{
		x *= num;
		y *= num;
	}

	Vector2 operator /(float num)
	{
		return Vector2(x / num, y / num);
	}

	Vector2 operator /=(float num)
	{
		x /= num;
		y /= num;
	}

	float magnitude() {
		return sqrt(x * x + y * y);
	}

	float sqrtMagnitude() {
		return (x * x + y * y);
	}

};

enum class IMG_BASE_POSITION
{
	SENTER_UP,
	LEFT_SENTER,
	SENTER_SENTER,
	RIGHT_SENTER,
	SENTER_DOWN
};

/// <summary>
/// シェーダーに渡すデータ
/// </summary>
__declspec(align(256))
struct ConstData
{
	Color color;
	XMMATRIX mat;
	XMFLOAT2 uv;
};

/// <summary>
/// 画像を読み込んだり表示したりするクラス
/// </summary>
class VTN_Image
{
public:
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, IMG_BASE_POSITION base = IMG_BASE_POSITION::SENTER_SENTER, bool add = false);

	void SetPos(Vector2 pos) {
		Pos = pos;
	}

	void AddPos(Vector2 pos) { Pos += pos; } // +=演算子が使えないのでVector作った

	Vector2 GetPos() { return Pos; }
	Vector2* GetPos_Address(){ return &Pos; }

	void SetScale(Vector2 scale) { Scale.x = scale.x; Scale.y = scale.y;}
	void AddScale(float num){ Scale.x = (Scale.x + num > 0)?(Scale.x + num):(0); Scale.y = (Scale.y + num > 0) ? (Scale.y + num) : (0); }
	float GetScale() { return (Scale.x + Scale.y) *0.5f; }
	Vector2* GetSize_Address() { return &Scale; }

	void Draw(ID3D12GraphicsCommandList* cmdList, int instanceCount = 1);

	void Destroy()
	{
		RELEASE(texbuff);
		RELEASE(basicDescHeap);
		RELEASE(rootsignature);
		RELEASE(_pipelinestate);
		RELEASE(m_pConstantBuffer);
		RELEASE(vertBuff);
		RELEASE(idxBuff);
		RELEASE(_vsBlob);
		RELEASE(_psBlob);
		RELEASE(errorBlob);
	}

	void SetColor(Color col){MapConstBuffer->color = col;}
	void AddColor(float num) { MapConstBuffer->color.a = (MapConstBuffer->color.a + num > 0) ? ((MapConstBuffer->color.a + num < 1) ? (MapConstBuffer->color.a + num) : (1)) : (0); }
	float GetColorArufa() { return MapConstBuffer->color.a; }

	void SetRotation(int Degree) { Rotation = Degree * 3.14f / 180 ; }
	void SetRotation(Vector2 dir) { Rotation = atan2(dir.x / 1280 * 720, dir.y);  }
	float GetRotation() { return Rotation; }

	Vector2 GetSize() { return Vector2(metadata.width, metadata.height) * 0.5f; }
	float Get_Radius() { return (GetSize().sqrtMagnitude())*0.5f; }
	bool getInitFlag(){ return InitFlag;}

protected:
	ID3D12Device* Dev;
	bool InitFlag = false;
	Vector2 Pos;
	Vector2 Scale;
	XMFLOAT2 Size;
	float Rotation = 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	ID3D12PipelineState* _pipelinestate = nullptr;
	ID3D12DescriptorHeap* basicDescHeap = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;

	// テクスチャ
	Vertex* vertMap = nullptr;
	const Image* img;
	ID3D12Resource* texbuff = nullptr;
	TexMetadata metadata = {};// テクスチャ

	// 頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	ID3D12Resource* idxBuff = nullptr;
	ID3D12Resource* vertBuff = nullptr;

	// 頂点の情報
	Vertex vertexs[4];

	// シェーダー
	ID3DBlob* _vsBlob = nullptr;
	ID3DBlob* _psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	D3D12_HEAP_PROPERTIES heapprop = {};

	// 定数バッファ系
	ID3D12Resource* m_pConstantBuffer = nullptr;
	UINT8* m_pCbvDataBegin;
	ConstData* MapConstBuffer;
	ConstData		m_ConstantBufferData;

	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

	LRESULT LoadTex(ID3D12Device* dev, const wchar_t* fileName);
	LRESULT LoadShader(ID3D12Device* dev, const wchar_t* pxShader, const wchar_t* vsShader);

	template<class T> LRESULT InitConstBuffer(T* BufferData, T** Buffer)
	{
		// 定数バッファ
		int size = (sizeof(T) + 0xff & ~0xff);// 255の倍数じゃないといけないらしい
		CD3DX12_RESOURCE_DESC heapdesc = CD3DX12_RESOURCE_DESC::Buffer(size);
		LRESULT result = Dev->CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&heapdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_pConstantBuffer)
		);

		m_pConstantBuffer->Map(0, nullptr, (void**)Buffer);
		**Buffer = *BufferData;

		//通常テクスチャビュー作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = metadata.format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		D3D12_CPU_DESCRIPTOR_HANDLE bacisHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
		Dev->CreateShaderResourceView(texbuff, &srvDesc, bacisHeapHandle);

		bacisHeapHandle.ptr += Dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_pConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (UINT)m_pConstantBuffer->GetDesc().Width;
		Dev->CreateConstantBufferView(&cbvDesc, bacisHeapHandle);

		return result;
	}

	/// <summary>
	/// 頂点関係の初期化のテンプレート
	/// </summary>
	void Init_Vertex(IMG_BASE_POSITION base = IMG_BASE_POSITION::SENTER_SENTER) {
		// 頂点レイアウト
		inputLayout[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
		inputLayout[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

		// 頂点の設定
		float width = metadata.width * 0.5f;
		float height = metadata.height * 0.5f;
		if (base == IMG_BASE_POSITION::SENTER_UP)
		{
			vertexs[0].Set(-width, 2 * height, 0.0f, 0.0f, 1.0f);
			vertexs[1].Set(-width, 0, 0.0f, 0.0f, 0.0f);
			vertexs[2].Set(width * width, 2 * height, 0.0f, 1.0f, 1.0f);
			vertexs[3].Set(width * width, 0, 0.0f, 1.0f, 0.0f);
		}
		else if (base == IMG_BASE_POSITION::LEFT_SENTER)
		{
			vertexs[0].Set(0, height, 0.0f, 0.0f, 1.0f);
			vertexs[1].Set(0, -height, 0.0f, 0.0f, 0.0f);
			vertexs[2].Set(2 * width, height, 0.0f, 1.0f, 1.0f);
			vertexs[3].Set(2 * width, -height, 0.0f, 1.0f, 0.0f);
		}
		else if (base == IMG_BASE_POSITION::SENTER_SENTER)
		{
			vertexs[0].Set(-width, height, 0.0f, 0.0f, 1.0f);
			vertexs[1].Set(-width, -height, 0.0f, 0.0f, 0.0f);
			vertexs[2].Set(width, height, 0.0f, 1.0f, 1.0f);
			vertexs[3].Set(width, -height, 0.0f, 1.0f, 0.0f);
		}
		else if (base == IMG_BASE_POSITION::RIGHT_SENTER)
		{
			vertexs[0].Set(-2 * width, height, 0.0f, 0.0f, 1.0f);
			vertexs[1].Set(-2 * width, -height, 0.0f, 0.0f, 0.0f);
			vertexs[2].Set(0, height, 0.0f, 1.0f, 1.0f);
			vertexs[3].Set(0, -height, 0.0f, 1.0f, 0.0f);
		}
		else if (base == IMG_BASE_POSITION::SENTER_DOWN)
		{
			vertexs[0].Set(-width, 0, 0.0f, 0.0f, 1.0f);
			vertexs[1].Set(-width, -2 * height, 0.0f, 0.0f, 0.0f);
			vertexs[2].Set(width, 0, 0.0f, 1.0f, 1.0f);
			vertexs[3].Set(width, -2 * height, 0.0f, 1.0f, 0.0f);
		}
	}
	/// <summary>
	/// サンプラーの初期化のテンプレート
	/// </summary>
	void Init_Sampler() {
		// サンプラー
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}
	/// <summary>
	/// ブレンドの初期化のテンプレート
	/// </summary>
	void Init_Blend(bool add)
	{
		// ブレンドの設定。アルファや合成等の設定
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		renderTargetBlendDesc.LogicOpEnable = false;

		renderTargetBlendDesc.BlendEnable = TRUE;
		renderTargetBlendDesc.LogicOpEnable = FALSE;

		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		if (add)
		{
			renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
			renderTargetBlendDesc.DestBlend = D3D12_BLEND_ONE;
			renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
		}
		else {
			renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;

			renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		}
		renderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}
};


/// <summary>
/// 雑魚敵等にも使いたいので継承出来るようにテンプレートクラスにする
/// </summary>
/// <typeparam name="T"></typeparam>
template<class T >
class STG_Instance : public VTN_Image
{
protected:
	// 弾のデータ
	T Data[COUNT];
	int maxCount = 0;
public:
	int GetCount() { return COUNT; }
	T* GetData(int id) { return &Data[id]; }

};

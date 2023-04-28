#include "..\stdafx.h"
#include "Lib.h"


using namespace DirectX;

#define RED "\e[0;31m"
/// <summary>
/// デバッグレイヤーをオンにする
/// </summary>
void DebugLayer()
{
	ID3D12Debug* debuglayer = nullptr;
	HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debuglayer));
	debuglayer->EnableDebugLayer();
	debuglayer->Release();
}

void DebugLog(std::wstring* str) {
	char buf[100];
	sprintf_s(buf, 100, "%s\n", str->c_str());
	printf_s(buf);
}

// main.cppから持ってくる
/// <summary>
/// HRESULTの内容の表示とエラーチェックを行う
/// </summary>
/// <param name="result"></param>
/// <param name="mes"></param>
/// <returns>trueだとエラーなので終了する</returns>
bool CheckResult(HRESULT result, const char* mes)
{
	char buf[100];
	bool error = false;
	if (result == S_OK) {
		sprintf_s(buf, 100, "[成功]%s\n", mes);
		printf_s(buf);
	}
	else
	{
		HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);     // コンソールハンドル取得
		sprintf_s(buf, 100, "[失敗]%s:%d\n", mes, result);

		SetConsoleTextAttribute(hCons, 0x0c);        // 色指定
		printf_s(buf);
		SetConsoleTextAttribute(hCons, 0x0f);        // 色を元に戻す
		error = true;
	}
	return error;
}

// main.cppから持ってくる
//-----------------------------------------------------------------
//    Window Proc.
//-----------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_PAINT:
		break;
	case WM_DESTROY:// 終了時に呼ばれる
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		OutputDebugString("Active");

		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}

	return 0L;
}

LRESULT VTN_Image::Init(ID3D12Device* dev, const wchar_t* fileName, IMG_BASE_POSITION base, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);
	if (CheckResult(result, "VTN_Image_Init"))
	{
		return result;
	}
	// 頂点レイアウト
	inputLayout[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputLayout[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	// 頂点の設定
	float width = metadata.width * 0.5f;
	float height = metadata.height * 0.5f;
	if (base == IMG_BASE_POSITION::SENTER_UP)
	{
		vertexs[0].Set(-width, 2*height, 0.0f, 0.0f, 1.0f);
		vertexs[1].Set(-width, 0, 0.0f, 0.0f, 0.0f);
		vertexs[2].Set(width * width, 2*height, 0.0f, 1.0f, 1.0f);
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
		vertexs[0].Set(-2*width, height, 0.0f, 0.0f, 1.0f);
		vertexs[1].Set(-2*width, -height, 0.0f, 0.0f, 0.0f);
		vertexs[2].Set(0, height, 0.0f, 1.0f, 1.0f);
		vertexs[3].Set(0, -height, 0.0f, 1.0f, 0.0f);
	}
	else if (base == IMG_BASE_POSITION::SENTER_DOWN)
	{
		vertexs[0].Set(-width, 0, 0.0f, 0.0f, 1.0f);
		vertexs[1].Set(-width, -2* height, 0.0f, 0.0f, 0.0f);
		vertexs[2].Set(width, 0, 0.0f, 1.0f, 1.0f);
		vertexs[3].Set(width, -2 * height, 0.0f, 1.0f, 0.0f);
	}
	
	//左上
	/*vertexs[0].Set(0, 2 * height, 0.0f, 0.0f, 1.0f);
	vertexs[1].Set(0, 0, 0.0f, 0.0f, 0.0f);
	vertexs[2].Set(2*width, 2*height, 0.0f, 1.0f, 1.0f);
	vertexs[3].Set(2*width, 0, 0.0f, 1.0f, 0.0f);*/

	// ブレンドの設定。アルファや合成等の設定
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	renderTargetBlendDesc.LogicOpEnable = false;

	renderTargetBlendDesc.BlendEnable = TRUE;
	renderTargetBlendDesc.LogicOpEnable = FALSE;

	renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	if (add)
	{
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
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

	// シェーダーの読み込み
	result = LoadShader(dev, L"Lib\\Shader\\PixelShader.hlsl", L"Lib\\Shader\\VertexShader.hlsl");
	if (CheckResult(result, "LoadShader"))
	{
		return result;
	}
	InitConstBuffer<ConstData>(&m_ConstantBufferData, &MapConstBuffer);

	// 定数バッファの再設定
	MapConstBuffer->mat = XMMatrixIdentity();
	MapConstBuffer->uv = XMFLOAT2(0, 0);
	InitFlag = true;

	Pos = XMFLOAT2(0.0f, 0.0f);;
	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);

	return result;
}

LRESULT VTN_Image::LoadTex(ID3D12Device* dev, const wchar_t* fileName)
{
	wprintf_s(L"LoadImage: %s\n", fileName);
	LRESULT result;
	// テクスチャをロード
	ScratchImage scratchImg = {};
	result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, &metadata, scratchImg);
	if (CheckResult(result, "LoadFrom")) { return result; }
	img = scratchImg.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	//WriteToSubresourceで転送する用のヒープ設定
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format;
	resDesc.Width = static_cast<UINT>(metadata.width);//幅
	resDesc.Height = static_cast<UINT>(metadata.height);//高さ
	resDesc.DepthOrArraySize = static_cast<uint16_t>(metadata.arraySize);
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	texbuff = nullptr;
	result = dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	switch (result)
	{
	case E_OUTOFMEMORY:printf("E_OUTOFMEMORY"); break;
	}
	result = texbuff->WriteToSubresource(0,
		nullptr,
		img->pixels,
		static_cast<UINT>(img->rowPitch),
		static_cast<UINT>(img->slicePitch)
	);
	if (CheckResult(result, "CreateCommittedResource")) { return result; }

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
	if (CheckResult(result, "descHeapDesc")) { return result; }

	return result;
}

LRESULT VTN_Image::LoadShader(ID3D12Device* dev, const wchar_t* pxShader, const wchar_t* vsShader)
{
	LRESULT result;

	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(vertexs);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの作成
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertexs), std::end(vertexs), vertMap);
	vertBuff->Unmap(0, nullptr);

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertexs);
	vbView.StrideInBytes = sizeof(vertexs[0]);

	unsigned short indices[] = { 0,1,2, 2,1,3 };

	resdesc.Width = sizeof(indices);
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&idxBuff));

	//作ったバッファにインデックスデータをコピー
	unsigned short* mappedIdx = nullptr;
	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(std::begin(indices), std::end(indices), mappedIdx);
	idxBuff->Unmap(0, nullptr);

	//インデックスバッファビューを作成
	ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);


	// シェーダーの作成
	result = D3DCompileFromFile(vsShader,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &_vsBlob, &errorBlob);

	if (CheckResult(result, "VertexShader Load")) {
		switch (result)
		{
		case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
		case HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND):
			printf_s("頂点シェーダーが見当たりません。%ls\n", vsShader);
			break;
		default:
		{
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
			printf_s("ErrorCode = %x\n", (UINT)(result - HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION) + 1));
		}
		break;
		}
		return result;
	}

	result = D3DCompileFromFile(pxShader,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &_psBlob, &errorBlob);

	if (CheckResult(result, "PixelShader Load")) {
		switch (result)
		{
		case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
		case HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND):
			printf_s("ピクセルシェーダーが見当たりません。%ls\n", pxShader);
			break;
		default:
		{
			printf_s("ErrorCode = %x\n", (UINT)(result - HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION) + 1));
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
		}
		break;
		}
		return result;
	}


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	const int DescTblRangeCount = 2;
	D3D12_DESCRIPTOR_RANGE descTblRange[DescTblRangeCount] = {};
	// テクスチャ用
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descTblRange[0].BaseShaderRegister = 0;
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 定数用
	descTblRange[1].NumDescriptors = 1;
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descTblRange[1].BaseShaderRegister = 0;
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootparam = {};
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootparam.DescriptorTable.NumDescriptorRanges = 2;
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootSignatureDesc.pParameters = &rootparam;
	rootSignatureDesc.NumParameters = 1;

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();

	// グラフィックスパイプラインの設定
	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = _vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = _vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = _psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = _psBlob->GetBufferSize();

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = FALSE;
	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;
	//for (int i = 0; i < _countof(pipelineStateDesc.BlendState.RenderTarget); ++i)
	//{
	//	gpipeline.BlendState.RenderTarget[i] = renderTargetBlendDesc;
	//}
	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;

	gpipeline.RasterizerState.FrontCounterClockwise = false;
	gpipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	gpipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	gpipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	gpipeline.RasterizerState.AntialiasedLineEnable = false;
	gpipeline.RasterizerState.ForcedSampleCount = 0;
	gpipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	gpipeline.DepthStencilState.DepthEnable = false;
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;
	gpipeline.pRootSignature = rootsignature;
	_pipelinestate = nullptr;
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&_pipelinestate));
	if (FAILED(result))
	{
		printf("Error : ID3D12Device::CreateDescriptorHeap() Failed.");
		return false;
	}
}

/// <summary>
/// 表示
/// </summary>
/// <param name="cmdList"></param>
void VTN_Image::Draw(ID3D12GraphicsCommandList* cmdList, int instanceCount)
{
	if (!InitFlag)return;
	// 原点に初期化


	MapConstBuffer->mat = XMMatrixIdentity();

	// 回転
	MapConstBuffer->mat = XMMatrixMultiply(MapConstBuffer->mat, XMMatrixRotationZ(Rotation));

	// 拡大縮小
	MapConstBuffer->mat = XMMatrixMultiply(MapConstBuffer->mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * Scale.x, -2.0f / WINDOW_HEIGHT * Scale.y, 0.1f));

	// 移動
	MapConstBuffer->mat.r[3].m128_f32[0] = Pos.x / WINDOW_WIDTH * 2.0f;
	MapConstBuffer->mat.r[3].m128_f32[1] = Pos.y / WINDOW_HEIGHT * -2.0f;

	cmdList->SetPipelineState(_pipelinestate);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->SetGraphicsRootSignature(rootsignature);
	cmdList->SetDescriptorHeaps(1, &basicDescHeap);

	cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

	cmdList->DrawIndexedInstanced(6, instanceCount, 0, 0, 0);

}





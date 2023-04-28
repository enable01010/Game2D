
#include "..\stdafx.h"
#include <iostream>
#include <fstream>
#include <random>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
// Once����Ă��w�b�_�[�ɓ����Ɖ��̂��d���C���N���[�h�G���[���o��

#include <cstddef>
#include <memory>
#include <new>

#define BUFFER_DRAW false

void TextManager::Init(ID3D12Device* dev, const char* textSet,const char* cacheFileName ,int r, int g, int b)
{
	LRESULT result;

	HRESULT hr;
	D3D12_RESOURCE_DESC descResourceTex;
	D3D12_HEAP_PROPERTIES heapProps;
	ZeroMemory(&heapProps, sizeof(heapProps));
	ZeroMemory(&descResourceTex, sizeof(descResourceTex));

	heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProps.CreationNodeMask = 0;
	heapProps.VisibleNodeMask = 0;

	descResourceTex.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	descResourceTex.Width = TexSize;
	descResourceTex.Height = TexSize;
	descResourceTex.DepthOrArraySize = 1;
	descResourceTex.MipLevels = 1;
	descResourceTex.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descResourceTex.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	descResourceTex.SampleDesc.Quality = 0;
	descResourceTex.SampleDesc.Count = 1;

	hr = dev->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &descResourceTex, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pTexture));
	if (FAILED(hr)) {

	}
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1},{0,0},{0,0},{0,1} };


	std::string outFileName = std::string(cacheFileName) + ".txt";
	std::string pngFileName = std::string(cacheFileName) + ".png";

	std::ifstream fontSetData(outFileName);
	int buf_size = 1024;
	char* readStr = new char[buf_size];
	bool fontCheck = false;
	

	int textSetSize = strlen(textSet);
	int txtcount = 0;
	if (fontSetData.good()) {
		while (fontSetData.get(readStr, buf_size)) {
			for (int h = 0; h < buf_size && h < textSetSize; )
			{
				// eof���o����I��
				if (readStr[h] == EOF)
				{
					break;
				}
				int cid = h;
				UINT check = CheckCode(readStr, &cid);

				UINT code = 0;
				if (IsDBCSLeadByte(textSet[h]))
				{
					code = (BYTE)textSet[h] << 8 | (BYTE)textSet[h + 1];
					h += 2;
				}
				else
				{
					code = textSet[h];
					h++;
				}

				if (code != check)
				{
					// �ۑ����ꂽ�f�[�^�ƌ��݂̕������Ⴆ�΍Đ���
					fontCheck = true;
				}

				charID.push_back(std::make_pair(txtcount, code));
				txtcount++;
			}
			if (fontCheck)
			{
				break;
			}
		}
	}
	else {
		fontCheck = true;
	}
	// �t�H���g���������Đ���
#if !BUFFER_DRAW
	if (fontCheck)
#endif
	{
		delete[] readStr;
		// �t�H���g���
		HFONT hFont = CreateFont(
			fontSize,
			0,
			0,
			0,
			0,
			FALSE,
			FALSE,
			FALSE,
			SHIFTJIS_CHARSET,
			OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_MODERN,
			"�l�r �S�V�b�N"
		);

		// �f�o�C�X�R���e�L�X�g�擾
		// �t�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		// �����R�[�h�擾
		int dataSize = 4 * TexSize * TexSize;
		BYTE* vecData = new BYTE[dataSize];// �ۑ��p

		for (int h = 0; h < dataSize / 4; h++)
		{
			vecData[h * 4 + 0] = r;
			vecData[h * 4 + 1] = g;
			vecData[h * 4 + 2] = b;
			vecData[h * 4 + 3] = 0;
		}

		// �����e�N�X�`���𐶐�
		int strsize = (sizeof(textSet) - 1);
		int count = 0;
		int xCount = TexSize / fontSize;
		int yCount = TexSize / fontSize;
		int rowCount = 0;
		for (int h = 0; h < textSetSize; )
		{
			UINT code = 0;
			int row = (rowCount % xCount);
			int clm = (rowCount / xCount);
			rowCount++;

			if (IsDBCSLeadByte(textSet[h]))
			{
				code = (BYTE)textSet[h] << 8 | (BYTE)textSet[h + 1];
				h += 2;
			}
			else
			{
				code = textSet[h];
				h++;
			}
			// �r�b�g�}�b�v�擾
			DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, 0, NULL, &Mat);
			BYTE* ptr = new BYTE[size];
			GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, size, ptr, &Mat);
			TEXTMETRICA TM;
			GetTextMetrics(hdc, &TM);
			int Width = size / GM.gmBlackBoxY;// GM.gmCellIncX - (GM.gmCellIncX - GM.gmBlackBoxX);
			int Height = GM.gmBlackBoxY;
			int topSpace = TM.tmHeight - (TM.tmDescent + GM.gmptGlyphOrigin.y);
			int margin = (int)((fontSize - GM.gmCellIncX) * 0.5f);

			// �擾�����r�b�g�}�b�v�ŕ�����`��
			for (int y = 0; y < Height; y++) {
				for (int x = 0; x < Width; x++) {
					int col = ptr[x + y * Width];
					int px = x + GM.gmptGlyphOrigin.x + margin;
					int pos = (int)( px +
						(y * TexSize) +
						(clm * fontSize * TexSize) +
						(row * fontSize) + // �����̈ړ�
						(topSpace * TexSize));// ��̕���
					if (pos * 4 + 3 >= dataSize)
					{
						printf_s("�t�H���g�쐬���s�B�����������炷��TexSize��傫�����Ă�������");
						return;
					}
					vecData[pos * 4 + 0] = r;
					vecData[pos * 4 + 1] = g;
					vecData[pos * 4 + 2] = b;
					vecData[pos * 4 + 3] = col * 255 / 65;
				}
			}
			delete[] ptr;
		}
		
		// �����e�N�X�`���������o��
		std::ofstream outputfile(outFileName);
		outputfile << textSet;
		outputfile.close();

		stbi_write_png(pngFileName.c_str(), TexSize, TexSize, 4, vecData, 0);

		D3D12_BOX box = { 0, 0, 0, TexSize, TexSize, 1 };
		hr = pTexture->WriteToSubresource(0, &box, vecData, 4 * TexSize, 4 * TexSize * TexSize);
		if (FAILED(hr))
		{
			return;
		}
		// �I�u�W�F�N�g�̊J��
		ReleaseDC(NULL, hdc);
		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
		delete[] vecData;
	}


	// �e�N�X�`�������[�h
	/////////////////////////////////////////
#if !BUFFER_DRAW
	ScratchImage scratchImg = {}; 

	wchar_t wcs[100];
	size_t ret;
	mbstowcs_s(&ret, wcs, 100, cacheFileName, _TRUNCATE);
	std::wstring pngFileNameWchar = std::wstring(wcs) + L".png";

	result = LoadFromWICFile(pngFileNameWchar.c_str(), WIC_FLAGS_NONE, &metadata, scratchImg);
	if (CheckResult(result, "LoadFrom")) {}
	img = scratchImg.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	texbuff = nullptr;
	//WriteToSubresource�œ]������p�̃q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format;
	resDesc.Width = static_cast<UINT>(metadata.width);//��
	resDesc.Height = static_cast<UINT>(metadata.height);//����
	resDesc.DepthOrArraySize = static_cast<uint16_t>(metadata.arraySize);
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	result = dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);

	result = texbuff->WriteToSubresource(0,
		nullptr,
		img->pixels,
		static_cast<UINT>(img->rowPitch),
		static_cast<UINT>(img->slicePitch)
	);
#endif
	/////////////////////////////////////////


	///////
	texDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&texDescHeap));

	//�ʏ�e�N�X�`���r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

#if BUFFER_DRAW
	dev->CreateShaderResourceView(pTexture, &srvDesc, texDescHeap->GetCPUDescriptorHandleForHeapStart());
#else
	dev->CreateShaderResourceView(texbuff, &srvDesc, texDescHeap->GetCPUDescriptorHandleForHeapStart());
#endif

	// �|���S���쐬
	Vertex vertices[] = {
		{{-0.5f,-0.9f,0.0f},{0.0f,1.0f} },//����
		{{-0.5f,0.9f,0.0f} ,{0.0f,0.0f}},//����
		{{0.5f,-0.9f,0.0f} ,{1.0f,1.0f}},//�E��
		{{0.5f,0.9f,0.0f} ,{1.0f,0.0f}},//�E��
	};


	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(vertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// ���_�o�b�t�@�̍쐬
	//*
	for (int h = 0; h < TextSize; h++)
	{
		ID3D12Resource* vertBuff = nullptr;
		result = dev->CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff));

		result = vertBuff->Map(0, nullptr, (void**)&vertMaps[h]);

		std::copy(std::begin(vertices), std::end(vertices), vertMaps[h]);

		vertBuff->Unmap(0, nullptr);

		vbViews[h].BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbViews[h].SizeInBytes = sizeof(vertices);
		vbViews[h].StrideInBytes = sizeof(vertices[0]);
	}

	unsigned short indices[] = { 0,1,2, 2,1,3 , 4,5,6, 6,5,7 };

	ID3D12Resource* idxBuff = nullptr;
	resdesc.Width = sizeof(indices);
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&idxBuff));

	//������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
	unsigned short* mappedIdx = nullptr;
	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(std::begin(indices), std::end(indices), mappedIdx);
	idxBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[���쐬
	ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);


	// �V�F�[�_�[�̍쐬
	result = D3DCompileFromFile(L"Lib\\Shader\\TextVS.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &_vsBlob, &errorBlob);
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			::OutputDebugStringA("�t�@�C������������܂���");
		}
		else {
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
		}
		return;
	}
	result = D3DCompileFromFile(L"Lib\\Shader\\TextPS.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &_psBlob, &errorBlob);
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			::OutputDebugStringA("�t�@�C������������܂���");
		}
		else {
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
		}
		return;
	}
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },

	};


	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};

	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	renderTargetBlendDesc.LogicOpEnable = false;

	// �u�����h�̐ݒ�B�A���t�@�i�����x�j��L���ɂ���
	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descTblRange = {};
	descTblRange.NumDescriptors = 1;
	descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descTblRange.BaseShaderRegister = 0;
	descTblRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootparam = {};
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange;
	rootparam.DescriptorTable.NumDescriptorRanges = 1;
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootSignatureDesc.pParameters = &rootparam;
	rootSignatureDesc.NumParameters = 1;

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();

	// �O���t�B�b�N�X�p�C�v���C���̐ݒ�
	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = _vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = _vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = _psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = _psBlob->GetBufferSize();

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;
	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

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

	Size = Vector2(fontSize, fontSize);
	SetPos(Vector2(0,0));

	InitFlag = true;
}

void TextManager::DrawStart(ID3D12GraphicsCommandList* cmdList)
{
	if (!InitFlag)return;
	startflg = true;
	count = 0;
	cmdList->SetPipelineState(_pipelinestate);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->SetGraphicsRootSignature(rootsignature);
	cmdList->SetDescriptorHeaps(1, &texDescHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, texDescHeap->GetGPUDescriptorHandleForHeapStart());
}

void TextManager::DrawInstance(ID3D12GraphicsCommandList* cmdList, float scale, bool isLeftUp)
{
	if (!InitFlag)return;
	if (!startflg)
	{
		HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);     // �R���\�[���n���h���擾
		SetConsoleTextAttribute(hCons, 0x0c);        // �F�w��
		printf_s("TextManager��DrawStart���Ă΂�Ă��܂���B\n");
		SetConsoleTextAttribute(hCons, 0x0f);        // �F�����ɖ߂�
		return;
	}
	float heightRate = (Size.y / WINDOW_HEIGHT) * 2.0f * scale;
	float widthRate = (Size.x / WINDOW_WIDTH) * 2.0f * scale;
	float yRate = Pos.y / (WINDOW_HEIGHT * 0.5f);
	float xRate = Pos.x / (WINDOW_WIDTH * 0.5f);
	int w = TexSize / fontSize;
	float size = 1.0f / (float)w;
	float posRate = (float)fontSize / (float)WINDOW_HEIGHT * scale;
	if (isLeftUp) {
		for (int h = 0; h < TextCount; h++)
		{
			int id = drawID[h];
			cmdList->IASetIndexBuffer(&ibView);
			float x = (id % w) * uvRate;
			float y = (id / w) * uvRate;
			float margin = 1.1f;
			Vertex vertices[] = {
				{{-1.0f + xRate + h * posRate			,1.0f - yRate - heightRate	,0.0f	} ,{x ,y + uvRate}},//����
				{{-1.0f + xRate + h * posRate			,1.0f - yRate 	,0.0f				} ,{x ,y}},//����
				{{-1.0f + xRate + h * posRate + widthRate,1.0f - yRate - heightRate	,0.0f	} ,{x + uvRate,y + uvRate}},//�E��
				{{-1.0f + xRate + h * posRate + widthRate,1.0f - yRate 	,0.0f				} ,{x + uvRate,y}},//�E��
			};
			std::copy(std::begin(vertices), std::end(vertices), vertMaps[count]);

			cmdList->IASetVertexBuffers(0, 1, &vbViews[count]);
			cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
			count++;
		}
	}
	else {
		for (int h = TextCount; h > 0; h--)
		{
			int id = drawID[TextCount-h];
			cmdList->IASetIndexBuffer(&ibView);
			float x = (id % w) * uvRate;
			float y = (id / w) * uvRate;
			float margin = 1.1f;
			Vertex vertices[] = {
				{{-1.0f + xRate - h * posRate			,1.0f - yRate - heightRate	,0.0f	} ,{x ,y + uvRate}},//����
				{{-1.0f + xRate - h * posRate			,1.0f - yRate 	,0.0f				} ,{x ,y}},//����
				{{-1.0f + xRate - h * posRate + widthRate,1.0f - yRate - heightRate	,0.0f	} ,{x + uvRate,y + uvRate}},//�E��
				{{-1.0f + xRate - h * posRate + widthRate,1.0f - yRate 	,0.0f				} ,{x + uvRate,y}},//�E��
			};
			std::copy(std::begin(vertices), std::end(vertices), vertMaps[count]);

			cmdList->IASetVertexBuffers(0, 1, &vbViews[count]);
			cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
			count++;
		}
	}
}
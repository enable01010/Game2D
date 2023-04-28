#include "stdafx.h"
#include"Scene.h"

void chengeSceneTop(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList) {
	Manager::ChangeScene(new ShootingTitleScene(dev, cmdList));
	//Manager::ChangeScene(new ShootingScene(dev, cmdList));
	//Manager::ChangeScene(new SelectScene(dev, cmdList));
	//Manager::ChangeScene(new EasyMode_One(dev, cmdList));
}

//-----------------------------------------------------------------
//    Main.
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevinst, LPSTR nCmdLine, int nCmdShow)
{
#ifdef DEBUG_MODE
	FILE* fp = NULL;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout); /* 標準出力(stdout)を新しいコンソールに向ける */
	freopen_s(&fp, "CONOUT$", "w", stderr); /* 標準エラー出力(stderr)を新しいコンソールに向ける */
#endif


	MSG  msg;
	WNDCLASS wc;
	//HWND hWnd;
	char szClassName[] = "NovelGame";

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	if (!RegisterClass(&wc)) return FALSE;

	DirectXManager::GetInstance()->hWnd = CreateWindow(szClassName, "ノベルゲーム第一回", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);
	if (!DirectXManager::GetInstance()->hWnd) return FALSE;

	ShowWindow(DirectXManager::GetInstance()->hWnd, nCmdShow);
	UpdateWindow(DirectXManager::GetInstance()->hWnd);

	// DirectXデバイス作成
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* _factory;
	IDXGISwapChain4* _swapChain;

	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&dev));

#ifdef DEBUG_MODE
	HRESULT result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_factory));
#else
	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&_factory));
#endif // DEBUG_MODE
	if (CheckResult(result, "DXGIFactory")) { return FALSE; }

	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; i < _factory->EnumAdapters(i, &tmpAdapter); i++)
	{
		adapters.push_back(tmpAdapter);

		DXGI_ADAPTER_DESC desc = {};
		tmpAdapter->GetDesc(&desc);
		std::wstring str = desc.Description;

		DebugLog(&str);
		if (str.find(L"NVIDIA") != std::string::npos)
		{
			break;
		}
	}

	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	if (CheckResult(result, "cmdAllocato")) { return FALSE; }
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator, nullptr, IID_PPV_ARGS(&cmdList));
	if (CheckResult(result, "CreateCommandList")) { return FALSE; }

	// SwapChainの設定
	ID3D12CommandQueue* cmdQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	if (CheckResult(result, "cmdQueueDesc")) { return FALSE; }

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 800,600,DXGI_FORMAT_R8G8B8A8_UNORM };
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	result = _factory->CreateSwapChainForHwnd(cmdQueue, DirectXManager::GetInstance()->hWnd, &swapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)&_swapChain);
	if (CheckResult(result, "SwapChain")) { return FALSE; }

	// ディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;// レンダーターゲットビュー
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* rtvHeaps = nullptr;
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	if (CheckResult(result, "SwapChain")) { return FALSE; }
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = _swapChain->GetDesc(&swcDesc);
	std::vector<ID3D12Resource*> _backBuffers(swcDesc.BufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();

	//SRGBレンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	result = _swapChain->GetDesc(&swapDesc);
	if (CheckResult(result, "_swapChain->GetDesc")) { return 0; }

	std::vector<ID3D12Resource*> backBuffer(swapDesc.BufferCount);

	for (int i = 0; i < swapDesc.BufferCount; i++)
	{
		result = _swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer[i]));
		if (CheckResult(result, "Buffer")) { return FALSE; }

		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		dev->CreateRenderTargetView(backBuffer[i], &rtvDesc, handle);
	}
	// fenceの設定
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	D3D12_VIEWPORT viewport;
	viewport.Width = WINDOW_WIDTH;
	viewport.Height = WINDOW_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	D3D12_RECT scissorrect;
	scissorrect.top = 0;
	scissorrect.left = 0;
	scissorrect.right = scissorrect.left + WINDOW_WIDTH;
	scissorrect.bottom = scissorrect.top + WINDOW_HEIGHT;

	chengeSceneTop(dev, cmdList);

	SoundManager::GetInstance()->Init();
	SoundManager::GetInstance()->Load("sound/Click.wav");//Play(0)
	SoundManager::GetInstance()->Load("sound/GrazeHit.wav");
	SoundManager::GetInstance()->Load("sound/EnemyHit.wav");
	SoundManager::GetInstance()->Load("sound/PlayerHit.wav");
	SoundManager::GetInstance()->Load("sound/PlayerBulletShoot.wav");
	SoundManager::GetInstance()->Load("sound/ChargeShoot.wav");
	SoundManager::GetInstance()->Load("sound/ChargeShootFire.wav");
	SoundManager::GetInstance()->Load("sound/PointItemGet.wav");
	SoundManager::GetInstance()->Load("sound/PowerItemGet.wav");
	SoundManager::GetInstance()->Load("sound/fanfarle.wav");
	SoundManager::GetInstance()->Load("sound/Mogumogu.wav");
	SoundManager::GetInstance()->Load("sound/Detabareneko.wav");
	SoundManager::GetInstance()->Load("sound/ForgetBeam.wav");
	SoundManager::GetInstance()->Load("sound/AreYouLooking.wav");
	SoundManager::GetInstance()->Load("sound/pin.wav");

	Manager m;
	m.Init(dev);
	FrameRateManager FRM;
	FRM.Init(dev);
	

	//メッセージループ(ｳｨﾝﾄﾞｳが閉じられるまでﾙｰﾌﾟ)
	//メインループ
	while (true)
	{
		InputManager::GetInstance()->Update();

		Manager::Update();
		FrameRateManager::Update();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			//ウィンドウ関数にメッセージを送る
			DispatchMessage(&msg);
		}
		// 描画関係
		UINT bbidx = _swapChain->GetCurrentBackBufferIndex();

		D3D12_RESOURCE_BARRIER barrierDesc = {};
		barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrierDesc.Transition.pResource = backBuffer[bbidx];
		barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		cmdList->ResourceBarrier(1, &barrierDesc);

		// レンダーターゲット
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += bbidx * dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		cmdList->OMSetRenderTargets(1, &rtvH, true, nullptr);

		// クリア
		float clearColor[] = { 0.2f,0.3f,0.7f,1.0f };
		cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

		cmdList->RSSetViewports(1, &viewport);
		cmdList->RSSetScissorRects(1, &scissorrect);

		// 各シーンの処理
		if (!(Manager::getIsFadeOut() && !Manager::getIsInitFisish()))
		{ 
			Manager::currentScene->Draw(); 
		}
		FRM.Draw(cmdList);
		if (!m.Draw(cmdList))
		{
			break;
		}
		if (!Manager::GetIsEscape() && !Manager::getIsChengeScene()&&!Manager::currentScene->Update() )
		{
			break;// 終了したらメインループを抜ける
		}
		

		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		cmdList->ResourceBarrier(1, &barrierDesc);
		cmdList->Close();

		ID3D12CommandList* cmdLists[] = { cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);
		cmdQueue->Signal(fence, ++fenceVal);

		if (fence->GetCompletedValue() != fenceVal)
		{
			HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, eventHandle);
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
		cmdAllocator->Reset();//キューをクリア
		cmdList->Reset(cmdAllocator, nullptr);//再びコマンドリストをためる準備

		_swapChain->Present(1, 0);

	}
	return msg.wParam;
}

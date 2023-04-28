#include "SoundManager.h"

SoundManager* SoundManager::instance = NULL;
LPDIRECTSOUNDBUFFER SoundManager::lpPrimary = NULL;      // プライマリサウンドバッファ
std::vector<LPDIRECTSOUNDBUFFER> SoundManager::Resources;// サウンドバッファ
LPDIRECTSOUND8  SoundManager::soundDevice;

SoundManager* SoundManager::GetInstance()
{
	// 初期化されて無ければ自動で初期化
	if (instance == nullptr)
	{
		instance = new SoundManager;
		instance->Init();
	}
	return instance;
}
int SoundManager::Load(const char* file)
{
    LPDIRECTSOUNDBUFFER buf;
    // サウンドバッファ
    if (!CreateSoundBuffer(&buf, file)) {
        return -1;
    }
    Resources.push_back(buf);
    return Resources.size();
}

void SoundManager::Play(int id)
{

    
    Resources[id]->SetCurrentPosition(0);// 素早く再生した時に最初から再生させる
    Resources[id]->Play(0, 0, 0);
}
void SoundManager::stop(int id) {
    Resources[id]->Stop();
}
void SoundManager::setVolume(int id,long sound) {
    Resources[id]->SetVolume(sound);
}

bool SoundManager::Init() {
	HRESULT ret;
	DirectSoundCreate8(NULL, &soundDevice, NULL);
	soundDevice->SetCooperativeLevel(DirectXManager::GetInstance()->hWnd, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);


    // プライマリサウンドバッファの作成
    DSBUFFERDESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.lpwfxFormat = NULL;
    ret = soundDevice->CreateSoundBuffer(&bufferDesc, &lpPrimary, NULL);
    if (FAILED(ret)) {
        printf("プライマリサウンドバッファ作成失敗\n");
        return FALSE;
    }

    // プライマリバッファのステータスを決定
    WAVEFORMATEX waveFormat;
    waveFormat.cbSize = sizeof(WAVEFORMATEX);
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    ret = lpPrimary->SetFormat(&waveFormat);
    if (FAILED(ret)) {
        printf("プライマリバッファのステータス失敗\n");
        return FALSE;
    }

	return TRUE;
}


BOOL SoundManager::CreateSoundBuffer(LPDIRECTSOUNDBUFFER* soundBuffer, const char* file) {
    HRESULT ret;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX waveFormat;

    // WAVファイルをロード
    HMMIO hSrc;
    hSrc = mmioOpenA((LPSTR)file, NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
    if (!hSrc) {
        printf("WAVファイルが見つかりません\n");
        return FALSE;
    }

    // 'WAVE'チャンクチェック
    ZeroMemory(&mSrcWaveFile, sizeof(mSrcWaveFile));
    ret = mmioDescend(hSrc, &mSrcWaveFile, NULL, MMIO_FINDRIFF);
    if (mSrcWaveFile.fccType != mmioFOURCC('W', 'A', 'V', 'E')) {
        printf("WAVEチャンクチェックエラー\n");
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // 'fmt 'チャンクチェック
    ZeroMemory(&mSrcWaveFmt, sizeof(mSrcWaveFmt));
    ret = mmioDescend(hSrc, &mSrcWaveFmt, &mSrcWaveFile, MMIO_FINDCHUNK);
    if (mSrcWaveFmt.ckid != mmioFOURCC('f', 'm', 't', ' ')) {
        printf("fmt チャンクチェックエラー\n");
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // ヘッダサイズの計算
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if (iSrcHeaderSize < sizeof(WAVEFORMATEX))
        iSrcHeaderSize = sizeof(WAVEFORMATEX);

    // ヘッダメモリ確保
    waveFormat = (LPWAVEFORMATEX)malloc(iSrcHeaderSize);
    if (!waveFormat) {
        printf("メモリ確保エラー\n");
        mmioClose(hSrc, 0);
        return FALSE;
    }
    ZeroMemory(waveFormat, iSrcHeaderSize);

    // WAVEフォーマットのロード
    ret = mmioRead(hSrc, (char*)waveFormat, mSrcWaveFmt.cksize);
    if (FAILED(ret)) {
        printf("WAVEフォーマットロードエラー\n");
        free(waveFormat);
        mmioClose(hSrc, 0);
        return FALSE;
    }

    printf("チャンネル数       = %d\n", waveFormat->nChannels);
    printf("サンプリングレート = %d\n", waveFormat->nSamplesPerSec);
    printf("ビットレート       = %d\n", waveFormat->wBitsPerSample);


    // fmtチャンクに戻る
    mmioAscend(hSrc, &mSrcWaveFmt, 0);

    // dataチャンクを探す
    while (1) {
        // 検索
        ret = mmioDescend(hSrc, &mSrcWaveData, &mSrcWaveFile, 0);
        if (FAILED(ret)) {
            printf("dataチャンクが見つからない\n");
            free(waveFormat);
            mmioClose(hSrc, 0);
            return FALSE;
        }
        if (mSrcWaveData.ckid == mmioStringToFOURCCA("data", 0))
            break;
        // 次のチャンクへ
        ret = mmioAscend(hSrc, &mSrcWaveData, 0);
    }

    // サウンドバッファの作成
    DSBUFFERDESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
    bufferDesc.dwBufferBytes = mSrcWaveData.cksize;
    bufferDesc.lpwfxFormat = waveFormat;
    bufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    ret = soundDevice->CreateSoundBuffer(&bufferDesc, soundBuffer, NULL);
    if (FAILED(ret)) {
        //DEBUG("サウンドバッファの作成エラー\n");
        free(waveFormat);
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // ロック開始
    LPVOID pMem1, pMem2;
    DWORD dwSize1, dwSize2;
    ret = (*soundBuffer)->Lock(0, mSrcWaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
    if (FAILED(ret)) {
        //DEBUG("ロック失敗\n");
        free(waveFormat);
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // データ書き込み
    mmioRead(hSrc, (char*)pMem1, dwSize1);
    mmioRead(hSrc, (char*)pMem2, dwSize2);

    // ロック解除
    (*soundBuffer)->Unlock(pMem1, dwSize1, pMem2, dwSize2);

    // ヘッダ用メモリを開放
    free(waveFormat);

    // WAVを閉じる
    mmioClose(hSrc, 0);

    return TRUE;
}

void SoundManager::Release()
{
    for (int i = 0; i < Resources.size(); i++)
    {
        Resources[i]->Release();
        Resources[i] = NULL;
    }
    if (lpPrimary) {
        lpPrimary->Release();
        lpPrimary = NULL;
    }

    if (soundDevice) {
        soundDevice->Release();
        soundDevice = NULL;
    }

    // COMの終了
    CoUninitialize();

}
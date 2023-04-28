#include "SoundManager.h"

SoundManager* SoundManager::instance = NULL;
LPDIRECTSOUNDBUFFER SoundManager::lpPrimary = NULL;      // �v���C�}���T�E���h�o�b�t�@
std::vector<LPDIRECTSOUNDBUFFER> SoundManager::Resources;// �T�E���h�o�b�t�@
LPDIRECTSOUND8  SoundManager::soundDevice;

SoundManager* SoundManager::GetInstance()
{
	// ����������Ė�����Ύ����ŏ�����
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
    // �T�E���h�o�b�t�@
    if (!CreateSoundBuffer(&buf, file)) {
        return -1;
    }
    Resources.push_back(buf);
    return Resources.size();
}

void SoundManager::Play(int id)
{

    
    Resources[id]->SetCurrentPosition(0);// �f�����Đ��������ɍŏ�����Đ�������
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


    // �v���C�}���T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.lpwfxFormat = NULL;
    ret = soundDevice->CreateSoundBuffer(&bufferDesc, &lpPrimary, NULL);
    if (FAILED(ret)) {
        printf("�v���C�}���T�E���h�o�b�t�@�쐬���s\n");
        return FALSE;
    }

    // �v���C�}���o�b�t�@�̃X�e�[�^�X������
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
        printf("�v���C�}���o�b�t�@�̃X�e�[�^�X���s\n");
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

    // WAV�t�@�C�������[�h
    HMMIO hSrc;
    hSrc = mmioOpenA((LPSTR)file, NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
    if (!hSrc) {
        printf("WAV�t�@�C����������܂���\n");
        return FALSE;
    }

    // 'WAVE'�`�����N�`�F�b�N
    ZeroMemory(&mSrcWaveFile, sizeof(mSrcWaveFile));
    ret = mmioDescend(hSrc, &mSrcWaveFile, NULL, MMIO_FINDRIFF);
    if (mSrcWaveFile.fccType != mmioFOURCC('W', 'A', 'V', 'E')) {
        printf("WAVE�`�����N�`�F�b�N�G���[\n");
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // 'fmt '�`�����N�`�F�b�N
    ZeroMemory(&mSrcWaveFmt, sizeof(mSrcWaveFmt));
    ret = mmioDescend(hSrc, &mSrcWaveFmt, &mSrcWaveFile, MMIO_FINDCHUNK);
    if (mSrcWaveFmt.ckid != mmioFOURCC('f', 'm', 't', ' ')) {
        printf("fmt �`�����N�`�F�b�N�G���[\n");
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // �w�b�_�T�C�Y�̌v�Z
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if (iSrcHeaderSize < sizeof(WAVEFORMATEX))
        iSrcHeaderSize = sizeof(WAVEFORMATEX);

    // �w�b�_�������m��
    waveFormat = (LPWAVEFORMATEX)malloc(iSrcHeaderSize);
    if (!waveFormat) {
        printf("�������m�ۃG���[\n");
        mmioClose(hSrc, 0);
        return FALSE;
    }
    ZeroMemory(waveFormat, iSrcHeaderSize);

    // WAVE�t�H�[�}�b�g�̃��[�h
    ret = mmioRead(hSrc, (char*)waveFormat, mSrcWaveFmt.cksize);
    if (FAILED(ret)) {
        printf("WAVE�t�H�[�}�b�g���[�h�G���[\n");
        free(waveFormat);
        mmioClose(hSrc, 0);
        return FALSE;
    }

    printf("�`�����l����       = %d\n", waveFormat->nChannels);
    printf("�T���v�����O���[�g = %d\n", waveFormat->nSamplesPerSec);
    printf("�r�b�g���[�g       = %d\n", waveFormat->wBitsPerSample);


    // fmt�`�����N�ɖ߂�
    mmioAscend(hSrc, &mSrcWaveFmt, 0);

    // data�`�����N��T��
    while (1) {
        // ����
        ret = mmioDescend(hSrc, &mSrcWaveData, &mSrcWaveFile, 0);
        if (FAILED(ret)) {
            printf("data�`�����N��������Ȃ�\n");
            free(waveFormat);
            mmioClose(hSrc, 0);
            return FALSE;
        }
        if (mSrcWaveData.ckid == mmioStringToFOURCCA("data", 0))
            break;
        // ���̃`�����N��
        ret = mmioAscend(hSrc, &mSrcWaveData, 0);
    }

    // �T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
    bufferDesc.dwBufferBytes = mSrcWaveData.cksize;
    bufferDesc.lpwfxFormat = waveFormat;
    bufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    ret = soundDevice->CreateSoundBuffer(&bufferDesc, soundBuffer, NULL);
    if (FAILED(ret)) {
        //DEBUG("�T�E���h�o�b�t�@�̍쐬�G���[\n");
        free(waveFormat);
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // ���b�N�J�n
    LPVOID pMem1, pMem2;
    DWORD dwSize1, dwSize2;
    ret = (*soundBuffer)->Lock(0, mSrcWaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
    if (FAILED(ret)) {
        //DEBUG("���b�N���s\n");
        free(waveFormat);
        mmioClose(hSrc, 0);
        return FALSE;
    }

    // �f�[�^��������
    mmioRead(hSrc, (char*)pMem1, dwSize1);
    mmioRead(hSrc, (char*)pMem2, dwSize2);

    // ���b�N����
    (*soundBuffer)->Unlock(pMem1, dwSize1, pMem2, dwSize2);

    // �w�b�_�p���������J��
    free(waveFormat);

    // WAV�����
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

    // COM�̏I��
    CoUninitialize();

}
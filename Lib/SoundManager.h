#pragma once
#include "../stdafx.h"

class SoundManager
{
	static SoundManager* instance; // �V���O���g��

    static LPDIRECTSOUND8 soundDevice;

	DWORD WavSize;
	WAVEFORMATEX format;
    static std::vector< LPDIRECTSOUNDBUFFER> Resources;
    static LPDIRECTSOUNDBUFFER lpPrimary;       // �v���C�}���T�E���h�o�b�t�@
public:
	static SoundManager* GetInstance();
    static BOOL CreateSoundBuffer(LPDIRECTSOUNDBUFFER* soundBuffer, const char* file);
    bool Init();
    static int Load(const char* file);
    static void Play(int id);
    static void stop(int id);
    static void setVolume(int id,long sound);

    static void ReleaseFile(int id)
    {
        Resources[id]->Release();
        Resources[id] = NULL;
    }
    static void Release();
};

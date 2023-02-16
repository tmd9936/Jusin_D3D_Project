#include "SoundMgr.h"
#include <strsafe.h>

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
{
}


CSoundMgr::~CSoundMgr()
{
	Free();
}

HRESULT CSoundMgr::Ready_Sound()
{
	m_fMusicVolume = 1.f;
	m_fSoundVolume = 1.f;

	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();

	return S_OK;
}

void CSoundMgr::PlaySoundW(_tchar* pSoundKey, CHANNELID eID)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_fSoundVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(_tchar* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[SOUND_BGM]);

	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);

	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], m_fMusicVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	_tchar input[MAX_PATH] = L"";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	StringCchCat(input, MAX_PATH, TEXT("*"));

	hFind = FindFirstFileW(L"../Bin/Resource/Sound/*.*", &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		FindClose(hFind);
		return;
	}

	_tchar szCurPath[MAX_PATH] = L"../Bin/Resource/Sound/";
	_tchar szFullPath[MAX_PATH] = L"";

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			wcscpy_s(szFullPath, szCurPath);
			wcscat_s(szFullPath, FindFileData.cFileName);

			FMOD_SOUND* pSound = nullptr;

			char* pFilePath = new char[MAX_PATH];
			ZeroMemory(pFilePath, sizeof(char) * MAX_PATH);

			WideCharToMultiByte(CP_ACP, 0, szFullPath, -1, pFilePath, MAX_PATH, NULL, NULL);

			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, pFilePath, FMOD_DEFAULT, 0, &pSound);

			Safe_Delete_Array(pFilePath);

			if (eRes == FMOD_OK)
			{
				_tchar* pSoundKey = new _tchar[MAX_PATH];

				wsprintf(pSoundKey, FindFileData.cFileName);

				m_mapSound.emplace(pSoundKey, pSound);
			}
		}
	} while (FindNextFile(hFind, &FindFileData));
}

void CSoundMgr::Free(void)
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
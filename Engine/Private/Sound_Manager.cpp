#include "Sound_Manager.h"
#include <strsafe.h>

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
	: m_pSystem(nullptr)
{
}

_uint APIENTRY LoadSoundFile(void* pArg)
{
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	CSound_Manager* pSound_Manager = (CSound_Manager*)pArg;

	EnterCriticalSection(pSound_Manager->Get_CriticalSection());

	HRESULT			hr = { 0 };


	_tchar input[MAX_PATH] = L"";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	StringCchCat(input, MAX_PATH, TEXT("*"));

	hFind = FindFirstFileW(L"../../Reference/Resources/Sound_Dummy/*.*", &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		pSound_Manager->Set_Finished();
		LeaveCriticalSection(pSound_Manager->Get_CriticalSection());
		FindClose(hFind);
		return 1;
	}

	_tchar szCurPath[MAX_PATH] = L"../../Reference/Resources/Sound_Dummy/";
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

			FMOD_RESULT eRes = FMOD_System_CreateSound(pSound_Manager->Get_FMOD_System(), pFilePath, FMOD_DEFAULT, 0, &pSound);

			Safe_Delete_Array(pFilePath);

			if (eRes == FMOD_OK)
			{
				_tchar* pSoundKey = new _tchar[MAX_PATH];

				wsprintf(pSoundKey, FindFileData.cFileName);

				pSound_Manager->Insert_Sound(pSoundKey, pSound);
			}
		}
	} while (FindNextFile(hFind, &FindFileData));

	pSound_Manager->Set_Finished();
	LeaveCriticalSection(pSound_Manager->Get_CriticalSection());

	return 0;
}


HRESULT CSound_Manager::Ready_Sound()
{
	m_fMusicVolume = 1.f;
	m_fSoundVolume = 1.f;

	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	for (_uint i = 0; i < 32; ++i)
	{
		FMOD_CHANNEL* channel;
		FMOD_System_GetChannel(m_pSystem, (int)i, &channel);
		//m_vecChannel.push_back(channel);
		m_pChannelArr[i] = channel;
	}

	InitializeCriticalSection(m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadSoundFile, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

void CSound_Manager::PlaySoundW(const _tchar* pSoundKey, CHANNELID eID)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_fSoundVolume);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(const _tchar* pSoundKey)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[SOUND_BGM]);

	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);

	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], m_fMusicVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlaySoundW(const _tchar* pSoundKey, const _float& fVolume)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	_uint iID = 0;
	FMOD_BOOL bPlay = FALSE;
	FMOD_Channel_IsPlaying(m_pChannelArr[iID], &bPlay);

	do
	{
		if (FALSE == bPlay)
		{
			FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[iID]);
			FMOD_Channel_SetVolume(m_pChannelArr[iID], fVolume);
			break;
		}
		else
		{
			++iID;
			if (29 < iID)
			{
				break;
			}
			FMOD_Channel_IsPlaying(m_pChannelArr[iID], &bPlay);
		}
	} while (true);

	FMOD_System_Update(m_pSystem);
}


void CSound_Manager::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(m_CriticalSection);
	DeleteObject(m_hThread);

	Safe_Delete(m_CriticalSection);

	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
#pragma once

#include "Base.h"
#include "Engine_Defines.h"

#include "fmod.h"
#include "fmod.hpp"
#include "../Fmod/fmod_common.h"
#include "../Fmod/fmod_errors.h"

//#pragma comment(lib, "fmodex_vc.lib")
#pragma comment(lib, "fmod_vc.lib")


BEGIN(Engine)

class CSound_Manager : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

public:
	explicit CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT			Ready_Sound();

public:
	void PlaySoundW(const _tchar* pSoundKey, CHANNELID eID);
	void PlayBGM(const _tchar* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	void Set_SoundVolume(_float _fSoundVolume) { m_fSoundVolume = _fSoundVolume; }
	void Set_BGMVolume(_float _fMusicVolume) { m_fMusicVolume = _fMusicVolume; }

public:
	_bool Get_Finished() const {
		return m_isFinished;
	}

	LPCRITICAL_SECTION Get_CriticalSection() {
		return m_CriticalSection;
	}

	FMOD_SYSTEM* Get_FMOD_System() {
		return m_pSystem;
	}

	void	Insert_Sound(_tchar* pSoundKey, FMOD_SOUND* pSound) {
		m_mapSound.emplace(pSoundKey, pSound);
	}

private:
	// ���� ���ҽ� ������ ���� ��ü 
	unordered_map<_tchar*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem = { nullptr };

	// ȿ���� ����
	_float m_fSoundVolume = { 0.f };

	// BGM ����
	_float m_fMusicVolume = { 0.f };

private:
	HANDLE				m_hThread = { 0 };
	LPCRITICAL_SECTION	m_CriticalSection = { new CRITICAL_SECTION };	
	_bool				m_isFinished = { false };



public:
	virtual void		Free(void);
};

END
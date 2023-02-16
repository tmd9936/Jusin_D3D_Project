#pragma once

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

public:
	HRESULT			Ready_Sound();

public:
	void PlaySoundW(_tchar* pSoundKey, CHANNELID eID);
	void PlayBGM(_tchar* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	void Set_SoundVolume(_float _fSoundVolume) { m_fSoundVolume = _fSoundVolume; }
	void Set_BGMVolume(_float _fMusicVolume) { m_fMusicVolume = _fMusicVolume; }

private:
	void LoadSoundFile();

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<_tchar*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

	// 효과음 볼륨
	_float m_fSoundVolume;

	// BGM 볼륨
	_float m_fMusicVolume;

public:
	virtual void		Free(void);
};

END
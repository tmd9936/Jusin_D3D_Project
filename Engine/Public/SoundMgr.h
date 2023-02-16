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
	// ���� ���ҽ� ������ ���� ��ü 
	map<_tchar*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

	// ȿ���� ����
	_float m_fSoundVolume;

	// BGM ����
	_float m_fMusicVolume;

public:
	virtual void		Free(void);
};

END
#pragma once

#include "Base.h"
#include "Engine_Defines.h"

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

private:
	void LoadSoundFile();

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

public:
	virtual void		Free(void);
};

END
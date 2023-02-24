#pragma once

#include "Component_Manager.h"
#include "PipeLine.h"

BEGIN(Engine)

class CTimer_Manager;
class CGraphic_Device;
class CLevel_Manager;
class CObject_Manager;
class CComponent_Manager;
class CSound_Manager;

class ENGINE_DLL CGameInstance :
    public CBase
{
    DECLARE_SINGLETON(CGameInstance)

private:
    CGameInstance();
    virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	HRESULT Tick_Engine(_double TimeDelta);
	void Clear_Engine(_uint iLevelIndex);

public: /* For.Timer_Manager */
	HRESULT	Ready_Timer(const _tchar* pTimerTag);
	_double Get_TimeDelta(const _tchar* pTimerTag);
	void Set_TimeDelta(const _tchar* pTimerTag);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present(UINT interval, UINT flags);
	HRESULT SetRenderTargets();
	ID3D11Device* Get_Device();
	IDXGISwapChain* Get_SwapChain();
	ID3D11RenderTargetView* Get_RTV();

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr); /* 원형을 복제하여 사본을 추가한다. */

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Sound_Manager */
	HRESULT		Ready_Sound();
	void		PlaySoundW(const _tchar* pSoundKey, CHANNELID eID);
	void		PlayBGM(const _tchar* pSoundKey);
	void		StopSound(CHANNELID eID);
	void		StopAll();
	void		SetChannelVolume(CHANNELID eID, float fVolume);
	void		Set_SoundVolume(_float _fSoundVolume);
	void		Set_BGMVolume(_float _fMusicVolume) ;
	const _bool	Get_Sound_Ready_Finish();

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_float4x4 Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState);

private:
	CTimer_Manager*			m_pTimer_Manager = { nullptr };
	CGraphic_Device*		m_pGraphic_Device = { nullptr };
	CLevel_Manager*			m_pLevel_Manager = { nullptr };
	CObject_Manager*		m_pObject_Manager = { nullptr };
	CComponent_Manager*		m_pComponent_Manager = { nullptr };
	CSound_Manager*			m_pSound_Manager = { nullptr };
	CPipeLine*				m_pPipeLine = { nullptr };

public:
	static void Release_Engine();

public:
	virtual void Free(void) override;

};

END


#pragma once

#include "Base.h"


BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

#ifdef _IMGUITOOL
class CMapToolGUI;
class CDataToolGUI;
class CSkillToolGUI;
#endif

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	static _bool Get_MainAppInit() {
		return m_Init;
	}

public:
	HRESULT Initialize();
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	HRESULT	RenderFPS();

private:
	void	Tool_Mode_ON_OFF_UI();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CGameInstance* m_pGameInstance = { nullptr };
	CRenderer* m_pRenderer = { nullptr };

#ifdef _IMGUITOOL
	CMapToolGUI* m_pMapToolGUI = { nullptr };
	CDataToolGUI* m_pDataToolGUI = { nullptr };
	CSkillToolGUI* m_pSkillToolGUI = { nullptr };
#endif

private:
	_bool		m_bToolMode = { true };

private:
	HRESULT SetUp_StartLevel(LEVEL eNextLevelID);
	HRESULT Ready_Prototype_Component_For_Static();
	HRESULT Ready_Prototype_GameObject_For_Static();
	HRESULT Ready_GameObject_For_Static();
	HRESULT Ready_Fonts();

#ifdef _IMGUITOOL
private:
	_double						m_TimeAcc = { 0.0 };
	_tchar						m_szFPS[MAX_PATH] = TEXT("");
	_ulong						m_dwNumDraw = { 0 };
#endif // _DEBUG

private:
	static	_bool				m_Init;

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END
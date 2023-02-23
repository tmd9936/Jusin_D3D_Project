#pragma once

#include "Tool_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Tool)
class CMapToolGUI;

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CGameInstance* m_pGameInstance = { nullptr };
	CRenderer* m_pRenderer = { nullptr };
	CMapToolGUI* m_pMapToolGUI = { nullptr };

	//ID3D11RenderTargetView** m_renderTargetView = { nullptr };

private:
	HRESULT SetUp_StartLevel(LEVEL eNextLevelID);
	HRESULT Ready_Prototype_Component_For_Static();
	HRESULT Ready_Prototype_GameObject_For_Static();



public:
	static CMainApp* Create();
	virtual void Free() override;
};

END
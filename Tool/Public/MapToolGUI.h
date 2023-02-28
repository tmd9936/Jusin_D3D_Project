#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CTransform;
class CCalculator;

END

BEGIN(Client)

class CDataToolGUI;

class CMapToolGUI : public CBase
{
	struct ImGui_ImplDX11_ViewportData2
	{
		IDXGISwapChain* SwapChain;
		ID3D11RenderTargetView* RTView;
	};

	struct ImGui_ImplWin32_ViewportData2
	{
		HWND    Hwnd;
		bool    HwndOwned;
		DWORD   DwStyle;
		DWORD   DwExStyle;
	};
	

	DECLARE_SINGLETON(CMapToolGUI)
private:
	explicit CMapToolGUI();
	virtual ~CMapToolGUI();

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); /* 사본객체의 초기화작업 */
	_uint Tick(_double TimeDelta);
	_uint LateTick(_double TimeDelta);
	void Reder_Begin();
	HRESULT Render();
	void Reder_End();

public:
	const int		Get_ListBox(Tool_ID _eType) const { return m_iListBox[_eType]; }
	const bool		Get_Install() const { return m_bInstall; }
	const int		Get_Radio() const { return m_iRadio; }

	void	Map_Index_Add(const int& index);
	void	Environment_Index_Add(const int& index);

	void	Render_Switch() { m_bRender = !m_bRender; }

	void	Set_PickingObject(CGameObject* obj);

	void	Remove_PickingObject();

	void	Set_PrefabName(const wstring& PrefabName) {
		m_PrefabName = PrefabName;
	}

	void	Add_Environment();

	HRESULT	Change_ViewerObject(const wstring& PrefabName, _uint iLevelindex, const wstring LayerTag, void* pArg = nullptr);

private:
	void	Save_CubeList();
	void	Load_CubeList();

	void	Save_EnvironmentList();
	void	Load_EnvironmentList();

private:
	void	ListBox();
	void	Slider();
	void	Radio();
	void	FileMenuBar();
	void	FileMenu();
	void	TerrainMenu();

private:
	void	Update_Data();
	void	Update_ViewerGameObject();
	HRESULT	Get_Picking_Terrain_Pos(_float3* pVOutPutPos);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	int						m_iListBox[TYPE_END];
	int						m_iRadio = { 0 };
	bool					m_bInstall = { false };

	bool					m_bRender = { true };

	int						m_iEnvironment_current_idx = { 0 };
	int						m_iMap_current_idx = { 0 };

	_float4					m_vScale = {};
	_float4					m_vPos = {};
	_float4					m_vRot = {};

	CGameObject*			m_pPickingObject = { nullptr };

	//vector<string*>			m_vecCube;

	_uint					m_iEnvNum = { 0 };
	_uint					m_iMapNum = { 0 };

	vector<string*>			m_vecEnvironment;
	vector<string*>			m_vecMap;

	ImGuiViewport* m_pRootViewport = { nullptr };
	//ImGuiViewport* m_pviewport = { nullptr };


	//ImGui_ImplDX11_ViewportData** m_pVd = { nullptr };
	//ImGui_ImplWin32_ViewportData2* m_pPd = { nullptr };

	int					m_iTerrainCntX = { 0 };
	int					m_iTerrainCntZ = { 0 };

	wstring				m_PrefabName = {};


	POINT		m_ptMouse = {};

private:
	CCalculator* m_pCalculator = { nullptr };
	CGameObject* m_pViewerObject = { nullptr };

public:
	virtual void Free(void) override;

};

END
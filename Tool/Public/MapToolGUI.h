#pragma once

#include "Tool_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CTransform;

END

BEGIN(Tool)

class CMapToolGUI : public CBase
{

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
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	int						m_iListBox[TYPE_END];
	int						m_iRadio = { 0 };
	bool					m_bInstall = { false };

	bool					m_bRender = { true };

	int						m_iEnvironment_current_idx = { 0 };
	int						m_iMap_current_idx = { 0 };

	_float4					m_vScale;
	_float4					m_vPos;
	_float4					m_vRot;

	CGameObject*			m_pPickingObject = { nullptr };

	//vector<string*>			m_vecCube;

	_uint					m_iEnvNum = { 0 };
	_uint					m_iMapNum = { 0 };

	vector<string*>			m_vecEnvironment;
	vector<string*>			m_vecMap;

	ImGuiViewport* m_pviewport = { nullptr };

public:
	virtual void Free(void) override;

};

END
#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CTransform;

END

BEGIN(Client)
class CMapToolGUI;

class CDataToolGUI final : public CBase
{
	DECLARE_SINGLETON(CDataToolGUI)
private:
	explicit CDataToolGUI();
	virtual ~CDataToolGUI();

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); /* 사본객체의 초기화작업 */
	_uint Tick(_double TimeDelta);
	_uint LateTick(_double TimeDelta);
	HRESULT Render();

public:
	const wstring* Get_Current_GameObject_Prefab() const;
	const _uint Get_Current_Levelindex() const;
	const wstring* Get_Current_LayerName() const;

public:
	void	Update_LevelGameObjects();
private:
	void	View_Base();
	void	View_Level_Layer();
	void	View_Prefab();
	void	View_Control();
	void	View_Level_Objects();


private:
	void	ListBox_Level_List();
	void	ListBox_Layer_List();

	void	Tree_Level_Objects();
private:
	void	Update_LayerList();
	void	LevelGameObjects_Free();

	void	Update_PrefabList();
	void	LayerListBox_Free();
	void	PrefabListBox_Free();

	void	Check_Level_Change();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	char*					m_LevelListBox[LEVEL_END];
	int						m_iLevelListBoxCurrentItem = { 0 };
	int						m_iPreLevelIndex = { -1 };

	char**					m_LayerListBox = { nullptr };
	int						m_iLayerListCurrentItem = { 0 };
	size_t					m_LayerListBoxSize = { 0 };

	char**					m_PrefabListBox = { nullptr };
	int						m_iPrefabListCurrentItem = { 0 };
	size_t					m_PrefabListBoxSize = { 0 };

	wstring					m_CurrentPrefabName = {};
	wstring					m_CurrentLayerName = {};

	_bool					m_StartUpdateObject = { false };

	unordered_map<const char*, vector<CGameObject*>>		m_LevelGameObjects;

public:
	virtual void Free(void) override;

};

END
#pragma once

#include "Tool_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CTransform;

END

BEGIN(Tool)

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

private:
	void	View_Base();
	void	View_Level_Layer();
	void	View_Prefab();

private:
	void	ListBox_Level_List();
	void	Slider();
	void	Radio();
	void	FileMenuBar();
	void	FileMenu();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	char*					m_iLevelListBox[LEVEL_END];
	int						m_iLevelListBoxCurrentItem = { 0 };

public:
	virtual void Free(void) override;

};

END
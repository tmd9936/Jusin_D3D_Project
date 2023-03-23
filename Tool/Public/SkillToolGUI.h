#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Skill_Manager.h"
#include "Skill.h"

#include "Effect_Manager.h"
#include "Effect.h"

#include "Player.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CTransform;

END

BEGIN(Client)
class CMapToolGUI;

class CSkillToolGUI final : public CBase
{
	DECLARE_SINGLETON(CSkillToolGUI)
private:
	explicit CSkillToolGUI();
	virtual ~CSkillToolGUI();

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); /* 사본객체의 초기화작업 */
	_uint Tick(_double TimeDelta);
	_uint LateTick(_double TimeDelta);
	HRESULT Render();

private:
	void	View_Base();
	void	View_Skill_Layer();
	void	View_Skill_Depend_Layer();
	void	View_Effect_Layer();

private:
	void	ListBox_Skill_List();
	void	ListBox_Skill_Depend_List();
	void	ListBox_Effect_List();

private:
	void	Player_Skill_Change();
	void	Reload_Skill_Data();
	void	Reload_Effect_Data();

private:
	void	SkillListBox_Free();
	void	SkillDependListBox_Free();
	void	EffectListBox_Free();

private:
	void	Update_Skill_List();
	void	Update_Skill_Depend_List();
	void	Update_Effect_List();


private:
	void	After_Init();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	char**					m_SkillListBox;
	int						m_iSkillListBoxCurrentItem = { 0 };
	size_t					m_SkillListBoxSize = { 0 };

	char**					m_SkillDependListBox;
	int						m_iSkillDependListBoxCurrentItem = { 0 };
	size_t					m_SkillDependListBoxSize = { 0 };

	char**					m_EffectListBox = { nullptr };
	int						m_iEffectListCurrentItem = { 0 };
	size_t					m_EffectListBoxSize = { 0 };


	_bool					m_ManagerInit = { false };

	CEffect_Manager*		m_pEffect_Manager = { nullptr };
	CSkill_Manager*			m_pSkill_Manager = { nullptr };

	//char**					m_PrefabListBox = { nullptr };
	//int						m_iPrefabListCurrentItem = { 0 };
	//size_t					m_PrefabListBoxSize = { 0 };

	//wstring					m_CurrentPrefabName = {};
	//wstring					m_CurrentLayerName = {};

public:
	virtual void Free(void) override;

};

END
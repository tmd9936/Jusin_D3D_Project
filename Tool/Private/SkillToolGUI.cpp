#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "SkillToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CSkillToolGUI)

CSkillToolGUI::CSkillToolGUI()
{
}

CSkillToolGUI::~CSkillToolGUI()
{
}

HRESULT CSkillToolGUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	m_pDevice = pDevice;
	m_pContext = pContext;

	return S_OK;
}

_uint CSkillToolGUI::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CSkillToolGUI::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CSkillToolGUI::Render()
{
	After_Init();

	View_Base();
	
	View_Skill_Layer();
	View_Effect_Layer();

	Reload_Skill_Data(); ImGui::SameLine();
	Reload_Effect_Data();

	return S_OK;
}


void CSkillToolGUI::View_Base()
{
	ImGui::Begin("View_Base");
		ImGui::Text("View_Base");
	ImGui::End();
}

void CSkillToolGUI::View_Skill_Layer()
{
	ImGui::Begin("View_Skill_Layer");
	{
		if (m_ManagerInit)
			ListBox_Skill_List();
	}
	ImGui::End();
}

void CSkillToolGUI::View_Skill_Depend_Layer()
{
	ImGui::Begin("View_Skill_Depend_Layer");
	{
		if (m_ManagerInit)
			ListBox_Skill_Depend_List();
	}
	ImGui::End();
}

void CSkillToolGUI::View_Effect_Layer()
{
	ImGui::Begin("View_Effect_Layer");
	{
		if (m_ManagerInit)
			ListBox_Effect_List();
	}
	ImGui::End();
}

void CSkillToolGUI::ListBox_Skill_List()
{
	ImGui::Text("ListBox_Skill_List");
	if (ImGui::ListBox(" ", &m_iSkillListBoxCurrentItem, m_SkillListBox, m_SkillListBoxSize))
	{

	}
}

void CSkillToolGUI::ListBox_Skill_Depend_List()
{
	ImGui::Text("ListBox_Skill_Depend_List");
	if (ImGui::ListBox(" ", &m_iSkillDependListBoxCurrentItem, m_SkillDependListBox, m_SkillDependListBoxSize))
	{

	}
}

void CSkillToolGUI::ListBox_Effect_List()
{
	ImGui::Text("ListBox_Effect_List");
	if (ImGui::ListBox(" ", &m_iEffectListCurrentItem, m_EffectListBox, m_EffectListBoxSize))
	{
	
	}
}

void CSkillToolGUI::Player_Skill_Change()
{
}

void CSkillToolGUI::Reload_Skill_Data()
{
	if (ImGui::Button("Reload_Skill_Data"))
	{
		if (nullptr != m_pSkill_Manager)
		{
			m_pSkill_Manager->Reload_Datas();
		}
	}
}

void CSkillToolGUI::Reload_Effect_Data()
{
	if (ImGui::Button("Reload_Effect_Data"))
	{
		if (nullptr != m_pEffect_Manager)
		{
			m_pEffect_Manager->Reload_Datas();
		}
	}
}


void CSkillToolGUI::SkillListBox_Free()
{
	for (size_t i = 0; i < m_SkillListBoxSize; ++i)
	{
		Safe_Delete_Array(m_SkillListBox[i]);
	}
	Safe_Delete_Array(m_SkillListBox);
}

void CSkillToolGUI::SkillDependListBox_Free()
{
	for (size_t i = 0; i < m_SkillDependListBoxSize; ++i)
	{
		Safe_Delete_Array(m_SkillDependListBox[i]);
	}
	Safe_Delete_Array(m_SkillDependListBox);
}

void CSkillToolGUI::EffectListBox_Free()
{
	for (size_t i = 0; i < m_EffectListBoxSize; ++i)
	{
		Safe_Delete_Array(m_EffectListBox[i]);
	}
	Safe_Delete_Array(m_EffectListBox);
}

void CSkillToolGUI::Update_Skill_List()
{
	vector<CSkill::SKILL_DESC> Skill_Desc_Datas;
	m_pSkill_Manager->Get_Skill_Desces(Skill_Desc_Datas);


}

void CSkillToolGUI::Update_Skill_Depend_List()
{
	vector<CSkill_Manager::SKILL_DEPEND_DATA> Skill_Depend_Datas;
	m_pSkill_Manager->Get_Skill_Depends(Skill_Depend_Datas);

}

void CSkillToolGUI::Update_Effect_List()
{
	vector<CEffect::EFFECT_DESC> Effect_Descs;
	m_pEffect_Manager->Get_Effect_Desces(Effect_Descs);

}


void CSkillToolGUI::After_Init()
{
	if (false == m_ManagerInit)
	{
		CGameObject* pEffect_Manager = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager");
		if (pEffect_Manager == nullptr)
			return;

		CGameObject* pSkill_Manager = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
		if (pSkill_Manager == nullptr)
			return;

		m_pEffect_Manager = dynamic_cast<CEffect_Manager*>(pEffect_Manager);
		m_pSkill_Manager = dynamic_cast<CSkill_Manager*>(pSkill_Manager);
		Safe_AddRef(m_pEffect_Manager);
		Safe_AddRef(m_pSkill_Manager);

		Update_Skill_List();
		Update_Skill_Depend_List();
		Update_Effect_List();

		m_ManagerInit = true;
	}
}

void CSkillToolGUI::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	SkillListBox_Free();
	SkillDependListBox_Free();
	EffectListBox_Free();

	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pSkill_Manager);
}

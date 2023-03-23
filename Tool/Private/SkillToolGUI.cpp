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
	View_Base();
	
	View_Skill_Layer();
	View_Effect_Layer();

	Reload_Skill_Data();
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
}

void CSkillToolGUI::Reload_Effect_Data()
{
}


void CSkillToolGUI::SkillListBox_Free()
{
	for (size_t i = 0; i < m_SkillListBoxSize; ++i)
	{
		Safe_Delete_Array(m_SkillListBox[i]);
	}
	Safe_Delete_Array(m_SkillListBox);
}

void CSkillToolGUI::EffectListBox_Free()
{
	for (size_t i = 0; i < m_EffectListBoxSize; ++i)
	{
		Safe_Delete_Array(m_EffectListBox[i]);
	}
	Safe_Delete_Array(m_EffectListBox);
}


void CSkillToolGUI::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	SkillListBox_Free();
	EffectListBox_Free();
}

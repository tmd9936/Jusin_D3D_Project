#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "SkillToolGUI.h"
#include "DataToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "Transform.h"

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

	//View_Base();
	
	View_Skill_Layer();
	//View_Skill_Depend_Layer();
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
	ImGui::Begin("Skill");
	{
		if (m_ManagerInit)
		{
			ListBox_Skill_List();
			ListBox_Skill_Depend_List();
			Skill_Info();
		}
	}
	ImGui::End();
}

void CSkillToolGUI::View_Skill_Depend_Layer()
{
	//ImGui::Begin("View_Skill_Depend_Layer");
	//{
	//	if (m_ManagerInit)
	//		ListBox_Skill_Depend_List();
	//}
	//ImGui::End();
}

void CSkillToolGUI::View_Effect_Layer()
{
	ImGui::Begin("Effect");
	{
		if (m_ManagerInit)
		{
			ListBox_Effect_List();
			Effect_Info();
		}
	}
	ImGui::End();
}

void CSkillToolGUI::ListBox_Skill_List()
{
	if (ImGui::ListBox("Skill_List ", &m_iSkillListBoxCurrentItem, m_SkillListBox, (int)m_SkillListBoxSize))
	{
		ToString_Skill_Info(m_iSkillListBoxCurrentItem);
		Player_Skill_Change(m_iSkillListBoxCurrentItem);
		m_iSkillDependListBoxCurrentItem = m_iSkillListBoxCurrentItem;
	}
}

void CSkillToolGUI::ListBox_Skill_Depend_List()
{
	if (ImGui::ListBox("Skill_Depend_List", &m_iSkillDependListBoxCurrentItem, m_SkillDependListBox, (int)m_SkillDependListBoxSize))
	{
		ToString_Skill_Info(m_iSkillDependListBoxCurrentItem);
		Player_Skill_Change(m_iSkillDependListBoxCurrentItem);
		m_iSkillListBoxCurrentItem = m_iSkillDependListBoxCurrentItem;
	}
}

void CSkillToolGUI::ListBox_Effect_List()
{
	ImGui::Text("ListBox_Effect_List");
	if (ImGui::ListBox(" ", &m_iEffectListCurrentItem, m_EffectListBox, (int)m_EffectListBoxSize))
	{
		ToString_Effect_Info(m_iEffectListCurrentItem);
	}
}

void CSkillToolGUI::Player_Skill_Change(_uint SkillType)
{
	const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

	CGameObject* pPlayer = CGameInstance::GetInstance()->Get_Object(iLevelindex, L"Layer_Player", L"Player1");

	if (pPlayer == nullptr)
		return;

	dynamic_cast<CPlayer*>(pPlayer)->Set_TestSkillindex(SkillType);

}

void CSkillToolGUI::ToString_Skill_Info(_uint SkillType)
{
	if (SkillType >= m_Skill_Desc_Datas.size())
		return;
	wstring desc_info;
	desc_info += L"m_skillPath: " + m_Skill_Desc_Datas[SkillType].m_skillPath + L"\n" +
		L"m_damagePercent: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_damagePercent) + L"\n" +
		L"m_chargeSecond: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_chargeSecond) + L"\n" +
		L"m_rapidDelay: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_rapidDelay) + L"\n" +
		L"m_isEnablePotential_Nway: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Nway) + L"\n" +
		L"m_isEnablePotential_Distance: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Distance) + L"\n" +
		L"m_isEnablePotential_Extend: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Extend) + L"\n" +
		L"m_isEnablePotential_Homing: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Homing) + L"\n" +
		L"m_isEnablePotential_Continue: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Continue) + L"\n" +
		L"m_isEnablePotential_Rapid: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Rapid) + L"\n" +
		L"m_isEnablePotential_Charge: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Charge) + L"\n" +
		L"m_isEnablePotential_ConditionDuration: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_ConditionDuration) + L"\n" +
		L"m_isEnablePotential_BuffProbUp: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_BuffProbUp) + L"\n" +
		L"m_isEnablePotential_DebuffProbUp: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_DebuffProbUp) + L"\n" +
		L"m_isEnablePotential_BuffTimeUp: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_BuffTimeUp) + L"\n" +
		L"m_isEnablePotential_BuffShare: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_BuffShare) + L"\n" +
		L"m_isEnablePotential_Knockback: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Knockback) + L"\n" +
		L"m_isEnablePotential_Damage: " + to_wstring(m_Skill_Desc_Datas[SkillType].m_isEnablePotential_Damage);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	m_Skill_Desc_Info = convert.to_bytes(desc_info);

	//======================================

	wstring data_info;
	
	data_info += L"Normal Effects\n";
	for (auto& index : m_Skill_Depend_Datas[SkillType].m_effects)
	{
		if (index < m_Effect_Descs.size())
		{
			data_info += m_Effect_Descs[index].m_effectPath + L"\n";
		}
	}

	data_info += L"\nCondition Effects\n";
	for (auto& index : m_Skill_Depend_Datas[SkillType].m_conditions)
	{
		if (index < m_Effect_Descs.size())
		{
			data_info += m_Effect_Descs[index].m_effectPath + L"\n";
		}
	}

	m_Skill_Depend_Info = convert.to_bytes(data_info);
}

void CSkillToolGUI::ToString_Effect_Info(_uint EffectType)
{
	if (EffectType >= m_Effect_Descs.size())
		return;
	wstring desc_info;
	desc_info += L"m_effectPath: " + m_Effect_Descs[EffectType].m_effectPath + L"\n" +
		L"m_exPath1: " + m_Effect_Descs[EffectType].m_exPath1 + L"\n" +
		L"m_exPath2: " + m_Effect_Descs[EffectType].m_exPath2 + L"\n" +
		L"m_effectType: " + to_wstring(m_Effect_Descs[EffectType].m_effectType) + L"\n" +
		L"m_actionType: " + to_wstring(m_Effect_Descs[EffectType].m_actionType) + L"\n" +
		L"m_soundEventID: " + to_wstring(m_Effect_Descs[EffectType].m_soundEventID) + L"\n" +
		L"m_soundEventTag: " + m_Effect_Descs[EffectType].m_soundEventTag + L"\n" +
		L"m_underFlag: " + to_wstring(m_Effect_Descs[EffectType].m_underFlag) + L"\n";

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	m_Effect_Desc_Info = convert.to_bytes(desc_info);
}

void CSkillToolGUI::Skill_Info()
{
	ImGui::NewLine();
	ImGui::Text(m_Skill_Desc_Info.c_str());
	ImGui::NewLine();
	ImGui::Text(m_Skill_Depend_Info.c_str());
}

void CSkillToolGUI::Effect_Info()
{
	ImGui::NewLine();
	ImGui::Text(m_Effect_Desc_Info.c_str());
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
	SkillListBox_Free();
	m_pSkill_Manager->Get_Skill_Desces(m_Skill_Desc_Datas);

	m_SkillListBoxSize = m_Skill_Desc_Datas.size();

	m_SkillListBox = new char* [m_SkillListBoxSize];

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (size_t i = 0; i < m_SkillListBoxSize; ++i)
	{
		m_SkillListBox[i] = new char[MAX_PATH];
		string skill_Path = to_string(i) + ": ";
		skill_Path += convert.to_bytes(m_Skill_Desc_Datas[i].m_skillPath);
		strcpy(m_SkillListBox[i], skill_Path.c_str());
	}

}

void CSkillToolGUI::Update_Skill_Depend_List()
{
	SkillDependListBox_Free();

	m_pSkill_Manager->Get_Skill_Depends(m_Skill_Depend_Datas);

	m_SkillDependListBoxSize = m_Skill_Depend_Datas.size();

	m_SkillDependListBox = new char* [m_SkillDependListBoxSize];

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (size_t i = 0; i < m_SkillDependListBoxSize; ++i)
	{
		m_SkillDependListBox[i] = new char[MAX_PATH];
		string skill_Depend = to_string(i) + ": ";
		skill_Depend += "effect: ";
		for (size_t j = 0; j < m_Skill_Depend_Datas[i].m_effects.size(); j++)
		{
			skill_Depend += to_string(m_Skill_Depend_Datas[i].m_effects[j]);
			skill_Depend += ",";
		}
		skill_Depend += " condition: ";
		for (size_t j = 0; j < m_Skill_Depend_Datas[i].m_conditions.size(); j++)
		{
			skill_Depend += to_string(m_Skill_Depend_Datas[i].m_conditions[j]);
			skill_Depend += ",";
		}

		strcpy(m_SkillDependListBox[i], skill_Depend.c_str());
	}

}

void CSkillToolGUI::Update_Effect_List()
{
	EffectListBox_Free();
	m_pEffect_Manager->Get_Effect_Desces(m_Effect_Descs);

	m_EffectListBoxSize = m_Effect_Descs.size();

	m_EffectListBox = new char* [m_EffectListBoxSize];

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (size_t i = 0; i < m_EffectListBoxSize; ++i)
	{
		m_EffectListBox[i] = new char[MAX_PATH];
		string effect_Path = to_string(i) + ": ";
		effect_Path +=	convert.to_bytes(m_Effect_Descs[i].m_effectPath);
		strcpy(m_EffectListBox[i], effect_Path.c_str());
	}

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

	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pSkill_Manager);

	SkillDependListBox_Free();
	EffectListBox_Free();
	SkillListBox_Free();
}

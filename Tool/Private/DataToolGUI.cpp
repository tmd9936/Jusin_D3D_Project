#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "DataToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"

#include <codecvt>

IMPLEMENT_SINGLETON(CDataToolGUI)

CDataToolGUI::CDataToolGUI()
{
}

CDataToolGUI::~CDataToolGUI()
{
}

HRESULT CDataToolGUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	m_pDevice = pDevice;
	m_pContext = pContext;

	for (size_t i = 0; i < LEVEL_END; ++i)
	{
		m_LevelListBox[i] = new char[MAX_PATH];
		sprintf(m_LevelListBox[i], "%zd", i);
	}



	return S_OK;
}

_uint CDataToolGUI::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CDataToolGUI::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CDataToolGUI::Render()
{
	//View_Base();
	View_Level_Layer();
	View_Prefab();

	return S_OK;
}

void CDataToolGUI::View_Base()
{
	ImGui::Begin("View_Base");
	ImGui::Text("View_Base");
	ImGui::End();
}

void CDataToolGUI::View_Level_Layer()
{
	ImGui::Begin("View_Level_Layer");
	ListBox_Level_List();
	ListBox_Layer_List();
	ImGui::End();
}

void CDataToolGUI::View_Prefab()
{
	ImGui::Begin("View_Prefab");

	ImGui::Text("View_Prefab");
	if (ImGui::Button("Update Prefab"))
	{
		Update_PrefabList();
	}

	if (ImGui::ListBox("  ", &m_iPrefabListCurrentItem, m_PrefabListBox, m_PrefabListBoxSize))
	{

	}


	ImGui::End();
}

void CDataToolGUI::ListBox_Level_List()
{
	ImGui::Text("Level List");
	if (ImGui::ListBox(" ", &m_iLevelListBoxCurrentItem, m_LevelListBox, LEVEL_END))
	{
		Update_LayerList();

	}
}

void CDataToolGUI::ListBox_Layer_List()
{
	ImGui::Text("Layer List");
	if (ImGui::ListBox("  ", &m_iLayerListCurrentItem, m_LayerListBox, m_LayerListBoxSize))
	{

	}
}

void CDataToolGUI::Slider()
{
}

void CDataToolGUI::Radio()
{
}

void CDataToolGUI::FileMenuBar()
{
}

void CDataToolGUI::FileMenu()
{
}

void CDataToolGUI::Update_LayerList()
{
	LayerListBox_Free();

	vector<wstring> vecNames;
	if (FAILED(CGameInstance::GetInstance()->Get_Layer_Names(m_iLevelListBoxCurrentItem, vecNames)))
	{
		return;
	}

	m_LayerListBoxSize = vecNames.size();

	if (vecNames.empty())
	{
		return;
	}

	m_LayerListBox = new char* [vecNames.size()];
	m_iLayerListCurrentItem = 0;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (size_t i = 0; i < vecNames.size(); ++i)
	{
		string layerName = convert.to_bytes(vecNames[i].c_str());
		m_LayerListBox[i] = new char[MAX_PATH];
		strcpy(m_LayerListBox[i], layerName.c_str());
	}
}

void CDataToolGUI::Update_PrefabList()
{
	PrefabListBox_Free();

	const unordered_map<const _tchar*, class CGameObject*>* protos = CGameInstance::GetInstance()->Get_GameObject_Prototypes();

	if (nullptr == protos)
	{
		return;
	}

	m_PrefabListBoxSize = protos->size();

	if (protos->empty())
	{
		return;
	}

	m_PrefabListBox = new char* [protos->size()];
	m_iPrefabListCurrentItem = 0;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	size_t index = 0;
	for (auto iter = protos->begin(); iter != protos->end(); ++iter)
	{
		string layerName = convert.to_bytes(iter->first);
		m_PrefabListBox[index] = new char[MAX_PATH];
		strcpy(m_PrefabListBox[index], layerName.c_str());

		++index;
	}

}

void CDataToolGUI::LayerListBox_Free()
{
	for (size_t i = 0; i < m_LayerListBoxSize; ++i)
	{
		Safe_Delete_Array(m_LayerListBox[i]);
	}
	Safe_Delete_Array(m_LayerListBox);
}

void CDataToolGUI::PrefabListBox_Free()
{
	for (size_t i = 0; i < m_PrefabListBoxSize; ++i)
	{
		Safe_Delete_Array(m_PrefabListBox[i]);
	}
	Safe_Delete_Array(m_PrefabListBox);
}

void CDataToolGUI::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	LayerListBox_Free();
	PrefabListBox_Free();

	for (size_t i = 0; i < LEVEL_END; ++i)
	{
		delete m_LevelListBox[i];
		m_LevelListBox[i] = nullptr;
	}

}

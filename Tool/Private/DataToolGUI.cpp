#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "DataToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "MapToolGUI.h"
#include "Level_Loading.h"

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
	Check_Level_Change();
	//View_Base();
	View_Level_Layer();
	View_Level_Objects();
	View_Prefab();
	View_Control();

	return S_OK;
}

const wstring* CDataToolGUI::Get_Current_GameObject_Prefab() const
{
	if (m_CurrentPrefabName.empty())
		return nullptr;
	
	return &m_CurrentPrefabName;
}

const _uint CDataToolGUI::Get_Current_Levelindex() const
{
	return m_iLevelListBoxCurrentItem;
}

const wstring* CDataToolGUI::Get_Current_LayerName() const
{
	if (m_CurrentLayerName.empty())
		return nullptr;

	return &m_CurrentLayerName;
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
	{
		ListBox_Level_List();
		ListBox_Layer_List();
	}
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
	

	if (ImGui::ListBox("  ", &m_iPrefabListCurrentItem, m_PrefabListBox, (int)m_PrefabListBoxSize))
	{
		if (m_PrefabListBox)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
			m_CurrentPrefabName = convert.from_bytes(m_PrefabListBox[m_iPrefabListCurrentItem]);

			CMapToolGUI::GetInstance()->Change_ViewerObject(m_CurrentPrefabName, m_iLevelListBoxCurrentItem, m_CurrentLayerName);
		}

	}

	ImGui::End();
}

void CDataToolGUI::View_Control()
{
	ImGui::Begin("View_Control");

	if (ImGui::Button("Remove_All_GameObject_In_Layer"))
	{
		CGameInstance::GetInstance()->Remove_All_GameObject_In_Layer(m_iLevelListBoxCurrentItem, m_CurrentLayerName.c_str());
	}
	ImGui::End();
}

void CDataToolGUI::View_Level_Objects()
{
	ImGui::Begin("View_Level_Objects");
	{
		if (ImGui::Button("Update Objects"))
		{
			Update_LevelGameObjects();
		}

		Tree_Level_Objects();
	}
	ImGui::End();
}

void CDataToolGUI::ListBox_Level_List()
{
	ImGui::Text("Level List");
	if (ImGui::ListBox(" ", &m_iLevelListBoxCurrentItem, m_LevelListBox, LEVEL_END))
	{
		//if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		//{
			Update_LayerList();
			Update_LevelGameObjects();
			if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVEL)m_iLevelListBoxCurrentItem))))
				return;
		//}
	}
}

void CDataToolGUI::ListBox_Layer_List()
{
	ImGui::Text("Layer List");
	if (ImGui::ListBox("  ", &m_iLayerListCurrentItem, m_LayerListBox, (int)m_LayerListBoxSize))
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
		m_CurrentLayerName = convert.from_bytes(m_LayerListBox[m_iLayerListCurrentItem]);
	}
}

void CDataToolGUI::Tree_Level_Objects()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (auto& iter : m_LevelGameObjects)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(iter.first, flags);

		if (opened)
		{
			for (const CGameObject* object : iter.second)
			{
				if (nullptr != object)
				{
					wstring objectNameTag = object->Get_NameTag();
					flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
					bool objectOpen = false;
					string nodeName;
					if (!objectNameTag.empty())
					{
						nodeName = convert.to_bytes(objectNameTag.c_str());
						nodeName += ": < ";
						nodeName += typeid(*object).name();
						nodeName += " >";
					}
					else
					{
						nodeName = "< ";
						nodeName += string(typeid(*object).name());
						nodeName += " >";
					}
					objectOpen = ImGui::TreeNodeEx(nodeName.c_str(), flags);

					if (objectOpen)
					{

						map<FamilyId, const CComponent*> components;
						object->Get_Components(components);
						flags = ImGuiTreeNodeFlags_Leaf;
						for (auto iter = components.begin(); iter != components.end(); ++iter)
						{
							string compoenetName;
							compoenetName += to_string(iter->first);
							compoenetName += " ";
							compoenetName += typeid(*(iter->second)).name();

							bool componentOpen = ImGui::TreeNodeEx(compoenetName.c_str(), flags);
							if (componentOpen)
								ImGui::TreePop();
						}
						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}

	}
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

void CDataToolGUI::Update_LevelGameObjects()
{
	LevelGameObjects_Free();

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	for (size_t i = 0; i < m_LayerListBoxSize; ++i)
	{
		wstring layerName = convert.from_bytes(m_LayerListBox[i]);

		vector<const CGameObject*> objects;

		CGameInstance::GetInstance()->Get_All_GameObject_In_Layer(objects, m_iLevelListBoxCurrentItem, layerName.c_str());

		m_LevelGameObjects.insert({ m_LayerListBox[i] , objects });
	}
}

void CDataToolGUI::LevelGameObjects_Free()
{
	//for (auto iter = m_LevelGameObjects.begin(); iter != m_LevelGameObjects.end();)
	//{
	//	iter->second->clear();
	//	Safe_Delete(iter->second);
	//	iter = m_LevelGameObjects.erase(iter);
	//}
	m_LevelGameObjects.clear();

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

void CDataToolGUI::Check_Level_Change()
{
	m_iLevelListBoxCurrentItem = CGameInstance::GetInstance()->Get_LevelIndex();
	if (m_iPreLevelIndex != m_iLevelListBoxCurrentItem)
	{
		Update_LayerList();
		Update_PrefabList();
		Update_LevelGameObjects();
		m_iPreLevelIndex = m_iLevelListBoxCurrentItem;
	}
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

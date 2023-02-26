#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "DataToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"

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

	ImGui::Text("View_Level_Layer");

	ImGui::End();
}

void CDataToolGUI::View_Prefab()
{
	ImGui::Begin("View_Prefab");

	ImGui::Text("View_Prefab");

	ImGui::End();
}

void CDataToolGUI::ListBox_Level_List()
{
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

void CDataToolGUI::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

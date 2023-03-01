#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MapToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"
#include "FlatTerrain.h"
#include "Calculator.h"
#include "DataToolGUI.h"

IMPLEMENT_SINGLETON(CMapToolGUI)

imgui_addons::ImGuiFileBrowser file_dialog;

CMapToolGUI::CMapToolGUI()
{
	for (int i : m_iListBox)
		ZeroMemory(m_iListBox, sizeof(int));
}

CMapToolGUI::~CMapToolGUI()
{
}

HRESULT CMapToolGUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pCalculator = CCalculator::Create(pDevice, pContext);

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	m_pDevice = pDevice;
	m_pContext = pContext;

	m_iEnvNum = 100;

	m_vecEnvironment.reserve(m_iEnvNum);
	for (size_t i = 0; i <= m_iEnvNum; ++i)
	{
		string* str = new string;
		(*str).append(to_string(i));
		m_vecEnvironment.push_back(str);
	}

	m_iMapNum = 10;

	m_vecMap.reserve(m_iMapNum);
	for (size_t i = 0; i <= m_iMapNum; ++i)
	{
		string* str = new string;
		(*str).append(to_string(i));
		m_vecMap.push_back(str);
	}

	m_iRadio = 1;

	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	m_pRootViewport = IM_NEW(ImGuiViewport);;

	io.Platform_CreateWindow(m_pRootViewport);
	((ImGui_ImplWin32_ViewportData2*)(m_pRootViewport->PlatformUserData))->Hwnd = g_hWnd;
	((ImGui_ImplWin32_ViewportData2*)(m_pRootViewport->PlatformUserData))->HwndOwned = true;

	m_pRootViewport->Size.x = g_iWinSizeX;
	m_pRootViewport->Size.y = g_iWinSizeY;
	m_pRootViewport->Pos.x = 0.f;
	m_pRootViewport->Pos.y = 0.f;
	m_pRootViewport->ID = 2;
	m_pRootViewport->Flags = ImGuiViewportFlags_IsPlatformWindow;

	io.Platform_SetWindowTitle(m_pRootViewport, "Windows");
	//io.Platform_SetWindowSize(m_pRootViewport, ImVec2((float)g_iWinSizeX * 0.75f, (float)g_iWinSizeY * 0.75f));
	io.Platform_SetWindowPos(m_pRootViewport, { 10.f, 40.f });
	//io.Platform_SetWindowAlpha(m_pRootViewport, 0.9f);

	//D3D11_VIEWPORT ViewPort[1];
	//ZeroMemory(ViewPort, sizeof(D3D11_VIEWPORT));
	//UINT iNumViewPorts = { 1 };
	//m_pContext->RSGetViewports(&iNumViewPorts, ViewPort);

	//ViewPort[0].Width = m_pRootViewport->Size.x;
	//ViewPort[0].Height = m_pRootViewport->Size.y;

	//m_pContext->RSSetViewports(1, ViewPort);

	return S_OK;
}

_uint CMapToolGUI::Tick(_double TimeDelta)
{
	Update_Data();

	return _uint();
}

_uint CMapToolGUI::LateTick(_double TimeDelta)
{
	return _uint();
}

void CMapToolGUI::Reder_Begin()
{
	ImGuiPlatformIO& io = ImGui::GetPlatformIO();

	//ImGui_ImplDX11_RenderDrawData(m_pviewport->DrawData);
}

HRESULT CMapToolGUI::Render()
{
	ImGuiPlatformIO& io = ImGui::GetPlatformIO();


	if (m_bRender)
	{
		//ImGui::SetNextWindowBgAlpha(0.5f);

		ImGui::Begin("Map Tool", 0, ImGuiWindowFlags_MenuBar);
		{
			//ImGui::Text("Hello");

			ListBox();

			Radio();

			Slider();

			FileMenuBar();

			FileMenu();

			TerrainMenu();

		}
		ImGui::End();

		ImGui::Begin("Tool2");
		{

			GetCursorPos(&m_ptMouse);
			ScreenToClient(g_hWnd, &m_ptMouse);

			ImGui::Text("MouseX %d", m_ptMouse.x);
			ImGui::Text("MouseY %d", m_ptMouse.y);

		}
		ImGui::End();

	}

	return S_OK;
}

void CMapToolGUI::Reder_End()
{
	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	//ImDrawData* drawData = m_pviewport->DrawData;
	//ImGui_ImplDX11_RenderDrawData(drawData);

}

void CMapToolGUI::Map_Index_Add(const int& index)
{
	m_iMap_current_idx += index;
	if (m_iMap_current_idx < 0)
		m_iMap_current_idx = 0;
	if (m_iMap_current_idx >= int(m_vecMap.size() - size_t(2)))
		m_iMap_current_idx = int(m_vecMap.size() - size_t(2));

	m_iListBox[MAP] = m_iMap_current_idx;
}

void CMapToolGUI::Environment_Index_Add(const int& index)
{
	m_iEnvironment_current_idx += index;
	if (m_iEnvironment_current_idx < 0)
		m_iEnvironment_current_idx = 0;
	if (m_iEnvironment_current_idx >= int(m_vecEnvironment.size() - size_t(2)))
		m_iEnvironment_current_idx = int(m_vecEnvironment.size() - size_t(2));

	m_iListBox[ENVIRONMENT] = m_iEnvironment_current_idx;
}

void CMapToolGUI::Picking_Environment()
{
	if (MOUSE_TAB(MOUSE::LBTN) && Mouse_Pos_In_Platform())
	{

		const wstring* layerTag = CDataToolGUI::GetInstance()->Get_Current_LayerName();
		const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

		m_pPickingObject = m_pCalculator->Picking_Environment_Object(g_hWnd, layerTag->c_str(), iLevelindex);
		
		if (nullptr == m_pPickingObject)
			return;

		CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, m_pPickingObject));

		m_vScale = pTransform->Get_Scaled();
		m_vRot = pTransform->Get_Rotate();
		XMStoreFloat3(&m_vPos, pTransform->Get_State(CTransform::STATE_POSITION));
	}
}

void CMapToolGUI::Remove_PickingObject()
{
	//if (m_pPickingObject)
	//{
	//	m_pPickingObject->Set_Dead();
	//	m_pPickingObject = nullptr;
	//}
}


void CMapToolGUI::ListBox()
{
	static int Environment_current_idx = 0; // Here we store our selection data as an index.
	const char* EnvironmentList_value = m_vecEnvironment[m_iEnvironment_current_idx]->c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("Env List", EnvironmentList_value, 0))
	{
		for (int n = 0; n < m_vecEnvironment.size() - 1; ++n)
		{
			const bool is_selected = (m_iEnvironment_current_idx == n);
			if (ImGui::Selectable(m_vecEnvironment[n]->c_str(), is_selected))
			{
				m_iEnvironment_current_idx = n;
				m_iListBox[ENVIRONMENT] = m_iEnvironment_current_idx;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	static int Map_current_idx = 0; // Here we store our selection data as an index.
	const char* MapList_value = m_vecMap[m_iMap_current_idx]->c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("Map List", MapList_value, 0))
	{
		for (int n = 0; n < m_vecMap.size() - 1; ++n)
		{
			const bool is_selected = (m_iMap_current_idx == n);
			if (ImGui::Selectable(m_vecMap[n]->c_str(), is_selected))
			{
				m_iMap_current_idx = n;
				m_iListBox[MAP] = m_iMap_current_idx;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}


	//static int terrain_current_idx = 0; // Here we store our selection data as an index.
	//const char* terrainList_value = m_vecTerrain[m_iTerrain_current_idx]->c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	//if (ImGui::BeginCombo("TerrainList", terrainList_value, 0))
	//{
	//	for (int n = 0; n < m_vecTerrain.size() - 1; ++n)
	//	{
	//		const bool is_selected = (m_iTerrain_current_idx == n);
	//		if (ImGui::Selectable(m_vecTerrain[n]->c_str(), is_selected))
	//		{
	//			m_iTerrain_current_idx = n;
	//			m_iListBox[TERRAIN] = m_iTerrain_current_idx;
	//			//m_pSelectKey = (items[item_current_idx]);
	//			//wstring w_pSelectKey = (wstring)(*(items[item_current_idx]));
	//		}

	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}
	//	ImGui::EndCombo();
	//}
}

void CMapToolGUI::Slider()
{
	// �����̵��
	if (m_pPickingObject != nullptr)
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, m_pPickingObject));
		
		if (nullptr == pTransform)
			return;
		
		ImGui::Text("Set_Scale");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("ScaleX", &m_vScale.x);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("ScaleY", &m_vScale.y);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("ScaleZ", &m_vScale.z);

		ImGui::Text("Set_Rot");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("RotX", &m_vRot.x);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("RotY", &m_vRot.y);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("RotZ", &m_vRot.z);


		ImGui::Text("Set_Pos");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("PosX", &m_vPos.x);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("PosY", &m_vPos.y);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("PosZ", &m_vPos.z);

		ImGui::PopItemWidth();

		pTransform->Set_Scaled({ m_vScale.x , m_vScale.y, m_vScale.z});
		pTransform->Set_Rotation({ XMConvertToRadians(m_vRot.x), XMConvertToRadians(m_vRot.y), XMConvertToRadians(m_vRot.z) });

		pTransform->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	}
}

void CMapToolGUI::Radio()
{
	static int iRadio = 1;

	ImGui::RadioButton("Enviroment", &m_iRadio, 0); ImGui::SameLine();
	ImGui::RadioButton("Map", &m_iRadio, 1); ImGui::SameLine();
	ImGui::RadioButton("Spawn", &m_iRadio, 2);
	ImGui::RadioButton("Pick_Environment", &m_iRadio, 3); ImGui::SameLine();
	ImGui::RadioButton("Pick_Spawn", &m_iRadio, 4); ImGui::SameLine();
	ImGui::RadioButton("SAVE_LOAD", &m_iRadio, 5);

	iRadio = m_iRadio;
}


void CMapToolGUI::FileMenuBar()
{
	bool open = false, save = false;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Load Cube", NULL))
				open = true;
			if (ImGui::MenuItem("Save Cube", NULL))
				save = true;

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
	if (open)
		ImGui::OpenPopup("Open File");
	if (save)
		ImGui::OpenPopup("Save File");

	/* Optional third parameter. Support opening only compressed rar/zip files.
	 * Opening any other file will show error, return false and won't close the dialog.
	 */
	if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".txt, .data"))
	{
		Load_CubeList();
	}
	if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt, .data"))
	{
		Save_CubeList();

		//Do writing of files based on extension here
	}
}

void CMapToolGUI::FileMenu()
{
	static int clicked = 0;
	bool load = false, save = false;


	if (ImGui::Button("Load Env"))
	{
		load = true;
		m_iRadio = 6;
	}
	ImGui::SameLine();

	if (ImGui::Button("Save Env"))
	{
		save = true;
		m_iRadio = 6;
	}
	ImGui::SameLine();

	if (ImGui::Button("Load Spwan"))
	{
		load = true;
		m_iRadio = 5;
	}
	ImGui::SameLine();

	if (ImGui::Button("Save Spwan"))
	{
		save = true;
		m_iRadio = 5;
	}

	if (load)
		ImGui::OpenPopup("Load File");
	if (save)
		ImGui::OpenPopup("Save File");

	if (file_dialog.showFileDialog("Load File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".txt, .data"))
	{
		if (5 == m_iRadio)
			Load_CubeList();

		else if (6 == m_iRadio)
			Load_EnvironmentList();
	}
	if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt, .data"))
	{
		if (5 == m_iRadio)
			Save_CubeList();

		else if (6 == m_iRadio)
			Save_EnvironmentList();
	}
}

void CMapToolGUI::TerrainMenu()
{
	ImGui::Text("Terrain");
	ImGui::PushItemWidth(100);
	ImGui::InputInt("CntX", &m_iTerrainCntX, 10, 400); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputInt("CntZ", &m_iTerrainCntZ, 10, 400); ImGui::SameLine();
	//ImGui::InputInt("Interval", &m_iTerrainInterval);

	if (ImGui::Button("Change Size"))
	{
		if (m_iTerrainCntX < 10 || m_iTerrainCntZ < 10)
			return;
		CComponent* pTerrain_VIbuffer = CGameInstance::GetInstance()->Get_Component(CVIBuffer_FlatTerrain::familyId, LEVEL_GAMEPLAY, L"Layer_Terrain", L"Terrain");
		if (pTerrain_VIbuffer)
		{
			dynamic_cast<CVIBuffer_FlatTerrain*>(pTerrain_VIbuffer)->ReSize({ (_uint)m_iTerrainCntX , (_uint)m_iTerrainCntZ });
		}
	}
}

void CMapToolGUI::Update_Data()
{
	switch (m_iRadio)
	{
	case ENVIRONMENT:
		Add_Environment();
		Update_ViewerGameObject();
		break;

	case SPAWN:
		break;

	case MAP:
		break;

	case PICKING_ENVIRONMENT:
		Picking_Environment();
		break;

	case PICKING_SPAWN:
		break;
	}
}

void CMapToolGUI::Update_ViewerGameObject()
{
	if (m_pViewerObject)
	{
		_float3 vPos{};
		if (FAILED(Get_Picking_Terrain_Pos(&vPos)))
			return;
	
		CTransform* pTransform = dynamic_cast<CTransform*>(m_pViewerObject->Get_Component(CTransform::familyId));
		pTransform->Set_Pos(vPos.x, 0.f, vPos.z);
	}
}

HRESULT CMapToolGUI::Get_Picking_Terrain_Pos(_float3* pVOutPutPos)
{
	if (pVOutPutPos == nullptr)
		return E_FAIL;

	CVIBuffer_FlatTerrain* ptex = dynamic_cast<CVIBuffer_FlatTerrain*>(CGameInstance::GetInstance()->Get_Component(CVIBuffer_FlatTerrain::familyId, LEVEL_GAMEPLAY, L"Layer_Terrain", L"Terrain"));
	if (nullptr == ptex)
		return E_FAIL;

	CTransform* pTransCom = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, LEVEL_GAMEPLAY, L"Layer_Terrain", L"Terrain"));
	if (nullptr == pTransCom)
		return E_FAIL;


	//_vec3 vPos = m_pCalculator->Picking_OnTerrain(g_hWnd, ptex, ptrans);
	_float2 size = { m_pRootViewport->Size.x,  m_pRootViewport->Size.y };
	_float3 vPos = m_pCalculator->Picking_OnTerrain(g_hWnd, size, ptex, pTransCom);

	CVIBuffer_FlatTerrain* pTerrainBufferCom = dynamic_cast<CVIBuffer_FlatTerrain*>(CGameInstance::GetInstance()->Get_Component(CVIBuffer_FlatTerrain::familyId, LEVEL_GAMEPLAY, L"Layer_Terrain", L"Terrain"));
	if (nullptr == pTerrainBufferCom)
		return E_FAIL;

	_float		fHeight = m_pCalculator->Compute_HeightOnTerrain(&vPos,
		pTerrainBufferCom->Get_VtxPos(),
		pTerrainBufferCom->Get_VtxCntX(),
		pTerrainBufferCom->Get_VtxCntZ());

	vPos.y = fHeight;

	*pVOutPutPos = vPos;

	return S_OK;
}

_bool CMapToolGUI::Mouse_Pos_In_Platform()
{
	if (m_ptMouse.x >= 0 && m_ptMouse.x < g_iWinSizeX &&
		m_ptMouse.y >= 0 && m_ptMouse.y < g_iWinSizeY)
		return true;

	return false;
}

void CMapToolGUI::Add_Environment()
{
	if (MOUSE_TAB(MOUSE::LBTN) && Mouse_Pos_In_Platform())
	{
		if (nullptr == m_pViewerObject)
			return;

		const wstring* prefabName = CDataToolGUI::GetInstance()->Get_Current_GameObject_Prefab();
		const wstring* layerTag = CDataToolGUI::GetInstance()->Get_Current_LayerName();
		const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

		CGameObject* addObject = nullptr;

		CGameInstance::GetInstance()->Add_GameObject(prefabName->c_str(), iLevelindex, layerTag->c_str(), &addObject, nullptr, nullptr);

		if (nullptr == addObject)
			return;

		CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, addObject));

		if (nullptr == pTransform)
			return;

		_float3 vPos{};
		if (FAILED(Get_Picking_Terrain_Pos(&vPos)))
			return;
		pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

		Safe_Release(addObject);
	}

}

HRESULT CMapToolGUI::Change_ViewerObject(const wstring& PrefabName, _uint iLevelindex, const wstring LayerTag, void* pArg)
{
	if (m_pViewerObject)
	{
		m_pViewerObject->Set_Dead();
		Safe_Release(m_pViewerObject);
	}

	CGameInstance::GetInstance()->Add_GameObject(PrefabName.c_str(), iLevelindex, LayerTag.c_str(), &m_pViewerObject, nullptr, pArg);

	if (nullptr == m_pViewerObject)
		return E_FAIL;

	return S_OK;
}

void CMapToolGUI::Save_CubeList()
{
}

void CMapToolGUI::Load_CubeList()
{
}

void CMapToolGUI::Save_EnvironmentList()
{
	vector<CGameObject*> vecObjects;

	//CGameObject::World->Get_ObjectList<CRcTex>(vecObjects, LAYER_ENVIRONMENT);

	if (vecObjects.empty())
	{
		MSG_BOX("Environment List is empty.");
		return;
	}

	wofstream		fout;

	fout.open(file_dialog.selected_path + file_dialog.ext, ios::out);

	if (!fout.fail())
	{
	//	for (auto& iter : vecObjects)
	//	{
	//		//CTransform* transForm = CGameObject::World->Get_Component<CTransform>(iter);

	//		_vec3 vScale, vRot, vPos;
	//		//int iTextureIndex = dynamic_cast<CEnvironment*>(iter)->Get_TextureIndex();

	//		vScale = transForm->m_vScale;
	//		vRot = transForm->m_vAngle;
	//		transForm->Get_Info(INFO_POS, &vPos);

	//		fout << iTextureIndex << L"|"
	//			<< vScale.x << L"|"
	//			<< vScale.y << L"|"
	//			<< vScale.z << L"|"
	//			<< vRot.x << L"|"
	//			<< vRot.y << L"|"
	//			<< vRot.z << L"|"
	//			<< vPos.x << L"|"
	//			<< vPos.y - vScale.y << L"|"
	//			<< vPos.z << endl;
	//	}

		fout.close();
	}
}

void CMapToolGUI::Load_EnvironmentList()
{
	wifstream		fin;
	fin.open(file_dialog.selected_path, ios::in);

	if (!fin.fail())	// ���� ����
	{
	//	//CGameObject::World->Remove_All_Objects_In_Layer(LAYER_ENVIRONMENT);

	//	TCHAR	szTextureIndex[MIN_STR] = L"";
	//	TCHAR	szScaleX[MIN_STR] = L"";
	//	TCHAR	szScaleY[MIN_STR] = L"";
	//	TCHAR	szScaleZ[MIN_STR] = L"";

	//	TCHAR	szRotX[MIN_STR] = L"";
	//	TCHAR	szRotY[MIN_STR] = L"";
	//	TCHAR	szRotZ[MIN_STR] = L"";

	//	TCHAR	szPosX[MIN_STR] = L"";
	//	TCHAR	szPosY[MIN_STR] = L"";
	//	TCHAR	szPosZ[MIN_STR] = L"";

	//	int iTextureIndex = 0;
	//	_vec3 vScale, vRot, vPos;

	//	while (true)
	//	{
	//		// '|' ������ ��� ���ڿ��� �о����
	//		fin.getline(szTextureIndex, MIN_STR, '|');
	//		fin.getline(szScaleX, MIN_STR, '|');
	//		fin.getline(szScaleY, MIN_STR, '|');
	//		fin.getline(szScaleZ, MIN_STR, '|');

	//		fin.getline(szRotX, MIN_STR, '|');
	//		fin.getline(szRotY, MIN_STR, '|');
	//		fin.getline(szRotZ, MIN_STR, '|');

	//		fin.getline(szPosX, MIN_STR, '|');
	//		fin.getline(szPosY, MIN_STR, '|');
	//		fin.getline(szPosZ, MIN_STR);

	//		// ���Ͽ� ���� �����ϸ� true�� ����
	//		if (fin.eof())
	//			break;

	//		iTextureIndex = _tstoi(szTextureIndex);

	//		vScale.x = _float(_tstof(szScaleX));
	//		vScale.y = _float(_tstof(szScaleY));
	//		vScale.z = _float(_tstof(szScaleZ));

	//		vRot.x = _float(_tstof(szRotX));
	//		vRot.y = _float(_tstof(szRotY));
	//		vRot.z = _float(_tstof(szRotZ));

	//		vPos.x = _float(_tstof(szPosX));
	//		vPos.y = _float(_tstof(szPosY));
	//		vPos.z = _float(_tstof(szPosZ));

	//		//CEnvironment::Create(m_pGraphicDev, nullptr, iTextureIndex, vScale, vRot, vPos);
	//	}

		fin.close();
	}
}

void CMapToolGUI::Free(void)
{
	//Safe_Release(m_pVd->RTView);
	//Safe_Release(m_pVd->SwapChain);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	//Safe_Delete(m_pviewport);


	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	//io.Renderer_DestroyWindow(m_pRootViewport);
	io.Platform_DestroyWindow(m_pRootViewport);

	IM_DELETE(m_pRootViewport);

	//Safe_Delete(m_pVd);
	//m_pVd = nullptr;
	//Safe_Delete(m_pPd);
	//m_pPd = nullptr;


	//if ((ImGui_ImplDX11_ViewportData2*)m_pRootViewport->RendererUserData)
	//{
	//	delete (ImGui_ImplDX11_ViewportData2*)m_pRootViewport->RendererUserData;
	//	m_pRootViewport->RendererUserData = nullptr;
	//}

	//if ((ImGui_ImplDX11_ViewportData2*)m_pRootViewport->PlatformUserData)
	//{
	//	delete (ImGui_ImplDX11_ViewportData2*)m_pRootViewport->PlatformUserData;
	//	m_pRootViewport->PlatformUserData = nullptr;
	//}


	//if ((ImGui_ImplDX11_ViewportData2*)m_pviewport->RendererUserData)
	//{
	//	delete (ImGui_ImplDX11_ViewportData2*)m_pviewport->RendererUserData;
	//}

	//if ((ImGui_ImplDX11_ViewportData2*)m_pviewport->PlatformUserData)
	//{
	//	delete (ImGui_ImplDX11_ViewportData2*)m_pviewport->PlatformUserData;
	//}

	//Safe_Release(((ImGui_ImplDX11_ViewportData2*)m_pviewport->RendererUserData)->RTView);
	//Safe_Release(((ImGui_ImplDX11_ViewportData2*)m_pviewport->RendererUserData)->SwapChain);


	for_each(m_vecEnvironment.begin(), m_vecEnvironment.end(), [](string* str) {
		if (str)
		{
			delete str;
			str = nullptr;
		}
	});

	m_vecEnvironment.clear();

	for_each(m_vecMap.begin(), m_vecMap.end(), [](string* str) {
		if (str)
		{
			delete str;
			str = nullptr;
		}
	});

	m_vecMap.clear();

	Safe_Release(m_pCalculator);
	Safe_Release(m_pViewerObject);

}

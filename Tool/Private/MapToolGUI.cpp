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

#include "Camera.h"

#include <codecvt>

using namespace rapidjson;

IMPLEMENT_SINGLETON(CMapToolGUI)

imgui_addons::ImGuiFileBrowser file_dialog;

CMapToolGUI::CMapToolGUI()
{

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

	m_iRadio = 1;

	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	m_pRootViewport = IM_NEW(ImGuiViewport);;

	io.Platform_CreateWindow(m_pRootViewport);
	((ImGui_ImplWin32_ViewportData2*)(m_pRootViewport->PlatformUserData))->Hwnd = g_hWnd;
	((ImGui_ImplWin32_ViewportData2*)(m_pRootViewport->PlatformUserData))->HwndOwned = false;

	m_pRootViewport->Size.x = g_iWinSizeX;
	m_pRootViewport->Size.y = g_iWinSizeY;
	m_pRootViewport->Pos.x = 0.f;
	m_pRootViewport->Pos.y = 0.f;
	m_pRootViewport->ID = 2;
	m_pRootViewport->Flags = ImGuiViewportFlags_IsPlatformWindow;

	io.Platform_SetWindowTitle(m_pRootViewport, "Windows");
	//io.Platform_SetWindowSize(m_pRootViewport, ImVec2((float)g_iWinSizeX * 0.75f, (float)g_iWinSizeY * 0.75f));
	io.Platform_SetWindowPos(m_pRootViewport, { 10.f, 40.f });
	io.Platform_SetWindowAlpha(m_pRootViewport, 1.0f);

	//D3D11_VIEWPORT ViewPort[1];
	//ZeroMemory(ViewPort, sizeof(D3D11_VIEWPORT));
	//UINT iNumViewPorts = { 1 };
	//m_pContext->RSGetViewports(&iNumViewPorts, ViewPort);

	//ViewPort[0].Width = m_pRootViewport->Size.x;
	//ViewPort[0].Height = m_pRootViewport->Size.y;

	//m_pContext->RSSetViewports(1, ViewPort);

	m_LerpDuration = (_float)CModel::Get_LerpDuration();

	return S_OK;
}

_uint CMapToolGUI::Tick(_double TimeDelta)
{
	if (m_bMapToolOpen)
	{
		Update_Data();
	}

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
	m_bMapToolOpen = ImGui::Begin("Map Tool", 0, ImGuiWindowFlags_MenuBar);
	{
		//ListBox();
		FileMenu();

		Radio();

		TerrainMenu();

		Slider();

		//FileMenuBar();

	}
	ImGui::End();

	ImGui::Begin("Tool2");
	{

		GetCursorPos(&m_ptMouse);
		ScreenToClient(g_hWnd, &m_ptMouse);

		ImGui::Text("MousePos: X %d, Y %d", m_ptMouse.x, m_ptMouse.y);
		ImGui::Text("BrushPos: X %.2f, Z %.2f", m_vBrushPos.x, m_vBrushPos.z);

		const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();
		CCamera* pCamera = (CCamera*)CGameInstance::GetInstance()->Get_Object(iLevelindex, L"Layer_Camera", L"Main_Camera");
		if(pCamera)
		{ 
			CCamera::CAMERADESC camera_desc = pCamera->Get_CameraDesc();
			ImGui::Text("Main Camera");

			ImGui::Text("vEye: X %.2f, Y %.2f, Z %.2f", camera_desc.vEye.x, camera_desc.vEye.y, camera_desc.vEye.z);
			ImGui::Text("vAt: X %.2f, Y %.2f, Z %.2f", camera_desc.vAt.x, camera_desc.vAt.y, camera_desc.vAt.z);
			ImGui::Text("vAxisY: %.2f", camera_desc.vAxisY);
			ImGui::Text("fFovy: %.2f, fAspect: %.2f, fNear: %.2f, fFar: %.2f", camera_desc.fFovy, camera_desc.fAspect, camera_desc.fNear, camera_desc.fFar);


		}

		ImGui::Text("Lerp_Duration");
		ImGui::PushItemWidth(100);
		if (ImGui::DragFloat("LerpDuration", &m_LerpDuration, 0.01f))
		{
			CModel::Set_LerpDuration((_double)m_LerpDuration);
		}
		
		if (ImGui::Button("Create Test Nav Data"))
		{
			Create_Navigation_Test_Data();
		}

	}
	ImGui::End();

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
	//m_iMap_current_idx += index;
	//if (m_iMap_current_idx < 0)
	//	m_iMap_current_idx = 0;
	//if (m_iMap_current_idx >= int(m_vecMap.size() - size_t(2)))
	//	m_iMap_current_idx = int(m_vecMap.size() - size_t(2));

	//m_iListBox[MAP] = m_iMap_current_idx;
}

void CMapToolGUI::Picking_GameObject()
{
	if (KEY_TAB(KEY::DEL))
	{
		Remove_PickingObject();
		return;
	}

	else if (MOUSE_TAB(MOUSE::LBTN) && Mouse_Pos_In_Platform())
	{

		const wstring* layerTag = CDataToolGUI::GetInstance()->Get_Current_LayerName();
		const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

		m_pPickingObject = m_pCalculator->Picking_Environment_Object(g_hWnd, layerTag->c_str(), iLevelindex);
		
		if (nullptr == m_pPickingObject)
			return;

		CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, m_pPickingObject));

		m_vScale = pTransform->Get_Scaled();
		m_vRot = pTransform->Get_Rotate();
		m_vRot.x = XMConvertToDegrees(m_vRot.x);
		m_vRot.y = XMConvertToDegrees(m_vRot.y);
		m_vRot.z = XMConvertToDegrees(m_vRot.z);

		XMStoreFloat3(&m_vPos, pTransform->Get_State(CTransform::STATE_POSITION));
	}
}

void CMapToolGUI::Set_Picking_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;

	//const wstring* layerTag = CDataToolGUI::GetInstance()->Get_Current_LayerName();
	//const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

	m_pPickingObject = pGameObject;

	CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, m_pPickingObject));

	m_vScale = pTransform->Get_Scaled();
	m_vRot = pTransform->Get_Rotate();
	m_vRot.x = XMConvertToDegrees(m_vRot.x);
	m_vRot.y = XMConvertToDegrees(m_vRot.y);
	m_vRot.z = XMConvertToDegrees(m_vRot.z);

	XMStoreFloat3(&m_vPos, pTransform->Get_State(CTransform::STATE_POSITION));
}

void CMapToolGUI::Remove_PickingObject()
{
	if (m_pPickingObject)
	{
		m_pPickingObject->Set_Dead();
		//Safe_Release(m_pPickingObject);
		m_pPickingObject = nullptr;
		CDataToolGUI::GetInstance()->Update_LevelGameObjects();
	}
}


void CMapToolGUI::ListBox()
{
	//static int Environment_current_idx = 0; // Here we store our selection data as an index.
	//const char* EnvironmentList_value = m_vecEnvironment[m_iEnvironment_current_idx]->c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	//if (ImGui::BeginCombo("Env List", EnvironmentList_value, 0))
	//{
	//	for (int n = 0; n < m_vecEnvironment.size() - 1; ++n)
	//	{
	//		const bool is_selected = (m_iEnvironment_current_idx == n);
	//		if (ImGui::Selectable(m_vecEnvironment[n]->c_str(), is_selected))
	//		{
	//			m_iEnvironment_current_idx = n;
	//			m_iListBox[ENVIRONMENT] = m_iEnvironment_current_idx;
	//		}

	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}

	//	ImGui::EndCombo();
	//}

}

void CMapToolGUI::Slider()
{
	if (m_pPickingObject != nullptr)
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, m_pPickingObject));
		
		if (nullptr == pTransform)
			return;
		
		ImGui::Text("Set_Scale");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("ScaleX", &m_vScale.x, 0.2f);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("ScaleY", &m_vScale.y, 0.2f);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("ScaleZ", &m_vScale.z, 0.2f);

		ImGui::Text("Set_Rot");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("RotX", &m_vRot.x, 0.2f);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("RotY", &m_vRot.y, 0.2f);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("RotZ", &m_vRot.z, 0.2f);

		ImGui::Text("Set_Pos");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("PosX", &m_vPos.x, 0.2f);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("PosY", &m_vPos.y, 0.2f);
		ImGui::SameLine(); ImGui::SameLine();
		ImGui::DragFloat("PosZ", &m_vPos.z, 0.2f);

		ImGui::PopItemWidth();

		pTransform->Set_Scaled({ m_vScale.x , m_vScale.y, m_vScale.z});
		pTransform->Set_Rotation({ XMConvertToRadians(m_vRot.x), XMConvertToRadians(m_vRot.y), XMConvertToRadians(m_vRot.z) });
		pTransform->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	}
}

void CMapToolGUI::Radio()
{
	static int iRadio = 1;

	ImGui::RadioButton("Picking", &m_iRadio, 0); ImGui::SameLine();
	ImGui::RadioButton("Add", &m_iRadio, 1); ImGui::SameLine();
	ImGui::RadioButton("Map", &m_iRadio, 2); ImGui::SameLine();
	ImGui::RadioButton("SAVE LOAD", &m_iRadio, 3);

	iRadio = m_iRadio;
}


void CMapToolGUI::FileMenuBar()
{
	bool open = false, save = false;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Load Layer GameObjects", NULL))
				open = true;
			if (ImGui::MenuItem("Save Layer GameObjects", NULL))
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
	}
	if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt, .data"))
	{
	}
}

void CMapToolGUI::FileMenu()
{
	static int clicked = 0;
	bool load = false, save = false;

	if (ImGui::Button("Load Layer GameObjects"))
	{
		load = true;
	}
	ImGui::SameLine();

	if (ImGui::Button("Save Layer GameObjects"))
	{
		save = true;
	}

	if (save)
		ImGui::OpenPopup("Save File");
	else if (load)
		ImGui::OpenPopup("Load File");

	if (file_dialog.showFileDialog("Load File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".json, .data"))
	{
		Load_Layer_GameObjects();
	}
	else if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".json, .data"))
	{
		Save_Layer_GameObjects();
	}
}

void CMapToolGUI::TerrainMenu()
{
	ImGui::Text("Terrain");
	ImGui::PushItemWidth(100);
	ImGui::InputInt("CntX", &m_iTerrainCntX, 10, 400); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputInt("CntZ", &m_iTerrainCntZ, 10, 400); ImGui::SameLine();

	if (ImGui::Button("Change Size"))
	{
		if (m_iTerrainCntX < 10 || m_iTerrainCntZ < 10)
			return;
		CComponent* pTerrain_VIbuffer = CGameInstance::GetInstance()->Get_Component(CVIBuffer_FlatTerrain::familyId, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain");
		if (pTerrain_VIbuffer)
		{
			dynamic_cast<CVIBuffer_FlatTerrain*>(pTerrain_VIbuffer)->ReSize({ (_uint)m_iTerrainCntX , (_uint)m_iTerrainCntZ });
		}
	}

	if (ImGui::Button("Terrain Mask Size Update"))
	{
		CTexture* pMaskTexture = dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Get_Component(FAMILY_ID_TEXTURE_MASK, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
		if (nullptr == pMaskTexture)
			return;

		pMaskTexture->Create_New_Texture(0, (_uint)m_iTerrainCntX, (_uint)m_iTerrainCntZ);

		Terrain_Mask_Pixels_Copy();
	} ImGui::SameLine();

	if (ImGui::Button("Terrain Mask Data Copy"))
	{
		Terrain_Mask_Pixels_Copy();
	}

	if (ImGui::Button("Terrain_Wire_On_Off"))
	{
		const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

		CGameObject* pTerrain = CGameInstance::GetInstance()->Get_Object(iLevelindex, L"Layer_Terrain", L"Terrain");
		if (pTerrain)
			dynamic_cast<CFlatTerrain*>(pTerrain)->Switch_Wire();

	}
}

void CMapToolGUI::Update_Data()
{
	ImGuiHoveredFlags flag = ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_RootWindow;
	if (ImGui::IsWindowHovered(flag))
	{
		return;
	}

	if (ImGui::IsItemHovered(flag))
	{
		return;
	}

	if (ImGui::IsAnyItemHovered() || ImGui::IsItemFocused())
	{
		return;
	}

	if (ImGui::IsAnyItemActive())
	{
		return;
	}

	switch (m_iRadio)
	{
	case PICKING:
		Picking_GameObject();
		break;

	case ADD:
		Add_GameObject();
		Update_ViewerGameObject();
		break;

	case MAP:
		Move_Brush();
		break;

	case SAVE_LOAD:
		break;
	}
}

void CMapToolGUI::Update_ViewerGameObject()
{
	if (m_pViewerObject && Mouse_Pos_In_Platform())
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

	CVIBuffer_FlatTerrain* ptex = dynamic_cast<CVIBuffer_FlatTerrain*>(CGameInstance::GetInstance()->Get_Component(CVIBuffer_FlatTerrain::familyId, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
	if (nullptr == ptex)
		return E_FAIL;

	CTransform* pTransCom = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
	if (nullptr == pTransCom)
		return E_FAIL;


	//_vec3 vPos = m_pCalculator->Picking_OnTerrain(g_hWnd, ptex, ptrans);
	_float2 size = { m_pRootViewport->Size.x,  m_pRootViewport->Size.y };
	_float3 vPos = m_pCalculator->Picking_OnTerrain(g_hWnd, size, ptex, pTransCom);

	CVIBuffer_FlatTerrain* pTerrainBufferCom = dynamic_cast<CVIBuffer_FlatTerrain*>(CGameInstance::GetInstance()->Get_Component(CVIBuffer_FlatTerrain::familyId, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
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

void CMapToolGUI::Move_Brush()
{
	Get_Picking_Terrain_Pos(&m_vBrushPos);

	CTexture* pBrushTexture = dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Get_Component(FAMILY_ID_TEXTURE_BRUSH, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
	if (nullptr == pBrushTexture)
		return;

	pBrushTexture->Set_BrushPos({ m_vBrushPos.x, m_vBrushPos.y, m_vBrushPos.z, 1.f });

	if (MOUSE_TAB(MOUSE::LBTN) && Mouse_Pos_In_Platform())
	{
		if (nullptr == pTerrainMaskPixel)
			Terrain_Mask_Pixels_Copy();

		CTexture* pMaskTexture = dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Get_Component(FAMILY_ID_TEXTURE_MASK, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
		if (nullptr == pMaskTexture)
			return;

		pTerrainMaskPixel[_uint(m_vBrushPos.z * m_iTerrainCntX + m_vBrushPos.x)] = D3DCOLOR_ARGB(255, 0, 0, 0);
		pMaskTexture->Update_Texture_Pixels_Info(0, pTerrainMaskPixel);

		Terrain_Mask_Pixels_Copy();
	}

	else if (MOUSE_TAB(MOUSE::RBTN) && Mouse_Pos_In_Platform())
	{
		if (nullptr == pTerrainMaskPixel)
			Terrain_Mask_Pixels_Copy();

		CTexture* pMaskTexture = dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Get_Component(FAMILY_ID_TEXTURE_MASK, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
		if (nullptr == pMaskTexture)
			return;

		pTerrainMaskPixel[_uint(m_vBrushPos.z * m_iTerrainCntX + m_vBrushPos.x)] = D3DCOLOR_ARGB(255, 255, 255, 255);
		pMaskTexture->Update_Texture_Pixels_Info(0, pTerrainMaskPixel);

		Terrain_Mask_Pixels_Copy();
	}

}

HRESULT CMapToolGUI::Terrain_Mask_Pixels_Copy()
{
	Safe_Delete_Array(pTerrainMaskPixel);

	CTexture* pMaskTexture = dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Get_Component(FAMILY_ID_TEXTURE_MASK, LEVEL_BASECAMP, L"Layer_Terrain", L"Terrain"));
	if (nullptr == pMaskTexture)
		return E_FAIL;

	pMaskTexture->Copy_Texture_Pixels(0, &pTerrainMaskPixel);

	return S_OK;
}

HRESULT CMapToolGUI::Create_Navigation_Test_Data()
{
	/* Navigation */
	HANDLE		hFile = 0;
	_ulong		dwByte = 0;

	hFile = CreateFile(TEXT("../Bin/DataFiles/NavigationData.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3		vPoints[3];

	/* 0번째 삼각형 */
	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(0.0f, 0.f, 5.f);
	vPoints[1] = _float3(5.0f, 0.f, 0.f);
	vPoints[2] = _float3(0.0f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	/* 1번째 삼각형 */
	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(0.0f, 0.f, 5.f);
	vPoints[1] = _float3(5.0f, 0.f, 5.0f);
	vPoints[2] = _float3(5.0f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	/* 2번째 삼각형 */
	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(0.0f, 0.f, 10.0f);
	vPoints[1] = _float3(5.0f, 0.f, 5.0f);
	vPoints[2] = _float3(0.f, 0.f, 5.0f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	/* 3번째 삼각형 */
	ZeroMemory(vPoints, sizeof(_float3) * 3);
	vPoints[0] = _float3(5.0f, 0.f, 5.0f);
	vPoints[1] = _float3(10.0f, 0.f, 0.0f);
	vPoints[2] = _float3(5.0f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

void CMapToolGUI::Add_GameObject()
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

		CDataToolGUI::GetInstance()->Update_LevelGameObjects();

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

void CMapToolGUI::Save_Layer_GameObjects()
{

	vector<CGameObject*> vecObjects;

	const wstring* prefabName = CDataToolGUI::GetInstance()->Get_Current_GameObject_Prefab();
	const wstring* layerTag = CDataToolGUI::GetInstance()->Get_Current_LayerName();
	const _uint iLevelindex = CDataToolGUI::GetInstance()->Get_Current_Levelindex();

	CGameInstance::GetInstance()->Get_ObjectList<CTransform>(vecObjects, iLevelindex, layerTag->c_str());

	if (vecObjects.empty())
	{
		MSG_BOX("GameObjects List is empty.");
		return;
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Document doc(kObjectType);

	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("Level", iLevelindex, allocator);

	string layerName = convert.to_bytes(layerTag->c_str());
	doc.AddMember("Layer", Value().SetString(StringRef(layerName.c_str())), allocator);

	_float3 vScale{}, vRot{}, vPos{};

	Document Objects;
	Value o(kArrayType);

	for (auto& iter : vecObjects)
	{
		if (iter == m_pViewerObject)
			continue;

		Value contacts(kObjectType);
		{
			CTransform* pTransform = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, iter));

			if (nullptr == pTransform)
				continue;

			vScale = pTransform->Get_Scaled();
			vRot = pTransform->Get_Rotate();
			XMStoreFloat3(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));

			contacts.AddMember("ScaleX", vScale.x, allocator);
			contacts.AddMember("ScaleY", vScale.y, allocator);
			contacts.AddMember("ScaleZ", vScale.z, allocator);

			contacts.AddMember("RotX", vRot.x, allocator);
			contacts.AddMember("RotY", vRot.y, allocator);
			contacts.AddMember("RotZ", vRot.z, allocator);

			contacts.AddMember("PosX", vPos.x, allocator);
			contacts.AddMember("PosY", vPos.y, allocator);
			contacts.AddMember("PosZ", vPos.z, allocator);

			Value ProtoTypeTag;
			char ProtoTypeTagbuffer[MAX_PATH];
			int ProtoTypeTaglen = sprintf(ProtoTypeTagbuffer, convert.to_bytes(iter->Get_ProtoTypeTag()).c_str()); // dynamically created string.
			ProtoTypeTag.SetString(ProtoTypeTagbuffer, ProtoTypeTaglen, allocator);
			memset(ProtoTypeTagbuffer, 0, sizeof(ProtoTypeTagbuffer));
			contacts.AddMember("PrototypeTag", ProtoTypeTag, allocator);

			Value ObjectNameTag;
			char ObjectNameTagbuffer[MAX_PATH];
			int ObjectNameTaglen = sprintf(ObjectNameTagbuffer, convert.to_bytes(iter->Get_NameTag()).c_str()); // dynamically created string.
			ObjectNameTag.SetString(ObjectNameTagbuffer, ObjectNameTaglen, allocator);
			memset(ObjectNameTagbuffer, 0, sizeof(ObjectNameTagbuffer));
			contacts.AddMember("ObjectNameTag", ObjectNameTag, allocator);
		}
		o.PushBack(contacts, allocator);

	}

	doc.AddMember("Objects", o, allocator);

	FILE* fp = fopen(string(file_dialog.selected_path + file_dialog.ext).c_str(), "wb"); // non-Windows use "w"

	if (NULL == fp)
		MSG_BOX("Save File Open Error");
	else
	{
		char* writeBuffer = new char[65536];
		FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		PrettyWriter<FileWriteStream> writer(os);
		doc.Accept(writer);

		fclose(fp);

		Safe_Delete_Array(writeBuffer);
	}
}

void CMapToolGUI::Load_Layer_GameObjects()
{
	FILE* fp = fopen((file_dialog.selected_path).c_str(), "rb"); // non-Windows use "r"

	if (NULL == fp)
		MSG_BOX("Load File Open Error");
	else
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		_uint	iLevelindex = 0;
		_float3 vScale{}, vRot{}, vPos{};
		wstring objectNameTag, layerTag, prototypeTag;

		CTransform* pTransform = nullptr;

		assert(document.HasMember("Level"));
		assert(document["Level"].IsUint());
		iLevelindex = (_uint)document["Level"].GetUint();

		assert(document.HasMember("Layer"));
		assert(document["Layer"].IsString());
		layerTag = convert.from_bytes(document["Layer"].GetString());

		const Value& objects = document["Objects"];
		assert(objects.IsArray());
		for (SizeType i = 0; i < objects.Size(); ++i)
		{
			CGameObject* pOut = nullptr;

			vScale.x = objects[i]["ScaleX"].GetFloat();
			vScale.y = objects[i]["ScaleY"].GetFloat();
			vScale.z = objects[i]["ScaleZ"].GetFloat();

			vRot.x = objects[i]["RotX"].GetFloat();
			vRot.y = objects[i]["RotY"].GetFloat();
			vRot.z = objects[i]["RotZ"].GetFloat();

			vPos.x = objects[i]["PosX"].GetFloat();
			vPos.y = objects[i]["PosY"].GetFloat();
			vPos.z = objects[i]["PosZ"].GetFloat();

			prototypeTag = convert.from_bytes(objects[i]["PrototypeTag"].GetString());
			objectNameTag = convert.from_bytes(objects[i]["ObjectNameTag"].GetString());


			if (objectNameTag.empty())
				CGameInstance::GetInstance()->Add_GameObject(prototypeTag.c_str(), iLevelindex, layerTag.c_str(), &pOut);
			else
				CGameInstance::GetInstance()->Add_GameObject(prototypeTag.c_str(), iLevelindex, layerTag.c_str(), &pOut, objectNameTag.c_str());

			pTransform = (CTransform*)CGameInstance::GetInstance()->Get_Component(CTransform::familyId, pOut);

			if (nullptr == pTransform)
				continue;

			pTransform->Set_Scaled(vScale);
			pTransform->Set_Rotation(vRot);
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

			Safe_Release(pOut);
		}

		fclose(fp);
		Safe_Delete_Array(readBuffer);

		CDataToolGUI::GetInstance()->Update_LevelGameObjects();
	}
}

void CMapToolGUI::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	//io.Renderer_DestroyWindow(m_pRootViewport);
	io.Platform_DestroyWindow(m_pRootViewport);

	IM_DELETE(m_pRootViewport);

	Safe_Release(m_pCalculator);
	Safe_Release(m_pViewerObject);
	Safe_Release(m_pPickingObject);

	Safe_Delete_Array(pTerrainMaskPixel);

}

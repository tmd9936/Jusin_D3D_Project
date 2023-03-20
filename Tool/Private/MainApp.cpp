#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MainApp.h"
#include "Level_Loading.h"
#include "MapToolGUI.h"
#include "DataToolGUI.h"
#include "GameInstance.h"
#include "Collider.h"
#include "ModelUI.h"
#include "MonFSM.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
	, m_pMapToolGUI(CMapToolGUI::GetInstance())
	, m_pDataToolGUI(CDataToolGUI::GetInstance())
{
	Safe_AddRef(m_pMapToolGUI);
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDataToolGUI);

}

HRESULT CMainApp::Initialize()
{
	srand(time(nullptr));

	GRAPHIC_DESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::MODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iViewSizeX = g_iWinSizeX;
	GraphicDesc.iViewSizeY = g_iWinSizeY;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;

	}
	ImGui_ImplDX11_InitPlatformInterface();

	//ImGui_ImplWin32_EnableAlphaCompositing(g_hWnd);

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	
	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_GameObject_For_Static()))
		return E_FAIL;

	if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(m_pMapToolGUI->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(m_pDataToolGUI->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (m_bToolMode)
	{
		m_pMapToolGUI->Tick(TimeDelta);
	}
	m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainApp::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Tool_Mode_ON_OFF_UI();

	//bool bDemo = true;
	//ImGui::ShowDemoWindow();
	m_pMapToolGUI->Render();
	m_pDataToolGUI->Render();


	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)g_iWinSizeX, (float)g_iWinSizeY);

	ImGuiPlatformIO& PlatformIO = ImGui::GetPlatformIO();

	m_pGameInstance->SetRenderTargets(true);
	//m_pGameInstance->Clear_BackBuffer_View(_float4{ 0.156f, 0.109f, 0.f, 1.f });
	m_pGameInstance->Clear_BackBuffer_View({ g_BackBufferColor.x, g_BackBufferColor.y, g_BackBufferColor.z, g_BackBufferColor.w });
	m_pGameInstance->Clear_DepthStencil_View();

	//m_pMapToolGUI->Reder_End();
	m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->SetRenderTargets(false);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	m_pGameInstance->Present(1, 0);

	return S_OK;
}

void CMainApp::Tool_Mode_ON_OFF_UI()
{
	ImGui::Begin("Tool Mode On/OFF");
	{
		if (m_bToolMode)
			ImGui::Text("Tool Mode ON");
		else
			ImGui::Text("Tool Mode OFF");

		ImGui::ToggleButton("ON/OFF", &m_bToolMode);
	}
	ImGui::End();
}

HRESULT CMainApp::SetUp_StartLevel(LEVEL eNextLevelID)
{		
	return m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevelID));	
}

 
HRESULT CMainApp::Ready_Prototype_Component_For_Static()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_MonFSM */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_MonFSM"),
		CMonFSM::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_PickingCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		CPickingCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModelColor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxModelColor.hlsl"), VTXNORCOLOR_DECLARATION::Elements, VTXNORCOLOR_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModelColor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModelColor.hlsl"), VTXCOLORANIMMODEL_DECLARATION::Elements, VTXCOLORANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	//_matrix PivotMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	//_matrix PivotMatrix = XMMatrixIdentity();
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Loading_Scene"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Resources/Mesh/Animation/Loading/title_loading.fbx", PivotMatrix, true))))
	//	return E_FAIL;

	_matrix PivotMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Loading_Scene"),
		CModel::Create(m_pDevice, m_pContext, "../../Reference/Resources/Mesh/Animation/Loading/title_loading.json", CModel::TYPE_ANIM, PivotMatrix))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
	//	return E_FAIL;


	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_For_Static()
{
	/* For.Prototype_GameObject_ModelUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ModelUI"),
		CModelUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	Safe_Release(m_pMapToolGUI);
	CMapToolGUI::GetInstance()->DestroyInstance();

	Safe_Release(m_pDataToolGUI);
	CDataToolGUI::GetInstance()->DestroyInstance();

	ImGui::DestroyPlatformWindows();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	CGameInstance::Release_Engine();

}

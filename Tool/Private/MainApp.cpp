#include "../Default/stdafx.h"

#include "Client_Defines.h"

#ifdef _IMGUITOOL
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#endif

#include "MainApp.h"
#include "Level_Loading.h"

#ifdef _IMGUITOOL
#include "MapToolGUI.h"
#include "DataToolGUI.h"
#include "SkillToolGUI.h"
#endif

#include "GameInstance.h"
#include "Collider.h"
#include "ModelUI.h"
#include "MonFSM.h"

#include "Client_Utility.h"

#include "ThreadPool.h"

_bool				CMainApp::m_Init = false;

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
#ifdef _IMGUITOOL
	, m_pMapToolGUI(CMapToolGUI::GetInstance())
	, m_pDataToolGUI(CDataToolGUI::GetInstance())
	, m_pSkillToolGUI(CSkillToolGUI::GetInstance())
#endif
{
	Safe_AddRef(m_pGameInstance);

#ifdef _IMGUITOOL
	Safe_AddRef(m_pMapToolGUI);
	Safe_AddRef(m_pDataToolGUI);
	Safe_AddRef(m_pSkillToolGUI);
#endif


}

_uint APIENTRY Ready_Font()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	if (FAILED(pGameInstance->Add_Font(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(),
		TEXT("Font_NanumBarunGothic"), TEXT("../../Reference/Resources/Fonts/kotra.spritefont"))))
		return hr;

	if (FAILED(pGameInstance->Add_Font(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(),
		TEXT("Font_NanumBarunGothicBold"), TEXT("../../Reference/Resources/Fonts/kotra.spritefont"))))
		return hr;

	return hr;
}

HRESULT CMainApp::Initialize()
{
	srand((unsigned int)time(nullptr));

	GRAPHIC_DESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::MODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iViewSizeX = g_iWinSizeX;
	GraphicDesc.iViewSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	CThreadPool::GetInstance()->Start();
	m_Init = true;
	
	//if (FAILED(Ready_Fonts()))
	//	return E_FAIL;

	CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(Ready_Font));

#ifdef _IMGUITOOL
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

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
#endif

	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject_For_Static()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_For_Static()))
		return E_FAIL;

	if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
		return E_FAIL;

#ifdef _IMGUITOOL
	if (FAILED(m_pMapToolGUI->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(m_pDataToolGUI->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(m_pSkillToolGUI->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;
#endif

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	m_pGameInstance->Tick_Engine(TimeDelta);

#ifdef _IMGUITOOL
	if (m_bToolMode)
	{
		m_pMapToolGUI->Tick(TimeDelta);
	}
#endif

#ifdef _IMGUITOOL
	m_TimeAcc += TimeDelta;
#endif // _DEBUG

}

HRESULT CMainApp::Render()
{
#ifdef _IMGUITOOL
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Tool_Mode_ON_OFF_UI();
#endif

	if (CClient_Utility::Mouse_Pos_In_Platform())
	{
#ifdef _IMGUITOOL
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
#endif
		SetCursor(NULL);
	}

#ifdef _IMGUITOOL
	//bool bDemo = true;
	//ImGui::ShowDemoWindow();
	m_pMapToolGUI->Render();
	m_pDataToolGUI->Render();
	m_pSkillToolGUI->Render();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)g_iWinSizeX, (float)g_iWinSizeY);

	ImGuiPlatformIO& PlatformIO = ImGui::GetPlatformIO();
#endif

#ifdef _IMGUITOOL
	m_pGameInstance->SetRenderTargets(true);
#endif
	//m_pGameInstance->Clear_BackBuffer_View(_float4{ 0.156f, 0.109f, 0.f, 1.f });
	m_pGameInstance->Clear_BackBuffer_View({ g_BackBufferColor.x, g_BackBufferColor.y, g_BackBufferColor.z, g_BackBufferColor.w });
	m_pGameInstance->Clear_DepthStencil_View();

	//m_pMapToolGUI->Reder_End();
	m_pRenderer->Draw_RenderGroup();

	if (FAILED(RenderFPS()))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Render_Font(TEXT("Font_NanumBarunGothic"), TEXT("테스트입니다. 화이팅"), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
	//	return E_FAIL;

#ifdef _IMGUITOOL
	m_pGameInstance->SetRenderTargets(false);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	m_pGameInstance->Present(1, 0);
#endif

#ifndef _IMGUITOOL
	m_pGameInstance->Present(0, 0);
#endif

	return S_OK;
}

HRESULT CMainApp::RenderFPS()
{
#ifdef _FPS
	++m_dwNumDraw;

	if (m_TimeAcc >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("fps : %d"), m_dwNumDraw);
		m_TimeAcc = 0.0;
		m_dwNumDraw = 0;
	}

	if (FAILED(m_pGameInstance->Render_Font(TEXT("Font_NanumBarunGothic"), m_szFPS, _float2(0.f, 20.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tool_Mode_ON_OFF_UI()
{
#ifdef _IMGUITOOL
	ImGui::Begin("Tool Mode On/OFF");
	{
		if (m_bToolMode)
			ImGui::Text("Tool Mode ON");
		else
			ImGui::Text("Tool Mode OFF");

		ImGui::ToggleButton("ON/OFF", &m_bToolMode);
	}
	ImGui::End();
#endif
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

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;


	//_matrix PivotMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	//_matrix PivotMatrix = XMMatrixIdentity();
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Loading_Scene"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Resources/Mesh/Animation/Loading/title_loading.fbx", PivotMatrix, true))))
	//	return E_FAIL;

	_matrix PivotMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Loading_Scene"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Resources/Mesh/Animation/Loading/title_loading.fbx", PivotMatrix))))
		return E_FAIL;


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

HRESULT CMainApp::Ready_GameObject_For_Static()
{
	if (FAILED(m_pGameInstance->Add_Layer(LEVEL_STATIC, L"Layer_Manager")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layer(LEVEL_STATIC, L"Layer_Mouse")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_NanumBarunGothic"), TEXT("../../Reference/Resources/Fonts/dovemayouGothic.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_NanumBarunGothicBold"), TEXT("../../Reference/Resources/Fonts/dovemayouGothic.spritefont"))))
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

#ifdef _IMGUITOOL
	Safe_Release(m_pMapToolGUI);
	CMapToolGUI::GetInstance()->DestroyInstance();

	Safe_Release(m_pDataToolGUI);
	CDataToolGUI::GetInstance()->DestroyInstance();

	Safe_Release(m_pSkillToolGUI);
	CSkillToolGUI::GetInstance()->DestroyInstance();

	ImGui::DestroyPlatformWindows();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif


	CThreadPool::GetInstance()->DestroyInstance();

	CGameInstance::Release_Engine();
}

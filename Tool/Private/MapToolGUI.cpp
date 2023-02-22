#include "../Default/stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MapToolGUI.h"
#include "ImGuiFileBrowser.h"

#include "GameInstance.h"

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


	return S_OK;
}

_uint CMapToolGUI::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CMapToolGUI::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CMapToolGUI::Render()
{
	if (m_bRender)
	{
		ImGui::Begin("Map Tool");
		{
			ImGui::Text("Hello");

			ListBox();

			Radio();

			Slider();

			//FileMenuBar();

			FileMenu();

			//TerrainMenu();

			//static int clicked = 0;
			//if (ImGui::Button("Install"))
			//	clicked++;

			//if (clicked & 1)
			//{
			//	m_bInstall = true;
			//	ImGui::SameLine();
			//	ImGui::Text("Thanks for clicking me!");
			//	clicked++;
			//}
			//else
			//	m_bInstall = false;

		}
		ImGui::End();
	}

	return S_OK;
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

void CMapToolGUI::Set_PickingObject(CGameObject* obj)
{
	if (obj == nullptr)
		return;
	m_pPickingObject = obj;

	//CTransform* transform = CGameObject::World->Get_Component<CTransform>(m_pPickingObject);

	//m_vScale = transform->m_vScale;
	//m_vRot = transform->m_vAngle;
	//transform->Get_Info(INFO_POS, &m_vPos);
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
	//if (m_pPickingObject != nullptr)
	//{
		//CTransform* transform = CGameObject::World->Get_Component<CTransform>(m_pPickingObject);
		ImGui::Text("Set_Scale");
		ImGui::PushItemWidth(100);

		ImGui::DragFloat("ScaleX", &m_vScale.x);
		ImGui::SameLine(); ImGui::SameLine();
		//ImGui::Indent(); // �鿩����
		//ImGui::DragFloat("Pos_Y (indented)##1b", &fY);
		ImGui::DragFloat("ScaleY", &m_vScale.y);
		//ImGui::Unindent();
		ImGui::SameLine(); ImGui::SameLine();
		//ImGui::Indent(); // �鿩����
		ImGui::DragFloat("ScaleZ", &m_vScale.z);
		//ImGui::Unindent();

		ImGui::Text("Set_Rot");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("RotX", &m_vRot.x);
		ImGui::SameLine(); ImGui::SameLine();
		//ImGui::Indent(); // �鿩����
		//ImGui::DragFloat("Pos_Y (indented)##1b", &fY);
		ImGui::DragFloat("RotY", &m_vRot.y);
		//ImGui::Unindent();
		ImGui::SameLine(); ImGui::SameLine();
		//ImGui::Indent(); // �鿩����
		ImGui::DragFloat("RotZ", &m_vRot.z);
		//ImGui::Unindent();

		ImGui::Text("Set_Pos");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("PosX", &m_vPos.x);
		ImGui::SameLine(); ImGui::SameLine();
		//ImGui::Indent(); // �鿩����
		//ImGui::DragFloat("Pos_Y (indented)##1b", &fY);
		ImGui::DragFloat("PosY", &m_vPos.y);
		//ImGui::Unindent();
		ImGui::SameLine(); ImGui::SameLine();
		//ImGui::Indent(); // �鿩����
		ImGui::DragFloat("PosZ", &m_vPos.z);
		//ImGui::Unindent();

		ImGui::PopItemWidth();

		//transform->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
		////transform->Rotation(ROT_X, m_vRot.x);
		////transform->Rotation(ROT_Y, m_vRot.y);
		////transform->Rotation(ROT_Z, m_vRot.z);
		//transform->m_vAngle.x = D3DXToRadian(m_vRot.x);
		//transform->m_vAngle.y = D3DXToRadian(m_vRot.y);
		//transform->m_vAngle.z = D3DXToRadian(m_vRot.z);
		//transform->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	//}
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
	//bool open = false, save = false;
	//if (ImGui::BeginMainMenuBar())
	//{
	//	if (ImGui::BeginMenu("Menu"))
	//	{
	//		if (ImGui::MenuItem("Load Cube", NULL))
	//			open = true;
	//		if (ImGui::MenuItem("Save Cube", NULL))
	//			save = true;

	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMainMenuBar();
	//}

	////Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
	//if (open)
	//	ImGui::OpenPopup("Open File");
	//if (save)
	//	ImGui::OpenPopup("Save File");

	///* Optional third parameter. Support opening only compressed rar/zip files.
	// * Opening any other file will show error, return false and won't close the dialog.
	// */
	//if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".txt, .data"))
	//{
	//	Load_CubeList();
	//}
	//if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt, .data"))
	//{
	//	Save_CubeList();

	//	//Do writing of files based on extension here
	//}
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
	//ImGui::Text("Terrain");
	////ImGui::Text("CntX : "); ImGui::SameLine();
	//ImGui::InputInt("CntX", &m_iTerrainCntX);
	//ImGui::InputInt("CntY", &m_iTerrainCntY);
	//ImGui::InputInt("Interval", &m_iTerrainInterval);

	//if (ImGui::Button("Change Size"))
	//{
	//	if (m_iTerrainCntX < 10 || m_iTerrainCntY < 10 || m_iTerrainInterval < 0)
	//		return;
	//	CGameObject* pTerrain = CGameObject::World->Get_Object(L"Terrain", LAYER_TERRAIN);
	//	if (pTerrain)
	//	{
	//		dynamic_cast<CTerrain*>(pTerrain)->Change_TerrainScale(m_iTerrainCntX, m_iTerrainCntY, m_iTerrainInterval);
	//	}
	//}
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
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

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
}

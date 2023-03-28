#include "stdafx.h"
#include "Button.h"

#include "GameInstance.h"

#include "Level_Loading.h"
#include "PartTexture.h"

CButton::CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CButton::CButton(const CButton& rhs)
	: CGameObject(rhs)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_ButtonDesc, pArg, (sizeof m_ButtonDesc) + 2);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	Common_Initialize();

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

HRESULT CButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	Common_Initialize();

	for (auto& part : m_TextureParts)
	{
		part->Set_Parent_Model(m_pModelCom);
	}

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

_uint CButton::Tick(_double TimeDelta)
{
	Button_Motion(TimeDelta);
	Picking_Button();

	
	for (auto& part : m_TextureParts)
	{
		part->Tick(TimeDelta);
	}
	return Change_State();
	//return _uint();
}

_uint CButton::LateTick(_double TimeDelta)
{
	m_TransformMatrix = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(1);

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	for (auto& part : m_TextureParts)
	{
		part->LateTick(TimeDelta);
	}


	return _uint();
}

HRESULT CButton::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_float viewZ = m_pModelCom->Get_ViewZ(i);
		m_pTransformCom->Set_PosZ(viewZ);
		
		if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;


		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);

	}

	return S_OK;
}

_bool CButton::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (m_pTransformCom)
	{
		Value ButtonDesc(kObjectType);
		{
			ButtonDesc.AddMember("m_fX", m_ButtonDesc.m_fX, allocator);
			ButtonDesc.AddMember("m_fY", m_ButtonDesc.m_fY, allocator);
			ButtonDesc.AddMember("m_fSizeX", m_ButtonDesc.m_fSizeX, allocator);
			ButtonDesc.AddMember("m_fSizeY", m_ButtonDesc.m_fSizeY, allocator);

			ButtonDesc.AddMember("m_eModelPrototypLevel", m_ButtonDesc.m_ModelPrototypLevel, allocator);
			ButtonDesc.AddMember("m_ShaderLevelIndex", m_ButtonDesc.m_ShaderLevelIndex, allocator);

			Value m_DiffuseTexturePath;
			string tag = m_ButtonDesc.m_DiffuseTexturePath;
			m_DiffuseTexturePath.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_DiffuseTexturePath", m_DiffuseTexturePath, allocator);

			Value m_MaskPrototypeTag;
			tag = convert.to_bytes(m_ButtonDesc.m_MaskPrototypeTag);
			m_MaskPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_MaskPrototypeTag", m_MaskPrototypeTag, allocator);

			Value m_BrushPrototypeTag;
			tag = convert.to_bytes(m_ButtonDesc.m_BrushPrototypeTag);
			m_BrushPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_BrushPrototypeTag", m_BrushPrototypeTag, allocator);

			Value m_ButtonName;
			tag = convert.to_bytes(m_ButtonDesc.m_ButtonName);
			m_ButtonName.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_ButtonName", m_ButtonName, allocator);

		}
		doc.AddMember("ButtonDesc", ButtonDesc, allocator);

	}

	return true;
}

_bool CButton::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& ButtonDesc = doc["ButtonDesc"];

		m_ButtonDesc.m_fX = ButtonDesc["m_fX"].GetFloat();
		m_ButtonDesc.m_fY = ButtonDesc["m_fY"].GetFloat();
		m_ButtonDesc.m_fSizeX = ButtonDesc["m_fSizeX"].GetFloat();
		m_ButtonDesc.m_fSizeY = ButtonDesc["m_fSizeY"].GetFloat();

		m_ButtonDesc.m_ModelPrototypLevel = ButtonDesc["m_ModelPrototypLevel"].GetUint();
		m_ButtonDesc.m_ShaderLevelIndex = ButtonDesc["m_ShaderLevelIndex"].GetUint();

		string m_DiffuseTexturePath = ButtonDesc["m_DiffuseTexturePath"].GetString();
		strcpy(m_ButtonDesc.m_DiffuseTexturePath, m_DiffuseTexturePath.c_str());

		string m_MaskPrototypeTag = ButtonDesc["m_MaskPrototypeTag"].GetString();
		lstrcpy(m_ButtonDesc.m_MaskPrototypeTag, convert.from_bytes(m_MaskPrototypeTag).c_str());

		string m_BrushPrototypeTag = ButtonDesc["m_BrushPrototypeTag"].GetString();
		lstrcpy(m_ButtonDesc.m_BrushPrototypeTag, convert.from_bytes(m_BrushPrototypeTag).c_str());

		string m_ButtonName = ButtonDesc["m_ButtonName"].GetString();
		lstrcpy(m_ButtonDesc.m_ButtonName, convert.from_bytes(m_ButtonName).c_str());

		const Value& parts = ButtonDesc["m_parts"].GetArray();
		for (SizeType i = 0; i < parts.Size(); ++i)
		{
			CGameObject* pPart = nullptr;

			CPartTexture::UI_DESC desc{};
			desc.pParent = m_pTransformCom;
			desc.m_fSizeX = parts[i]["m_fSizeX"].GetFloat();
			desc.m_fSizeY = parts[i]["m_fSizeY"].GetFloat();
			desc.m_fX = parts[i]["m_fX"].GetFloat();
			desc.m_fY = parts[i]["m_fY"].GetFloat();
			desc.m_TextureProtoTypeLevel = parts[i]["m_TextureProtoTypeLevel"].GetUint();

			string textureProtoTypeName = parts[i]["m_TextureProtoTypeName"].GetString();
			lstrcpy(desc.m_TextureProtoTypeName, convert.from_bytes(textureProtoTypeName).c_str());

			pPart = pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_PartTexture"), &desc);
			if (nullptr == pPart)
				return false;

			m_TextureParts.push_back(dynamic_cast<CPartTexture*>(pPart));
		}


	}

	return true;
}

HRESULT CButton::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, m_ButtonDesc.m_ModelPrototypLevel, TEXT("Prototype_Component_Model_Button_Base"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	if (strlen(m_ButtonDesc.m_DiffuseTexturePath) > 0)
		m_pModelCom->Set_Texture_In_Material(0, 1, m_ButtonDesc.m_DiffuseTexturePath);

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, m_ButtonDesc.m_ShaderLevelIndex, TEXT("Prototype_Component_Shader_VtxtexButton"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
	//	(CComponent**)&m_pTextureCom, nullptr)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CButton::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, m_ButtonDesc.m_ModelPrototypLevel, TEXT("Prototype_Component_Model_Button_Base"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	if (strlen(m_ButtonDesc.m_DiffuseTexturePath) > 0)
		m_pModelCom->Set_Texture_In_Material(0, 1, m_ButtonDesc.m_DiffuseTexturePath);

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, m_ButtonDesc.m_ShaderLevelIndex, TEXT("Prototype_Component_Shader_VtxtexButton"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton::SetUp_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;	

	Safe_Release(pGameInstance);

	return S_OK;
}

void CButton::Button_Motion(_double TimeDelta)
{

	switch (m_eCurState)
	{
	case CButton::BUTTON_IDLE:
		m_pModelCom->Play_Animation(TimeDelta);
		//m_pTransformCom->Set_Scaled({ m_ButtonDesc.m_fSizeX,  m_ButtonDesc.m_fSizeY, 1.f });
		break;
	case CButton::BUTTON_PRESS:
		m_pTransformCom->Set_Scaled({ m_ButtonDesc.m_fSizeX * m_TransformMatrix.m[0][0],  m_ButtonDesc.m_fSizeY * m_TransformMatrix.m[1][1], 1.f });
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_eCurState = BUTTON_SELECT;
		}
		break;
	case CButton::BUTTON_RELEASE:
		m_pTransformCom->Set_Scaled({ m_ButtonDesc.m_fSizeX * m_TransformMatrix.m[0][0],  m_ButtonDesc.m_fSizeY * m_TransformMatrix.m[1][1], 1.f });
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_eCurState = BUTTON_IDLE;
		}
		break;
	case CButton::BUTTON_SELECT:
		m_pTransformCom->Set_Scaled({ m_ButtonDesc.m_fSizeX * m_TransformMatrix.m[0][0],  m_ButtonDesc.m_fSizeY * m_TransformMatrix.m[1][1], 1.f });
		m_pModelCom->Play_Animation(TimeDelta, false);

		break;
	}
}

void CButton::Picking_Button()
{
	if (MOUSE_TAB(MOUSE::LBTN) && m_eCurState == BUTTON_IDLE)
	{
		RECT uiRect{ LONG(m_ButtonDesc.m_fX - m_ButtonDesc.m_fSizeX * 0.5f), LONG(m_ButtonDesc.m_fY - m_ButtonDesc.m_fSizeY * 0.5f)
		, LONG(m_ButtonDesc.m_fX + m_ButtonDesc.m_fSizeX * 0.5f),  LONG(m_ButtonDesc.m_fY + m_ButtonDesc.m_fSizeY * 0.5f) };

		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		RECT mouseRect{ pt.x - m_mouseInterSize, pt.y - m_mouseInterSize, pt.x + m_mouseInterSize, pt.y + m_mouseInterSize };

		RECT result{};
		if (IntersectRect(&result, &uiRect, &mouseRect))
		{
			m_eCurState = BUTTON_PRESS;
		}
	}

	else if (MOUSE_AWAY(MOUSE::LBTN))
	{
		m_eCurState = BUTTON_RELEASE;
	}
}

_uint CButton::Change_State()
{
	_uint result = 0;
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CButton::BUTTON_IDLE:
			m_pModelCom->Set_Animation(BUTTON_IDLE);
			result = On_Idle();
			break;
		case CButton::BUTTON_PRESS:
			m_pModelCom->Set_Animation(BUTTON_PRESS);
			result = On_Press();
			break;
		case CButton::BUTTON_RELEASE:
			m_pModelCom->Set_Animation(BUTTON_RELEASE);
			result = On_Release();
			break;
		case CButton::BUTTON_SELECT:
			m_selectTransformMatrix = m_TransformMatrix;
			//m_pModelCom->Set_Animation(BUTTON_SELECT);
			result = On_Select();
			break;
		}
		m_ePreState = m_eCurState;

	}

	return result;
}

HRESULT CButton::Common_Initialize()
{
	m_eRenderId = RENDER_UI;

	m_pTransformCom->Set_Scaled({ m_ButtonDesc.m_fSizeX, m_ButtonDesc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_ButtonDesc.m_fX - g_iWinSizeX * 0.5f, -m_ButtonDesc.m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}


void CButton::Free()
{
	__super::Free();

	for (auto& part : m_TextureParts)
	{
		Safe_Release(part);
	}

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}

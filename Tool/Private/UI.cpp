#include "stdafx.h"
#include "UI.h"

#include "GameInstance.h"

#include "PartTexture.h"
#include "PartText.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg != nullptr)
	{
		memcpy(&m_UIDesc, pArg, (sizeof m_UIDesc) + 2);
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX, m_UIDesc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_UIDesc.m_fX - g_iWinSizeX * 0.5f, -m_UIDesc.m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

HRESULT CUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX, m_UIDesc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_UIDesc.m_fX - g_iWinSizeX * 0.5f, -m_UIDesc.m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

_uint CUI::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CUI::LateTick(_double TimeDelta)
{

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CUI::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

_bool CUI::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Value UIDesc(kObjectType);
	{
		UIDesc.AddMember("m_fX", m_UIDesc.m_fX, allocator);
		UIDesc.AddMember("m_fY", m_UIDesc.m_fY, allocator);

		UIDesc.AddMember("m_fSizeX", m_UIDesc.m_fSizeX, allocator);
		UIDesc.AddMember("m_fSizeY", m_UIDesc.m_fSizeY, allocator);
		UIDesc.AddMember("m_TextureProtoTypeLevel", m_UIDesc.m_TextureProtoTypeLevel, allocator);
		UIDesc.AddMember("m_UIType", m_UIDesc.m_UIType, allocator);

		Value m_vColor(kObjectType);
		{
			m_vColor.AddMember("x", m_UIDesc.m_vColor.x, allocator);
			m_vColor.AddMember("y", m_UIDesc.m_vColor.y, allocator);
			m_vColor.AddMember("z", m_UIDesc.m_vColor.z, allocator);
			m_vColor.AddMember("w", m_UIDesc.m_vColor.w, allocator);
		}
		UIDesc.AddMember("m_vColor", m_vColor, allocator);

		Value m_TextureProtoTypeName;
		string TextureProtoTypeName = convert.to_bytes(m_UIDesc.m_TextureProtoTypeName);
		m_TextureProtoTypeName.SetString(TextureProtoTypeName.c_str(), (SizeType)TextureProtoTypeName.size(), allocator);
		UIDesc.AddMember("m_TextureProtoTypeName", m_TextureProtoTypeName, allocator);

	}
	doc.AddMember("UIDesc", UIDesc, allocator);

	return true;
}

_bool CUI::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	Value& UIDesc = doc["UIDesc"];

	m_UIDesc.m_fX = UIDesc["m_fX"].GetFloat();
	m_UIDesc.m_fY = UIDesc["m_fY"].GetFloat();
	m_UIDesc.m_fSizeX = UIDesc["m_fSizeX"].GetFloat();
	m_UIDesc.m_fSizeY = UIDesc["m_fSizeY"].GetFloat();

	m_UIDesc.m_TextureProtoTypeLevel = UIDesc["m_TextureProtoTypeLevel"].GetUint();
	m_UIDesc.m_UIType = UIDesc["m_UIType"].GetUint();

	const Value& m_vColor = UIDesc["m_vColor"];
	m_UIDesc.m_vColor = _float4(m_vColor["x"].GetFloat(), m_vColor["y"].GetFloat(), m_vColor["z"].GetFloat(), m_vColor["w"].GetFloat());

	string m_TextureProtoTypeName = UIDesc["m_TextureProtoTypeName"].GetString();

	lstrcpy(m_UIDesc.m_TextureProtoTypeName, convert.from_bytes(m_TextureProtoTypeName).c_str());

	const Value& TextureParts = UIDesc["m_TextureParts"].GetArray();
	for (SizeType i = 0; i < TextureParts.Size(); ++i)
	{
		CGameObject* pPart = nullptr;

		CPartTexture::UI_DESC desc{};
		desc.pParent = m_pTransformCom;
		desc.m_fSizeX = TextureParts[i]["m_fSizeX"].GetFloat();
		desc.m_fSizeY = TextureParts[i]["m_fSizeY"].GetFloat();
		desc.m_fX = TextureParts[i]["m_fX"].GetFloat();
		desc.m_fY = TextureParts[i]["m_fY"].GetFloat();
		desc.m_TextureProtoTypeLevel = TextureParts[i]["m_TextureProtoTypeLevel"].GetUint();

		string textureProtoTypeName = TextureParts[i]["m_TextureProtoTypeName"].GetString();
		lstrcpy(desc.m_TextureProtoTypeName, convert.from_bytes(textureProtoTypeName).c_str());

		pPart = pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_PartTexture"), &desc);
		if (nullptr == pPart)
			return false;

		m_TextureParts.push_back(dynamic_cast<CPartTexture*>(pPart));
	}

	const Value& TextParts = UIDesc["m_TextParts"].GetArray();
	for (SizeType i = 0; i < TextParts.Size(); ++i)
	{
		CGameObject* pPart = nullptr;
		CPartText::TEXT_DESC desc{};

		desc.pParent = m_pTransformCom;
		desc.m_fX = TextParts[i]["m_fX"].GetFloat();
		desc.m_fY = TextParts[i]["m_fY"].GetFloat();

		const Value& m_vPartColor = TextParts[i]["m_vColor"];
		desc.m_vColor = _float4(m_vPartColor["x"].GetFloat(), m_vPartColor["y"].GetFloat(), m_vPartColor["z"].GetFloat(), m_vPartColor["w"].GetFloat());

		desc.m_Rotation = TextParts[i]["m_Rotation"].GetFloat();

		const Value& m_vRotationOrigin = TextParts[i]["m_vRotationOrigin"];
		desc.m_vRotationOrigin = _float2(m_vRotationOrigin["x"].GetFloat(), m_vRotationOrigin["y"].GetFloat());

		const Value& m_vScale = TextParts[i]["m_vScale"];
		desc.m_vScale = _float2(m_vScale["x"].GetFloat(), m_vScale["y"].GetFloat());

		string fontTag = TextParts[i]["m_FontTag"].GetString();
		desc.m_FontTag = convert.from_bytes(fontTag);

		string m_Text = TextParts[i]["m_Text"].GetString();
		desc.m_Text = convert.from_bytes(m_Text);

		pPart = pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_PartText"), &desc);
		if (nullptr == pPart)
			return false;

		m_TextParts.push_back(dynamic_cast<CPartText*>(pPart));
	}

	return true;
}

HRESULT CUI::Add_Components()
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

	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_Rect::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		(CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	if (m_UIDesc.m_UIType == UI_TYPE_TEXTURE)
	{
		/* For.Com_Shader */
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
	}
	else if (m_UIDesc.m_UIType == UI_TYPE_COLOR_TEXTURE)
	{
		/* For.Com_Shader */
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, m_UIDesc.m_TextureProtoTypeLevel, m_UIDesc.m_TextureProtoTypeName,
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	else if (m_UIDesc.m_UIType == UI_TYPE_COLOR_TEXTURE)
	{
		m_pShaderCom->Set_RawValue("g_vColor", &m_UIDesc.m_vColor, sizeof(_float4));
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CUI::Add_Components_By_File()
{
	return E_NOTIMPL;
}

CUI* CUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI* pInstance = new CUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CUI* pInstance = new CUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CUI* pInstance = new CUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}

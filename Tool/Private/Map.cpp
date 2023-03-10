#include "stdafx.h"
#include "Map.h"

#include "GameInstance.h"

#include <codecvt>


CMap::CMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CMap::CMap(const CMap& rhs)
	: CGameObject(rhs)
{

}

HRESULT CMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

HRESULT CMap::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
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

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CMap::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CMap::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CMap::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

_bool CMap::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (m_pTransformCom)
	{
		Value MapDesc(kObjectType);
		{
			Value vPos(kObjectType);
			{
				_float4 pos = {};
				XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				vPos.AddMember("x", pos.x, allocator);
				vPos.AddMember("y", pos.y, allocator);
				vPos.AddMember("z", pos.z, allocator);
				vPos.AddMember("w", pos.w, allocator);
			}

			MapDesc.AddMember("vPos", vPos, allocator);


			Value ModelPrototypeTag;
			string tag = convert.to_bytes(m_MapDesc.ModelPrototypeTag.c_str());
			ModelPrototypeTag.SetString(tag.c_str(), tag.size(), allocator);
			MapDesc.AddMember("ModelPrototypeTag", ModelPrototypeTag, allocator);
			

			Value ShaderPrototypeTag;
			tag = convert.to_bytes(m_MapDesc.ShaderPrototypeTag.c_str());
			ShaderPrototypeTag.SetString(tag.c_str(), tag.size(), allocator);
			MapDesc.AddMember("ShaderPrototypeTag", ShaderPrototypeTag, allocator);

		}
		doc.AddMember("MapDesc", MapDesc, allocator);
	}

	return true;
}

_bool CMap::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& CameraDesc = doc["MapDesc"];

		const Value& vPos = CameraDesc["vPos"];
		m_pTransformCom->Set_Pos(vPos["x"].GetFloat(), vPos["y"].GetFloat(), vPos["z"].GetFloat());

		string ModelPrototypeTag = CameraDesc["ModelPrototypeTag"].GetString();
		m_MapDesc.ModelPrototypeTag = convert.from_bytes(ModelPrototypeTag);

		string ShaderPrototypeTag = CameraDesc["ShaderPrototypeTag"].GetString();
		m_MapDesc.ShaderPrototypeTag = convert.from_bytes(ShaderPrototypeTag);

	}

	return true;
}

HRESULT CMap::Add_Components()
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
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_BASECAMP, m_MapDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, m_MapDesc.ShaderPrototypeTag.c_str(),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMap::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_BASECAMP, m_MapDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, m_MapDesc.ShaderPrototypeTag.c_str(),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMap::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition",
		&pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	const LIGHTDESC* pLightDesc = pGameInstance->Get_Light(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir",
		&pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse",
		&pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient",
		&pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular",
		&pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CMap* CMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMap* pInstance = new CMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMap::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CMap* pInstance = new CMap(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMap::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CMap* pInstance = new CMap(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMap::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}

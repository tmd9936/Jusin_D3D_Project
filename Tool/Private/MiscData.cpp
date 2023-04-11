#include "stdafx.h"
#include "MiscData.h"

#include "GameInstance.h"

CMiscData::CMiscData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CMiscData::CMiscData(const CMiscData& rhs)
	: CGameObject(rhs)
	, m_MiscDesc(rhs.m_MiscDesc)
{

}

HRESULT CMiscData::Initialize_Prototype(const char* filePath)
{
	if (false == Load_By_JsonFile(filePath))
		return E_FAIL;

	m_filePath = filePath;

	return S_OK;
}

HRESULT CMiscData::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

HRESULT CMiscData::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
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

	return S_OK;
}

_uint CMiscData::Tick(_double TimeDelta)
{

	return _uint();
}

_uint CMiscData::LateTick(_double TimeDelta)
{
	return _uint();
}


_bool CMiscData::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	/*if (m_pTransformCom)
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

			MapDesc.AddMember("isAnim", m_MapDesc.isAnim, allocator);
			MapDesc.AddMember("Shader_Level_Index", m_MapDesc.Shader_Level_Index, allocator);
			MapDesc.AddMember("Model_Level_Index", m_MapDesc.Model_Level_Index, allocator);

		}
		doc.AddMember("MapDesc", MapDesc, allocator);
	}*/

	return true;
}

_bool CMiscData::Load_By_JsonFile_Impl(Document& doc)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	const Value& MiscDesc = doc["MiscDesc"];

	// ==== hpBarMiscDesc ====
	const Value& hpBarMiscDesc = MiscDesc["hpBarMiscDesc"].GetObj();
	m_MiscDesc.hpBarMiscDesc.SizeX = hpBarMiscDesc["SizeX"].GetFloat();
	m_MiscDesc.hpBarMiscDesc.SizeY = hpBarMiscDesc["SizeY"].GetFloat();

	m_MiscDesc.hpBarMiscDesc.PositionX = hpBarMiscDesc["PositionX"].GetFloat();
	m_MiscDesc.hpBarMiscDesc.PositionY = hpBarMiscDesc["PositionY"].GetFloat();
	m_MiscDesc.hpBarMiscDesc.PositionZ = hpBarMiscDesc["PositionZ"].GetFloat();

	const Value& playerColor = hpBarMiscDesc["playerColor"].GetObj();
	m_MiscDesc.hpBarMiscDesc.playerColor = {
		playerColor["x"].GetFloat(),
		playerColor["y"].GetFloat(),
		playerColor["z"].GetFloat(),
		playerColor["w"].GetFloat()
	};

	const Value& enemyColor = hpBarMiscDesc["enemyColor"].GetObj();
	m_MiscDesc.hpBarMiscDesc.enemyColor = {
		enemyColor["x"].GetFloat(),
		enemyColor["y"].GetFloat(),
		enemyColor["z"].GetFloat(),
		enemyColor["w"].GetFloat()
	};
	// ==== hpBarMiscDesc ====

	// ==== damageTextMiscDesc ====
	const Value& damageTextMiscDesc = MiscDesc["damageTextMiscDesc"].GetObj();
	m_MiscDesc.damageTextMiscDesc.SizeX = damageTextMiscDesc["SizeX"].GetFloat();
	m_MiscDesc.damageTextMiscDesc.SizeY = damageTextMiscDesc["SizeY"].GetFloat();

	m_MiscDesc.damageTextMiscDesc.PositionX = damageTextMiscDesc["PositionX"].GetFloat();
	m_MiscDesc.damageTextMiscDesc.PositionY = damageTextMiscDesc["PositionY"].GetFloat();
	m_MiscDesc.damageTextMiscDesc.PositionZ = damageTextMiscDesc["PositionZ"].GetFloat();
	// ==== damageTextMiscDesc ====

	return true;
}

HRESULT CMiscData::Add_Components()
{
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();

	///* For.Com_Shader */
	//if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, m_MapDesc.ShaderPrototypeTag.c_str(),
	//	(CComponent**)&m_pShaderCom, nullptr)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CMiscData::Add_Components_By_File()
{
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();

	///* For.Com_Renderer */
	//if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
	//	(CComponent**)&m_pRendererCom, nullptr)))
	//	return E_FAIL;

	return S_OK;
}

CMiscData* CMiscData::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* filePath)
{
	CMiscData* pInstance = new CMiscData(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(filePath)))
	{
		MSG_BOX("Failed to Created CMiscData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMiscData::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CMiscData* pInstance = new CMiscData(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CMiscData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMiscData::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CMiscData* pInstance = new CMiscData(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CMiscData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiscData::Free()
{
	__super::Free();
}

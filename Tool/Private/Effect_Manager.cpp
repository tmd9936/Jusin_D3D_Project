#include "stdafx.h"
#include "Effect_Manager.h"

#include "GameInstance.h"

#include "Model.h"

CEffect_Manager::CEffect_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CEffect_Manager::CEffect_Manager(const CEffect_Manager& rhs)
	: CGameObject(rhs)
	, m_Effect_Descs(rhs.m_Effect_Descs)
{

}

HRESULT CEffect_Manager::Initialize_Prototype(const char* filePath)
{
	if (false == Load_By_JsonFile(filePath))
		return E_FAIL;

	m_filePath = filePath;

	return S_OK;
}

HRESULT CEffect_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
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

_uint CEffect_Manager::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CEffect_Manager::LateTick(_double TimeDelta)
{
	return _uint();
}

CEffect* CEffect_Manager::Create_Effect(_uint effectType, const _tchar* pLayerTag, _uint iLevelIndex, _bool hasCollider,  _float3 vPos)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (effectType >= m_Effect_Descs.size() || nullptr == pLayerTag || iLevelIndex >= LEVEL_END)
		return nullptr;

	CEffect::EFFECT_DESC effect_Desc = m_Effect_Descs[effectType];

	effect_Desc.m_bCollision = hasCollider;

	wstring	FilePath = m_EffectFilePath + effect_Desc.m_effectPath + L".fbx";
	effect_Desc.m_ProtoTypeTag = L"Prototype_Component_Model_" + effect_Desc.m_effectPath;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (false == pGameInstance->Check_Prototype(effect_Desc.m_ProtoTypeTag))
	{
		_matrix	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		string effectPath = convert.to_bytes(FilePath);
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, effect_Desc.m_ProtoTypeTag.c_str(),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, effectPath.c_str(), PivotMatrix))))
			return	nullptr;

	}

	//CGameObject* pEffect = nullptr;

	//pEffect = pGameInstance->Clone_GameObject(L"Layer_Effect", iLevelIndex, TEXT("Prototype_GameObject_Effect"), &effect_Desc);

	//if (nullptr == pEffect)
	//	return nullptr;

	CEffect* pEffect = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Effect"), iLevelIndex, pLayerTag, (CGameObject**)&pEffect, nullptr, &effect_Desc)))
		return nullptr;

	pEffect->Init_LoopCount(0);

	if (0 == effect_Desc.m_effectPath.compare(L"E_EF_Charge"))
	{
		pEffect->Set_ParentRotateApply(false);
		pEffect->Set_AnimaitonStartTime(19.7);
		pEffect->Init_LoopCount(3);
		pEffect->Set_Animation_Speed(2.2);
	}
	else if (0 == effect_Desc.m_effectPath.compare(L"E_BGB_Denki"))
	{
		pEffect->Set_ParentRotateApply(false);
		pEffect->Init_LoopCount(7);
		pEffect->Set_Homming(true, false, CEffect::HOMMING_OUT);
	}

	else if (0 == effect_Desc.m_effectPath.compare(L"E_SP_Kaminari_Start"))
	{
		//pEffect->Set_ParentRotateApply(false);
		pEffect->Init_LoopCount(1);
		//pEffect->Set_Homming(true, false, CEffect::HOMMING_OUT);
	}

	Safe_Release(pGameInstance);

	return pEffect;
}

void CEffect_Manager::Get_Effect_Desces(vector<CEffect::EFFECT_DESC>& Effect_Descs)
{
	Effect_Descs = m_Effect_Descs;
}


HRESULT CEffect_Manager::Reload_Datas()
{
	m_Effect_Descs.clear();
	if (false == Load_By_JsonFile(m_filePath.c_str()))
		return E_FAIL;

	return S_OK;
}

_bool CEffect_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{

	return true;
}

_bool CEffect_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& m_datas = doc["m_datas"];
	assert(m_datas.IsArray());

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (SizeType i = 0; i < m_datas.Size(); ++i)
	{
		CEffect::EFFECT_DESC m_desc{};

		m_desc.m_effectType = m_datas[i]["m_effectType"].GetUint();
		m_desc.m_actionType = m_datas[i]["m_actionType"].GetUint();
		m_desc.m_effectPath = convert.from_bytes(m_datas[i]["m_effectPath"].GetString());
		m_desc.m_exPath1 = convert.from_bytes(m_datas[i]["m_exPath1"].GetString());
		m_desc.m_exPath2 = convert.from_bytes(m_datas[i]["m_exPath2"].GetString());
		m_desc.m_soundEventID = m_datas[i]["m_soundEventID"].GetUint();
		//m_desc.m_soundEventTag = convert.from_bytes(m_datas[i]["m_soundEventTag"].GetString());
		m_desc.m_underFlag = m_datas[i]["m_underFlag"].GetUint();

		m_Effect_Descs.push_back(m_desc);
	}

	return true;
}


HRESULT CEffect_Manager::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	return S_OK;
}

HRESULT CEffect_Manager::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();


	return S_OK;
}

HRESULT CEffect_Manager::SetUp_ShaderResources()
{

	return S_OK;
}

CEffect_Manager* CEffect_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* filePath)
{
	CEffect_Manager* pInstance = new CEffect_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(filePath)))
	{
		MSG_BOX("Failed to Created CEffect_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CEffect_Manager* pInstance = new CEffect_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CEffect_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CEffect_Manager* pInstance = new CEffect_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CEffect_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Manager::Free()
{
	__super::Free();

	m_Effect_Descs.clear();
}
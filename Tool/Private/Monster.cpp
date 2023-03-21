#include "stdafx.h"
#include "Monster.h"

#include "GameInstance.h"

#include "BuffState.h"
#include "Weapon.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_MotionState()))
		return E_FAIL;

	if (FAILED(Add_BuffState()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	m_pTransformCom->Set_Pos(rand() % 10 + 12.f, 0.f, rand() % 10 + 19.f);

	Add_TransitionRandomState();

	m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1);

	return S_OK;
}

HRESULT CMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
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

	m_pTransformCom->Set_TransforDesc({ m_PokemonDesc.moveSpeed, XMConvertToRadians(45.0f) });

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	if (FAILED(Add_MotionState()))
		return E_FAIL;

	if (FAILED(Add_BuffState()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	Add_TransitionRandomState();

	m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1);

	return S_OK;
}

_uint CMonster::Tick(_double TimeDelta)
{
	m_pMonFSM->Update_Component((_float)TimeDelta, m_pModelCom);

	m_pAABB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	m_pOBB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	m_pSphere->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	
	for (auto& pGameObject : m_Parts)
		pGameObject->Tick(TimeDelta);

	return State_Tick(TimeDelta);
}

_uint CMonster::LateTick(_double TimeDelta)
{
	for (auto& pGameObject : m_Parts)
		pGameObject->LateTick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CMonster::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pAABB->Render();
	m_pOBB->Render();
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CMonster::Add_BuffState()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pGameObject = nullptr;

	CBone* pBone = m_pModelCom->Get_BonePtr("effect00_end");
	if (nullptr == pBone)
		return E_FAIL;

	CBuffState::BUFFSTATE_DESC		BuffStateDesc;
	ZeroMemory(&BuffStateDesc, sizeof BuffStateDesc);

	BuffStateDesc.pBonePtr = pBone;
	Safe_AddRef(pBone);

	BuffStateDesc.pParent = m_pTransformCom;
	Safe_AddRef(m_pTransformCom);

	XMStoreFloat4x4(&BuffStateDesc.PivotMatrix, m_pModelCom->Get_PivotMatrix());

	lstrcpy(BuffStateDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Pokemon_State_doku");

	BuffStateDesc.m_TextureLevelIndex = LEVEL_BASECAMP;

	pGameObject = pGameInstance->Clone_GameObject(L"Layer_BuffState", BuffStateDesc.m_TextureLevelIndex, TEXT("Prototype_GameObject_BuffState"), &BuffStateDesc);
	if (nullptr == pGameObject)
		return E_FAIL;

	m_Parts.push_back(pGameObject);

	Safe_Release(pGameInstance);

	return S_OK;

	//CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);

	//CGameObject* pGameObject = nullptr;

	//CBone* pBone = m_pModelCom->Get_BonePtr("effect00_end");
	//if (nullptr == pBone)
	//	return E_FAIL;

	//CWeapon::WEAPONDESC		WeaponDesc;
	//ZeroMemory(&WeaponDesc, sizeof WeaponDesc);

	//WeaponDesc.pBonePtr = pBone;
	//Safe_AddRef(pBone);

	//WeaponDesc.pParent = m_pTransformCom;
	//Safe_AddRef(m_pTransformCom);

	//XMStoreFloat4x4(&WeaponDesc.PivotMatrix, m_pModelCom->Get_PivotMatrix());

	//pGameObject = pGameInstance->Clone_GameObject(L"Layer_BuffState", LEVEL_BASECAMP, TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//m_Parts.push_back(pGameObject);

	//Safe_Release(pGameInstance);

	//return S_OK;

}

HRESULT CMonster::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_PickingCube */
	if (FAILED(pGameInstance->Add_Component(CPickingCube::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		(CComponent**)&m_pPickingCube, nullptr)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	//if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, m_PokemonDesc.ModelPrototypeLevelIndex, m_PokemonDesc.ModelPrototypeTag.c_str(),
	//	(CComponent**)&m_pModelCom, nullptr)))
	//	return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_BASECAMP, L"Prototype_Component_Model_Pokemon_PM6",
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_MonFSM */
	if (FAILED(pGameInstance->Add_Component(CMonFSM::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_MonFSM"),
		(CComponent**)&m_pMonFSM, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB*/
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.2f, 2.0f, 1.2f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_OBB, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		(CComponent**)&m_pOBB, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere */
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_SPHERE, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		(CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_PickingCube */
	if (FAILED(pGameInstance->Add_Component(CPickingCube::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		(CComponent**)&m_pPickingCube, nullptr)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	m_PokemonDesc.ModelPrototypeTag = L"Prototype_Component_Model_Pokemon_PM";
	m_PokemonDesc.ModelPrototypeTag += to_wstring((int)m_PokemonDesc.m_monsterNo);
	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, m_PokemonDesc.ModelPrototypeLevelIndex, m_PokemonDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_MonFSM */
	if (FAILED(pGameInstance->Add_Component(CMonFSM::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_MonFSM"),
		(CComponent**)&m_pMonFSM, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId	, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB*/
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.2f, 2.0f, 1.2f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_OBB, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		(CComponent**)&m_pOBB, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere */
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_SPHERE, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		(CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderResources()
{
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
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

_bool CMonster::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (m_pTransformCom)
	{
		Value PokemonDesc(kObjectType);
		{
			Value ModelPrototypeTag;
			string tag = convert.to_bytes(m_PokemonDesc.ModelPrototypeTag.c_str());
			ModelPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			PokemonDesc.AddMember("ModelPrototypeTag", ModelPrototypeTag, allocator);

			PokemonDesc.AddMember("ModelPrototypeLevelIndex", m_PokemonDesc.ModelPrototypeLevelIndex, allocator);

			Value vPos(kObjectType);
			{
				_float4 pos = {};
				XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				vPos.AddMember("x", pos.x, allocator);
				vPos.AddMember("y", pos.y, allocator);
				vPos.AddMember("z", pos.z, allocator);
				vPos.AddMember("w", pos.w, allocator);
			}
			PokemonDesc.AddMember("vPos", vPos, allocator);

			PokemonDesc.AddMember("moveSpeed", m_PokemonDesc.moveSpeed, allocator);
			PokemonDesc.AddMember("rotateSpeed", m_PokemonDesc.rotateSpeed, allocator);

			PokemonDesc.AddMember("m_monsterNo", m_PokemonDesc.m_monsterNo, allocator);
			PokemonDesc.AddMember("m_attackBasis", m_PokemonDesc.m_attackBasis, allocator);
			PokemonDesc.AddMember("m_hpGrow", m_PokemonDesc.m_hpGrow, allocator);
			PokemonDesc.AddMember("m_attackGrow", m_PokemonDesc.m_attackGrow, allocator);
			PokemonDesc.AddMember("m_type1", m_PokemonDesc.m_type1, allocator);
			PokemonDesc.AddMember("m_type2", m_PokemonDesc.m_type2, allocator);
			PokemonDesc.AddMember("m_visitWeightDefault", m_PokemonDesc.m_visitWeightDefault, allocator);
			PokemonDesc.AddMember("m_visitWeight", m_PokemonDesc.m_visitWeight, allocator);
			PokemonDesc.AddMember("m_cookTableID", m_PokemonDesc.m_cookTableID, allocator);
			PokemonDesc.AddMember("m_color", m_PokemonDesc.m_color, allocator);
			PokemonDesc.AddMember("m_Rate", m_PokemonDesc.m_Rate, allocator);
			PokemonDesc.AddMember("m_isLayer", m_PokemonDesc.m_isLayer, allocator);
			PokemonDesc.AddMember("m_meleePercent", m_PokemonDesc.m_meleePercent, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightHp", m_PokemonDesc.m_slotTypeWeightHp, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightAttack", m_PokemonDesc.m_slotTypeWeightAttack, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightMulti", m_PokemonDesc.m_slotTypeWeightMulti, allocator);

			Value m_skillIDs(kArrayType);
			{
				for (size_t i = 0; i < m_PokemonDesc.m_skillIDs.size(); ++i)
				{
					m_skillIDs.PushBack(m_PokemonDesc.m_skillIDs[i], allocator);
				}
			}
			PokemonDesc.AddMember("m_skillIDs", m_skillIDs, allocator);

		}
		doc.AddMember("PokemonDesc", PokemonDesc, allocator);
	}

	return true;
}

_bool CMonster::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& PokemonDesc = doc["PokemonDesc"];

		string ModelPrototypeTag = PokemonDesc["ModelPrototypeTag"].GetString();
		m_PokemonDesc.ModelPrototypeTag = convert.from_bytes(ModelPrototypeTag);

		m_PokemonDesc.ModelPrototypeLevelIndex = PokemonDesc["ModelPrototypeLevelIndex"].GetUint();

		const Value& vPos = PokemonDesc["vPos"];
		m_pTransformCom->Set_Pos(vPos["x"].GetFloat(), vPos["y"].GetFloat(), vPos["z"].GetFloat());

		m_PokemonDesc.moveSpeed = PokemonDesc["moveSpeed"].GetFloat();
		m_PokemonDesc.rotateSpeed = PokemonDesc["rotateSpeed"].GetFloat();

		m_PokemonDesc.m_monsterNo = PokemonDesc["m_monsterNo"].GetUint();
		m_PokemonDesc.m_hpBasis = PokemonDesc["m_hpBasis"].GetUint();
		m_PokemonDesc.m_attackBasis = PokemonDesc["m_attackBasis"].GetUint();
		m_PokemonDesc.m_hpGrow = PokemonDesc["m_hpGrow"].GetUint();
		m_PokemonDesc.m_attackGrow = PokemonDesc["m_attackGrow"].GetUint();
		m_PokemonDesc.m_type1 = PokemonDesc["m_type1"].GetUint();
		m_PokemonDesc.m_type2 = PokemonDesc["m_type2"].GetUint();
		m_PokemonDesc.m_visitWeightDefault = PokemonDesc["m_visitWeightDefault"].GetUint();
		m_PokemonDesc.m_visitWeight = PokemonDesc["m_visitWeight"].GetUint();
		m_PokemonDesc.m_cookTableID = PokemonDesc["m_cookTableID"].GetUint();
		m_PokemonDesc.m_color = PokemonDesc["m_color"].GetUint();
		m_PokemonDesc.m_Rate = PokemonDesc["m_Rate"].GetUint();
		m_PokemonDesc.m_isLayer = PokemonDesc["m_isLayer"].GetUint();
		m_PokemonDesc.m_meleePercent = PokemonDesc["m_meleePercent"].GetUint();
		m_PokemonDesc.m_slotTypeWeightHp = PokemonDesc["m_slotTypeWeightHp"].GetUint();
		m_PokemonDesc.m_slotTypeWeightAttack = PokemonDesc["m_slotTypeWeightAttack"].GetUint();
		m_PokemonDesc.m_slotTypeWeightMulti = PokemonDesc["m_slotTypeWeightMulti"].GetUint();

		//const Value& skillIDs = PokemonDesc["m_skillIDs"];
		//for (SizeType i = 0; i < skillIDs.Size(); ++i)
		//{
		//	m_PokemonDesc.m_skillIDs.push_back(skillIDs[i].GetInt());
		//}
	}

	return true;
}

HRESULT CMonster::Add_MotionState()
{
	if (nullptr == m_pMonFSM)
		return E_FAIL;

	for (_uint i = 0; i < CMonFSM::END_MOTION; ++i)
	{
		m_pMonFSM->Add_MotionState(CMonFSM::MONSTER_STATE(i), i);
	}

	return S_OK;
}

//CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	CMonster* pInstance = new CMonster(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created CMonster");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CGameObject* CMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
//{
//	CMonster* pInstance = new CMonster(*this);
//
//	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
//	{
//		MSG_BOX("Failed to Cloned CMonster");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CGameObject* CMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
//{
//	CMonster* pInstance = new CMonster(*this);
//
//	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
//	{
//		MSG_BOX("Failed to Cloned CMonster");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

void CMonster::Free()
{
	__super::Free();


	for (auto& pGameObject : m_Parts)
		Safe_Release(pGameObject);

	m_Parts.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCube);
	Safe_Release(m_pMonFSM);
	Safe_Release(m_pAABB);
	Safe_Release(m_pOBB);
	Safe_Release(m_pSphere);

}

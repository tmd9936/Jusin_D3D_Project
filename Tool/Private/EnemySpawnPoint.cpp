#include "stdafx.h"
#include "EnemySpawnPoint.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

CEnemySpawnPoint::CEnemySpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CEnemySpawnPoint::CEnemySpawnPoint(const CEnemySpawnPoint& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEnemySpawnPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemySpawnPoint::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.m_position = (*(ENEMY_SPAWN_POINT_DESC*)(pArg)).m_position;
		m_Desc.m_type = (*(ENEMY_SPAWN_POINT_DESC*)(pArg)).m_type;
		m_Desc.m_spawnRadius = (*(ENEMY_SPAWN_POINT_DESC*)(pArg)).m_spawnRadius;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_Desc.m_position.x, m_Desc.m_position.y, m_Desc.m_position.z);

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

HRESULT CEnemySpawnPoint::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_Json()))
		return E_FAIL;

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_Desc.m_position.x, m_Desc.m_position.y, m_Desc.m_position.z);

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CEnemySpawnPoint::Tick(_double TimeDelta)
{
	m_pSphere->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	return _uint();
}

_uint CEnemySpawnPoint::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CEnemySpawnPoint::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

#ifdef _DEBUG
	m_pRendererCom->Add_DebugRenderGroup(m_pSphere);
#endif // _DEBUG

	return S_OK;
}

_bool CEnemySpawnPoint::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	Value EnemySpawnPointDesc(kObjectType);
	{
		Value m_position(kObjectType);
		{
			m_position.AddMember("x", m_Desc.m_position.x, allocator);
			m_position.AddMember("y", m_Desc.m_position.y, allocator);
			m_position.AddMember("z", m_Desc.m_position.z, allocator);
		}
		EnemySpawnPointDesc.AddMember("m_position", m_position, allocator);

		EnemySpawnPointDesc.AddMember("m_type", m_Desc.m_type, allocator);
		EnemySpawnPointDesc.AddMember("m_spawnRadius", m_Desc.m_spawnRadius, allocator);
	}
	doc.AddMember("EnemySpawnPointDesc", EnemySpawnPointDesc, allocator);

	return true;
}

_bool CEnemySpawnPoint::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	const Value& EnemySpawnPointDesc = doc["EnemySpawnPointDesc"];

	const Value& m_position = EnemySpawnPointDesc["m_position"];
	m_Desc.m_position = _float3(m_position["x"].GetFloat(), m_position["y"].GetFloat(), m_position["z"].GetFloat());

	m_Desc.m_type = EnemySpawnPointDesc["m_type"].GetUint();
	m_Desc.m_spawnRadius = EnemySpawnPointDesc["m_spawnRadius"].GetFloat();

	return true;
}


HRESULT CEnemySpawnPoint::Add_Components()
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

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		(CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEnemySpawnPoint::Add_Components_By_Json()
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

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(m_Desc.m_spawnRadius, m_Desc.m_spawnRadius, m_Desc.m_spawnRadius);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		(CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CEnemySpawnPoint::SetUp_ShaderResources()
{
	return S_OK;
}

HRESULT CEnemySpawnPoint::Insert_In_Stage_Manager()
{
	CGameObject* pObject = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");
	if (nullptr == pObject)
		return E_FAIL;

	CStage_Manager* pStageManager = dynamic_cast<CStage_Manager*>(pObject);
	if (nullptr == pStageManager)
		return E_FAIL;

	pStageManager->Add_EnemySpawnPoint(this);

	return S_OK;
}

CEnemySpawnPoint* CEnemySpawnPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnemySpawnPoint* pInstance = new CEnemySpawnPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CEnemySpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemySpawnPoint::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CEnemySpawnPoint* pInstance = new CEnemySpawnPoint(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CEnemySpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemySpawnPoint::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CEnemySpawnPoint* pInstance = new CEnemySpawnPoint(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CEnemySpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CEnemySpawnPoint::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCube);
	Safe_Release(m_pSphere);
}

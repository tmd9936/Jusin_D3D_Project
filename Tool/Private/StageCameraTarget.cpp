#include "stdafx.h"
#include "StageCameraTarget.h"

#include "GameInstance.h"

CStageCameraTarget::CStageCameraTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CStageCameraTarget::CStageCameraTarget(const CStageCameraTarget& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStageCameraTarget::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStageCameraTarget::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

_uint CStageCameraTarget::Tick(_double TimeDelta)
{
	switch (m_eCurrentState)
	{
	case STATE_FORMATION:
		Formation_State_Tick(TimeDelta);
		break;
	case STATE_STOP:
		break;
	}
	Change_State();
	return _uint();
}

_uint CStageCameraTarget::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CStageCameraTarget::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CStageCameraTarget::Change_State()
{
	if (m_eCurrentState != m_ePreState)
	{
		switch (m_eCurrentState)
		{
		case STATE_FORMATION:
			break;
		case STATE_STOP:
			break;
		}
	}
}

void CStageCameraTarget::Formation_State_Tick(const _double& TimeDelta)
{
	CGameObject* pPlyaer = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player");
	if (nullptr == pPlyaer)
		return;
	CTransform* pPlayerTransform = pPlyaer->Get_As<CTransform>();
	if (nullptr == pPlayerTransform)
		return;

	m_pTransformCom->Chase(pPlayerTransform->Get_State(CTransform::STATE_POSITION), (_float)TimeDelta);
}

HRESULT CStageCameraTarget::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 5.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageCameraTarget::SetUp_ShaderResources()
{

	return S_OK;
}

CStageCameraTarget* CStageCameraTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageCameraTarget* pInstance = new CStageCameraTarget(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageCameraTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageCameraTarget::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageCameraTarget* pInstance = new CStageCameraTarget(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageCameraTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageCameraTarget::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}

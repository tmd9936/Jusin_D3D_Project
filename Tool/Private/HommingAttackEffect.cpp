#include "HommingAttackEffect.h"

#include "Skill_Manager.h";

#include "GameInstance.h"

CHommingAttackEffect::CHommingAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackEffect(pDevice, pContext)
{
}

CHommingAttackEffect::CHommingAttackEffect(const CHommingAttackEffect& rhs)
	: CAttackEffect(rhs)
{
}

HRESULT CHommingAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHommingAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_HommingAttackEffectDesc.m_bArriveHomeDead = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_bArriveHomeDead;
		m_HommingAttackEffectDesc.m_bParentRotateApply = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_bParentRotateApply;
		m_HommingAttackEffectDesc.m_eHommingState = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_eHommingState;
	
		m_HommingAttackEffectDesc.m_SmallRotation = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_SmallRotation;
		m_HommingAttackEffectDesc.m_SmallRotationSpeed = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_SmallRotationSpeed;

		m_HommingAttackEffectDesc.m_BigRotation = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_BigRotation;
		m_HommingAttackEffectDesc.m_BigRotationRadius = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_BigRotationRadius;
		m_HommingAttackEffectDesc.m_BigRotationSpeed = (*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).m_BigRotationSpeed;

		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &(*(HOMMING_ATTACK_EFFECT_DESC*)(pArg)).attackEffectDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	return S_OK;
}

_uint CHommingAttackEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	Small_Rotation(TimeDelta);
	Homming(TimeDelta);
	Big_Rotation(TimeDelta);

	return _uint();
}

//_uint CHommingAttackEffect::LateTick(_double TimeDelta)
//{
//	__super::LateTick(TimeDelta);
//
//	return _uint();
//}


void CHommingAttackEffect::Small_Rotation(const _double& TimeDelta)
{
	if (m_HommingAttackEffectDesc.m_SmallRotation)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)(TimeDelta * m_HommingAttackEffectDesc.m_SmallRotationSpeed));
	}
}

void CHommingAttackEffect::Homming(const _double& TimeDelta)
{

	_vector vDir = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vParentPos = {};
	if (!m_EffectDesc.pParent)
	{
		//vParentPos = m_HommingAttackEffectDesc.pParent->Get_State(CTransform::STATE_POSITION);
	}
	else
	{
		vParentPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	switch (m_HommingAttackEffectDesc.m_eHommingState)
	{
	case HOMMING_OUT:
		vDir = XMVector3Normalize(vPos - vParentPos);
		if (m_pTransformCom->Get_Positin_Length() >= m_HommingAttackEffectDesc.m_BigRotationRadius)
		{
			m_HommingAttackEffectDesc.m_eHommingState = HOMMING_IN;
		}
		break;
	case HOMMING_IN:
		vDir = XMVector3Normalize(vParentPos - vPos);
		if (XMVectorGetX(XMVector3Length(vParentPos - vPos)) <= 0.3f)
		{
			m_HommingAttackEffectDesc.m_eHommingState = HOMMING_OUT;
		}
		break;
	}

	vPos += vDir * (_float)TimeDelta;
	m_pTransformCom->Set_Pos(vPos);
	
}

void CHommingAttackEffect::Big_Rotation(const _double& TimeDelta)
{
	if (m_HommingAttackEffectDesc.m_BigRotation)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };

		_matrix BigRot = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vUp, XMConvertToRadians(m_HommingAttackEffectDesc.m_BigRotationSpeed)));

		vPos = XMVector3TransformCoord(vPos, BigRot);

		m_pTransformCom->Set_Pos(vPos);
	}
}

CHommingAttackEffect* CHommingAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHommingAttackEffect* pInstance = new CHommingAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CHommingAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHommingAttackEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CHommingAttackEffect* pInstance = new CHommingAttackEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CHommingAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHommingAttackEffect::Free()
{
	__super::Free();

	Safe_Release(m_pAttackCom);
	Safe_Release(m_pColliderCom);

}

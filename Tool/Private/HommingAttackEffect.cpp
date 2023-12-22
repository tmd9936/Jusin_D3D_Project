#include "HommingAttackEffect.h"

#include "Skill_Manager.h"

#include "GameInstance.h"

CHommingAttackEffect::CHommingAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackEffect(pDevice, pContext)
{
}

CHommingAttackEffect::CHommingAttackEffect(const CHommingAttackEffect& rhs)
	: CAttackEffect(rhs)
	, m_HommingAttackEffectDesc(rhs.m_HommingAttackEffectDesc)
{
}

HRESULT CHommingAttackEffect::Initialize_Prototype(Homming_Attack_Effect_Desc& desc)
{
	m_HommingAttackEffectDesc = desc;

	m_AttackEffectDesc = m_HommingAttackEffectDesc.attackEffectDesc;

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHommingAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	m_EffectDesc.m_bParentRotateApply = m_HommingAttackEffectDesc.attackEffectDesc.effectDesc.m_bParentRotateApply;
	m_EffectDesc.m_CurrentLoopCount = m_HommingAttackEffectDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount;

	m_EffectDesc.m_AnimationSpeed = m_HommingAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationSpeed;
	m_EffectDesc.m_AnimationStartAcc = m_HommingAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc;

	return S_OK;
}

_uint CHommingAttackEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	Loop_Count_Check(TimeDelta);

	Attack_Time_Check(TimeDelta);

	Small_Rotation(TimeDelta);
	Homming(TimeDelta);
	Big_Rotation(TimeDelta);

	return _uint();
}

_uint CHommingAttackEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	return _uint();
}


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

CHommingAttackEffect* CHommingAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Homming_Attack_Effect_Desc& desc)
{
	CHommingAttackEffect* pInstance = new CHommingAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
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

}

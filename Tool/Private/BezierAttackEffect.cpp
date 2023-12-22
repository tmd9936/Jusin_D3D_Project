#include "BezierAttackEffect.h"

#include "Skill_Manager.h"

#include "GameInstance.h"

CBezierAttackEffect::CBezierAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackEffect(pDevice, pContext)
{
}

CBezierAttackEffect::CBezierAttackEffect(const CBezierAttackEffect& rhs)
	: CAttackEffect(rhs)
	, m_BezierAttackEffectDesc(rhs.m_BezierAttackEffectDesc)
{
}

HRESULT CBezierAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBezierAttackEffect::Initialize_Prototype(BEZIER_ATTACK_EFFECT_DESC& desc)
{
	m_BezierAttackEffectDesc = desc;

	m_AttackEffectDesc = m_BezierAttackEffectDesc.attackEffectDesc;

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBezierAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	m_EffectDesc.m_bParentRotateApply = m_BezierAttackEffectDesc.attackEffectDesc.effectDesc.m_bParentRotateApply;
	m_EffectDesc.m_CurrentLoopCount = m_BezierAttackEffectDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount;

	m_EffectDesc.m_AnimationSpeed = m_BezierAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationSpeed;
	m_EffectDesc.m_AnimationStartAcc = m_BezierAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc;

	return S_OK;
}

_uint CBezierAttackEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), (_float)TimeDelta);

	Bezier(TimeDelta);

	Loop_Count_Check(TimeDelta);

	Attack_Time_Check(TimeDelta);

	return _uint();
}

_uint CBezierAttackEffect::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

void CBezierAttackEffect::Bezier(const _double& TimeDelta)
{
	if (m_BezierTime < 1.f)
	{
		_float x = pow((1.f - m_BezierTime), 2.f) * m_BezierAttackEffectDesc.m_vPoint1.x + 
			2.f * (1.f - m_BezierTime) * m_BezierTime * m_BezierAttackEffectDesc.m_vPoint2.x + 
			pow(m_BezierTime, 2.f) * m_BezierAttackEffectDesc.m_vPoint3.x;

		_float y = pow((1.f - m_BezierTime), 2.f) * m_BezierAttackEffectDesc.m_vPoint1.y +
			2.f * (1.f - m_BezierTime) * m_BezierTime * m_BezierAttackEffectDesc.m_vPoint2.y +
			pow(m_BezierTime, 2.f) * m_BezierAttackEffectDesc.m_vPoint3.y;

		_float z = pow((1.f - m_BezierTime), 2.f) * m_BezierAttackEffectDesc.m_vPoint1.z +
			2.f * (1.f - m_BezierTime) * m_BezierTime * m_BezierAttackEffectDesc.m_vPoint2.z +
			pow(m_BezierTime, 2.f) * m_BezierAttackEffectDesc.m_vPoint3.z;

		m_pTransformCom->Set_Pos(x, y, z);

		m_BezierTime += (_float)TimeDelta * m_BezierSpeed;
	}
	else 
	{
		Create_Collision_Effect(m_pTransformCom);
		Set_Dead();
	}
}


CBezierAttackEffect* CBezierAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBezierAttackEffect* pInstance = new CBezierAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBezierAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBezierAttackEffect* CBezierAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BEZIER_ATTACK_EFFECT_DESC& desc)
{
	CBezierAttackEffect* pInstance = new CBezierAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX("Failed to Created CBezierAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBezierAttackEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CBezierAttackEffect* pInstance = new CBezierAttackEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CBezierAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBezierAttackEffect::Free()
{
	__super::Free();

}

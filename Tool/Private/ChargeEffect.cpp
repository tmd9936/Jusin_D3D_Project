#include "ChargeEffect.h"

#include "Skill_Manager.h"
#include "Effect_Manager.h"

#include "GameInstance.h"

CChargeEffect::CChargeEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillEffect(pDevice, pContext)
{
}

CChargeEffect::CChargeEffect(const CChargeEffect& rhs)
	: CSkillEffect(rhs)
{
}

HRESULT CChargeEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChargeEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_ChargeEffectDesc.m_NextEffectPrototypeTag = (*(Charge_Effect_Desc*)(pArg)).m_NextEffectPrototypeTag;
		m_ChargeEffectDesc.m_NextEffectPower = (*(Charge_Effect_Desc*)(pArg)).m_NextEffectPower;
		m_ChargeEffectDesc.m_ChargeTime = (*(Charge_Effect_Desc*)(pArg)).m_ChargeTime;

		m_ChargeEffectDesc.m_NextEffectNum = (*(Charge_Effect_Desc*)(pArg)).m_NextEffectNum;
		m_ChargeEffectDesc.m_NextEffectAngles = (*(Charge_Effect_Desc*)(pArg)).m_NextEffectAngles;
		m_ChargeEffectDesc.m_NextEffectTypeIndex = (*(Charge_Effect_Desc*)(pArg)).m_NextEffectTypeIndex;
		m_ChargeEffectDesc.m_NextEffectType = (*(Charge_Effect_Desc*)(pArg)).m_NextEffectType;

		m_ChargeEffectDesc.m_AttackDesc = (*(Charge_Effect_Desc*)(pArg)).m_AttackDesc;

		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &(*(Charge_Effect_Desc*)(pArg)).effectDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}


	return S_OK;
}

_uint CChargeEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	Loop_Count_Check(TimeDelta);

	Charge_Time_Check(TimeDelta);

	return _uint();
}

_uint CChargeEffect::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

CChargeEffect* CChargeEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChargeEffect* pInstance = new CChargeEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CChargeEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChargeEffect::Charge_Time_Check(const _double& TimeDelta)
{
	/*pSkill_Mananger->Do_Skill(Get_LayerTag().c_str(), m_iLevelindex, m_ChargeEffectDesc.nextAttackEffect,
				m_ChargeEffectDesc.nextAttackEffectPower, m_pTransformCom->Get_WorldMatrix_Matrix(), m_pModelCom, "RootNode", m_pTransformCom);*/

	if (m_ChargeTimeAcc < m_ChargeEffectDesc.m_ChargeTime)
	{
		m_ChargeTimeAcc += TimeDelta;
	}
	else
	{
		if (m_ChargeEffectDesc.m_NextEffectType == EFFECT_TYPE_ATTACK)
		{
			Attack_Effect_Add();
		}
	}
}

void CChargeEffect::Attack_Effect_Add()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));
	if (nullptr == pEffect_Manager)
		return;

	CSkillEffect* pSkillEffect = nullptr;
	if (m_ChargeEffectDesc.m_NextEffectNum > 1)
	{
		for (size_t i = 0; i < m_ChargeEffectDesc.m_NextEffectNum; ++i)
		{
			pSkillEffect = pEffect_Manager->CreateEffect(m_ChargeEffectDesc.m_NextEffectTypeIndex, m_ChargeEffectDesc.m_NextEffectPrototypeTag.c_str(),
				Get_LayerTag().c_str(), Get_Levelindex());

			static_cast<CAttackEffect*>(pSkillEffect)->Set_AttackDesc(m_ChargeEffectDesc.m_AttackDesc);
			_vector vParentLook = XMLoadFloat4((_float4*)m_EffectDesc.m_FinalWorldMatrix.m[2]);
			_vector vParentPos = XMLoadFloat4((_float4*)m_EffectDesc.m_FinalWorldMatrix.m[3]);

			vParentLook = XMVector3Rotate(vParentLook, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_ChargeEffectDesc.m_NextEffectAngles[i]));

			_float4 pos = {};
			if (m_ChargeEffectDesc.m_AttackDesc.effectDesc.m_IsParts)
			{
				XMStoreFloat4(&pos, vParentLook);
				pSkillEffect->Set_Pos(pos);
				pSkillEffect->Set_Parent(m_EffectDesc.pBonePtr, m_EffectDesc.pParent, m_EffectDesc.PivotMatrix);
			}
			else
			{
				XMStoreFloat4(&pos, vParentPos);
				pSkillEffect->Set_Pos(pos);
			}

			CAttack* pAttack = pSkillEffect->Get_As<CAttack>();
			if (nullptr != pAttack)
			{
				pAttack->Set_AttackPower(m_ChargeEffectDesc.m_NextEffectPower);
			}

			Safe_Release(pSkillEffect);
		}
	}
	else
	{
		pSkillEffect = pEffect_Manager->CreateEffect(m_ChargeEffectDesc.m_NextEffectTypeIndex, m_ChargeEffectDesc.m_NextEffectPrototypeTag.c_str(),
			Get_LayerTag().c_str(), Get_Levelindex());

		static_cast<CAttackEffect*>(pSkillEffect)->Set_AttackDesc(m_ChargeEffectDesc.m_AttackDesc);

		CAttack* pAttack = pSkillEffect->Get_As<CAttack>();
		if (nullptr != pAttack)
		{
			pAttack->Set_AttackPower(m_ChargeEffectDesc.m_NextEffectPower);
		}

		_vector vParentLook = m_EffectDesc.pParent->Get_State(CTransform::STATE_LOOK);
		_vector vParentPos = XMLoadFloat4((_float4*)m_EffectDesc.m_FinalWorldMatrix.m[3]);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr != pTransform)
		{
			pTransform->Set_State(CTransform::STATE_LOOK, vParentLook);
		}

		_float4 pos = {};
		if (m_ChargeEffectDesc.m_AttackDesc.effectDesc.m_IsParts)
		{
			XMStoreFloat4(&pos, vParentLook);
			pSkillEffect->Set_Pos(pos);
			pSkillEffect->Set_Parent(m_EffectDesc.pBonePtr, m_EffectDesc.pParent, m_EffectDesc.PivotMatrix);
		}
		else
		{
			XMStoreFloat4(&pos, vParentPos);
			pSkillEffect->Set_Pos(pos);
		}


		Safe_Release(pSkillEffect);
	}

	Set_Dead();
}


CGameObject* CChargeEffect::Clone(const _tchar * pLayerTag, _uint iLevelIndex, void* pArg)
{
	CChargeEffect* pInstance = new CChargeEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CChargeEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChargeEffect::Free()
{
	__super::Free();
}
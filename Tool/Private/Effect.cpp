#include "stdafx.h"
#include "Effect.h"

#include "GameInstance.h"

#include "Loader.h"
#include "Bone.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg)
	{
		m_EffectDesc.m_actionType = (*(EFFECT_DESC*)(pArg)).m_actionType;
		m_EffectDesc.m_effectPath = (*(EFFECT_DESC*)(pArg)).m_effectPath;
		m_EffectDesc.m_ProtoTypeTag = (*(EFFECT_DESC*)(pArg)).m_ProtoTypeTag;

		m_EffectDesc.m_effectType = (*(EFFECT_DESC*)(pArg)).m_effectType;
		m_EffectDesc.m_exPath1 = (*(EFFECT_DESC*)(pArg)).m_exPath1;
		m_EffectDesc.m_exPath2 = (*(EFFECT_DESC*)(pArg)).m_exPath2;
		m_EffectDesc.m_soundEventID = (*(EFFECT_DESC*)(pArg)).m_soundEventID;
		m_EffectDesc.m_soundEventTag = (*(EFFECT_DESC*)(pArg)).m_soundEventTag;
		m_EffectDesc.m_underFlag = (*(EFFECT_DESC*)(pArg)).m_underFlag;

		m_EffectDesc.m_bCollision = (*(EFFECT_DESC*)(pArg)).m_bCollision;
		m_EffectDesc.m_AnimationLoopTime = (*(EFFECT_DESC*)(pArg)).m_AnimationLoopTime;
	}


	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_BLEND;

	_float3 vPos{};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pNavigationCom->Set_Index_By_Position({ vPos.x, vPos.y, vPos.z });

	m_pModelCom->Set_Animation(0);

	return S_OK;
}


_uint CEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	Attack_Time_Check(TimeDelta);
	Loop_Count_Check(TimeDelta);

	if (m_pTransformCom)
	{
		Small_Rotation(TimeDelta);
		Rush(TimeDelta);
		Homming(TimeDelta);
		Big_Rotation(TimeDelta);
	}

	return _uint();
}

_uint CEffect::LateTick(_double TimeDelta)
{
	if (nullptr != m_EffectDesc.pBonePtr && nullptr != m_EffectDesc.pParent)
	{
		_matrix		ParentMatrix = m_EffectDesc.pBonePtr->Get_OffsetMatrix() *
			m_EffectDesc.pBonePtr->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_EffectDesc.PivotMatrix);

		REMOVE_SCALE(ParentMatrix)

		if (m_EffectDesc.m_bParentRotateApply)
			XMStoreFloat4x4(&m_EffectDesc.m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * ParentMatrix * m_EffectDesc.pParent->Get_WorldMatrix_Matrix());
		else
			XMStoreFloat4x4(&m_EffectDesc.m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * m_EffectDesc.pParent->Get_Position_Matrix());


		if (m_pColliderCom)
			m_pColliderCom->Tick(XMLoadFloat4x4(&m_EffectDesc.m_FinalWorldMatrix));

		m_pNavigationCom->Set_Index_By_Position({ m_EffectDesc.m_FinalWorldMatrix.m[3][0], 
			m_EffectDesc.m_FinalWorldMatrix.m[3][1], m_EffectDesc.m_FinalWorldMatrix.m[3][2] });
	}
	else
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pNavigationCom->Set_Index_By_Position({ XMVectorGetX(vPos), XMVectorGetY(vPos), XMVectorGetZ(vPos) });

		if (m_pColliderCom)
			m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);


#ifdef _DEBUG
	if (m_pColliderCom)
		m_pColliderCom->Render();

	m_pNavigationCom->Render();

#endif // _DEBUG

	return _uint();
}

HRESULT CEffect::Render()
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

	return S_OK;
}

void CEffect::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	if (m_pColliderCom)
	{
		_vector vDestCenter = m_pColliderCom->Get_Center();
		_vector vSourCenter = pOther->Get_Center();
		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		if (m_pAttackCom)
		{
			CHP* pOtherHpCom = pOtherOwner->Get_As<CHP>();

			if (nullptr != pOtherHpCom)
			{
				if (m_AttackTimeAcc <= 0.0)
				{
					pOtherHpCom->Get_Damage(m_pAttackCom->Get_AttackPower());
					m_AttackTimeAcc = m_AttackTime;
				}
			}
		}

		if (m_EffectDesc.m_bKnockBack)
		{
			CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
			if (!pOtherTransform)
				return;

			CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();


			if (fX > fZ)
			{

				if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
				{
					pOtherTransform->Move(0.f, 0.f, fZ, pNavigationCom);
				}
				else
				{
					pOtherTransform->Move(0.f, 0.f, -fZ, pNavigationCom);
				}

			}
			else if (fX == fZ) {}
			else
			{
				if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
				{
					pOtherTransform->Move(fX, 0.f, 0.f, pNavigationCom);

				}
				else
				{
					pOtherTransform->Move(-fX, 0.f, 0.f, pNavigationCom);
				}
			}
		}
	}
}

void CEffect::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	if (m_pColliderCom)
	{
		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
		if (!pOtherTransform)
			return;

		if (m_pAttackCom)
		{
			CHP* pOtherHpCom = pOtherOwner->Get_As<CHP>();

			if (nullptr != pOtherHpCom)
			{
				pOtherHpCom->Get_Damage(m_pAttackCom->Get_AttackPower());
			}
		}

		if (m_EffectDesc.m_bKnockBack)
		{
			_vector vDestCenter = m_pColliderCom->Get_Center();
			_vector vSourCenter = pOther->Get_Center();

			CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();
			if (fX > fZ)
			{

				if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
				{
					pOtherTransform->Move(0.f, 0.f, fZ, pNavigationCom);

				}
				else
				{
					pOtherTransform->Move(0.f, 0.f, -fZ, pNavigationCom);
				}
			}
			else if (fX == fZ) {}
			else
			{
				if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
				{
					pOtherTransform->Move(fX, 0.f, 0.f, pNavigationCom);

				}
				else
				{
					pOtherTransform->Move(-fX, 0.f, 0.f, pNavigationCom);
				}
			}
		}
	}
}

void CEffect::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CEffect::Set_Pos(const _float4& vPos)
{
	if (m_pTransformCom)
	{
		if (m_EffectDesc.m_effectPath.find(L"E_BD") != wstring::npos)
		{
			m_pTransformCom->Set_Pos(vPos.x, -0.2f , vPos.z);
		}

		else
		{
			m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}
}

void CEffect::Set_Parent(CBone* pBoneParent, CTransform* pTransformParent, _float4x4 PivotMatrix)
{
	if (nullptr == pBoneParent || nullptr == pTransformParent)
		return;

	m_EffectDesc.PivotMatrix = PivotMatrix;

	Set_ParentBone(pBoneParent);
	Set_ParentTransform(pTransformParent);

	m_EffectDesc.m_IsParts = true;
}

void CEffect::Set_AnimaitonStartTime(_double time)
{
	m_EffectDesc.m_AnimationStartAcc = time;
	if (m_pModelCom)
	{
		m_pModelCom->Set_Animation_Start_Time(m_EffectDesc.m_AnimationStartAcc);
	}
}

void CEffect::Set_AttackPower(_uint attackPower)
{
	if (nullptr == m_pAttackCom)
		return;

	m_pAttackCom->Set_AttackPower(attackPower);
}

void CEffect::Attack_Time_Check(const _double& TimeDelta)
{
	if (m_AttackTimeAcc > 0.0)
		m_AttackTimeAcc -= TimeDelta;
}

void CEffect::Loop_Count_Check(const _double& TimeDelta)
{
	if (m_pModelCom->Play_Animation(TimeDelta * m_EffectDesc.m_AnimationSpeed))
	{
		m_pModelCom->Set_Animation_Start_Time(m_EffectDesc.m_AnimationStartAcc);

		--m_EffectDesc.m_CurrentLoopCount;
	}
}

void CEffect::Small_Rotation(const _double& TimeDelta)
{
	if (m_EffectDesc.m_SmallRotation)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)(TimeDelta * m_EffectDesc.m_SmallRotationSpeed));
	}
}

void CEffect::Rush(const _double& TimeDelta)
{
	if (m_EffectDesc.m_bRush)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Go_Straight((_float)(TimeDelta * m_EffectDesc.m_RushSpeed));
	}
}

void CEffect::Homming(const _double& TimeDelta)
{
	if (m_EffectDesc.m_bHomming)
	{
		_vector vDir = {};
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vParentPos = {};
		if (!m_EffectDesc.pParent)
		{
			//vParentPos = m_EffectDesc.pParent->Get_State(CTransform::STATE_POSITION);
		}
		else
		{
			vParentPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}

		switch (m_EffectDesc.m_eHommingState)
		{
		case HOMMING_OUT:
			vDir = XMVector3Normalize(vPos - vParentPos);
			if (m_pTransformCom->Get_Positin_Length() >= m_EffectDesc.m_BigRotationRadius)
			{
				m_EffectDesc.m_eHommingState = HOMMING_IN;
			}
			break;
		case HOMMING_IN:
			vDir = XMVector3Normalize(vParentPos - vPos);
			if (XMVectorGetX(XMVector3Length(vParentPos - vPos)) <= 0.3f)
			{
				m_EffectDesc.m_eHommingState = HOMMING_OUT;
			}
			break;
		}

		vPos += vDir * (_float)TimeDelta;
		m_pTransformCom->Set_Pos(vPos);
	}
}

void CEffect::Big_Rotation(const _double& TimeDelta)
{
	if (m_EffectDesc.m_BigRotation)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };

		_matrix BigRot = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vUp, XMConvertToRadians(m_EffectDesc.m_BigRotationSpeed)));

		vPos = XMVector3TransformCoord(vPos, BigRot);

		m_pTransformCom->Set_Pos(vPos);
	}
}


HRESULT CEffect::Add_Components()
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
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_EffectDesc.m_ProtoTypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iIndex = 1;
	if (FAILED(pGameInstance->Add_Component(CNavigation::familyId, this, m_iLevelindex, TEXT("Prototype_Component_Navigation"),
		(CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	/* For.Com_OBB*/
	if (m_EffectDesc.m_bCollision)
	{
		CCollider::COLLIDER_DESC		ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
		ColliderDesc.vScale = _float3(2.2f, 2.0f, 2.2f);
		ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
		if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
			(CComponent**)&m_pColliderCom, &ColliderDesc)))
			return E_FAIL;

		CAttack::ATTACK_DESC attackDesc = {};
		attackDesc.m_AttackPower = 0;

		if (FAILED(pGameInstance->Add_Component(CAttack::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Attack"),
			(CComponent**)&m_pAttackCom, &attackDesc)))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CEffect::SetUp_ShaderResources()
{
	if (!m_EffectDesc.m_IsParts)
	{
		if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_EffectDesc.m_FinalWorldMatrix)))
			return E_FAIL;
	}

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

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_EffectDesc.pBonePtr);

	Safe_Release(m_EffectDesc.pParent);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pAttackCom);
}

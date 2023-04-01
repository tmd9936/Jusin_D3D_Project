#include "stdafx.h"
#include "Player.h"

#include "GameInstance.h"

#include "Skill_Manager.h"
#include "Skill.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CMonster(rhs)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;


	//m_pTransformCom->Set_Pos(22.f, 0.f, 13.5f);

	m_eRenderId = RENDER_NONBLEND;

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

_uint CPlayer::Tick(_double TimeDelta)
{
	_float4x4 mat = {};
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::ATK_NORMAL:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND, m_pModelCom);
		}
		break;
	case CMonFSM::JUMPLANDING_SLE_START:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_LOOP, m_pModelCom);
			mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			m_pTransformCom->Set_PosY(mat.m[3][2]);
		}
		break;
	case CMonFSM::JUMPLANDING_SLE_LOOP:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::ROTATE_LOOP, m_pModelCom);
		}
		break;
	case CMonFSM::ROTATE_LOOP:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
		}
		break;
	//case CMonFSM::JUMPLANDING_SLE_LOOP:
	//	m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
	//	break;
	case CMonFSM::JUMPLANDING_SLE_END:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			m_pTransformCom->Set_PosY(mat.m[3][2]);
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
		break;
	default:
		break;
	}
	return _uint();

	_bool move = false;
	if (KEY_TAB(KEY::DOWN))
	{
		if (m_pMonFSM->Get_MotionState() != CMonFSM::IDLE_GROUND)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND, m_pModelCom);
		}
		m_pTransformCom->Go_Backward((_float)TimeDelta, m_pNavigationCom);
	}
	else if (KEY_HOLD(KEY::DOWN))
	{
		m_pTransformCom->Go_Backward((_float)TimeDelta, m_pNavigationCom);
	}
	else  if (KEY_AWAY(KEY::DOWN))
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta * -1.f);
		//m_pModelCom->Set_Animation(41);

	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta);
		//m_pModelCom->Set_Animation(41);

	}

	if (KEY_TAB(KEY::UP))
	{
		if (m_pMonFSM->Get_MotionState() != CMonFSM::IDLE_GROUND)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND, m_pModelCom);
		}

		m_pTransformCom->Go_Straight((_float)TimeDelta, m_pNavigationCom);
	}
	else if (KEY_HOLD(KEY::UP))
	{
		m_pTransformCom->Go_Straight((_float)TimeDelta, m_pNavigationCom);
	}
	else  if (KEY_AWAY(KEY::UP))
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
	}

	else  if (KEY_AWAY(KEY::SPACE))
	{
		Do_Skill();
	}

	else  if (KEY_AWAY(KEY::S))
	{
		Jump_Rotate();
	}

	m_pAABB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	m_pOBB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	m_pSphere->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());

	return _uint();
}

_uint CPlayer::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CPlayer::Render()
{


	return __super::Render();
}

void CPlayer::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	if (fX > fZ)
	{
		_vector vDestCenter = m_pAABB->Get_Center();
		_vector vSourCenter = pOther->Get_Center();

		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
		if (!pOtherTransform)
			return;

		CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();

		if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
		{
			pOtherTransform->Move(0.f, 0.f, fZ * 0.0166f, pNavigationCom);
		}
		else
		{
			pOtherTransform->Move(0.f, 0.f, -fZ * 0.0166f, pNavigationCom);
		}
		
	}
	else if (fX == fZ) {}
	else
	{
		_vector vDestCenter = m_pAABB->Get_Center();
		_vector vSourCenter = pOther->Get_Center();

		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();

		if (!pOtherTransform)
			return;
		
		CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();

		if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
		{
			pOtherTransform->Move(fX * 0.0166f, 0.f, 0.f, pNavigationCom);

		}
		else
		{
			pOtherTransform->Move(-fX * 0.0166f, 0.f, 0.f, pNavigationCom);
		}
	}
}

void CPlayer::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	if (fX > fZ)
	{
		_vector vDestCenter = m_pAABB->Get_Center();
		_vector vSourCenter = pOther->Get_Center();

		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
		if (!pOtherTransform)
			return;

		CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();

		if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
		{
			pOtherTransform->Move(0.f, 0.f, fZ * 0.0166f, pNavigationCom);

		}
		else
		{
			pOtherTransform->Move(0.f, 0.f, -fZ * 0.0166f, pNavigationCom);
		}
	}
	else if (fX == fZ) {}
	else
	{
		_vector vDestCenter = m_pAABB->Get_Center();
		_vector vSourCenter = pOther->Get_Center();

		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
		if (!pOtherTransform)
			return;

		CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();

		if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
		{
			pOtherTransform->Move(fX * 0.0166f, 0.f, 0.f, pNavigationCom);

		}
		else
		{
			pOtherTransform->Move(-fX * 0.0166f, 0.f, 0.f, pNavigationCom);
		}
	}
}

void CPlayer::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CPlayer::Do_Skill()
{
	if (m_pMonFSM->Get_MotionState() != CMonFSM::ATK_NORMAL)
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom);

		CGameObject* pSkill_Mananger = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
		if (nullptr != pSkill_Mananger)
		{
			CSkill* pSkill = dynamic_cast<CSkill_Manager*>(pSkill_Mananger)->Create_Skill(L"Layer_PlayerSkill", m_iLevelindex, m_TestSkillindex);
			if (nullptr != pSkill)
			{
				_float4 vPos{};
				_float4 vPosLook{};

				XMStoreFloat4(&vPosLook, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.2f);
				pSkill->Set_Effects_Pos(vPos);
				pSkill->Set_Conditions_Pos(vPos);

				pSkill->Set_Effects_Look(vPosLook);
				pSkill->Set_Conditions_Look(vPosLook);

				Safe_Release(pSkill);
			}
		}
	}
}

void CPlayer::Jump_Rotate()
{
	m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_START, m_pModelCom);

}

//HRESULT CPlayer::SetUp_ShaderResources()
//{
//	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
//		return E_FAIL;
//
//	CGameInstance* pGameInstance = CGameInstance::GetInstance();
//	Safe_AddRef(pGameInstance);
//
//	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
//		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
//		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition",
//		&pGameInstance->Get_CamPosition(), sizeof(_float4))))
//		return E_FAIL;
//
//	const LIGHTDESC* pLightDesc = pGameInstance->Get_Light(0);
//	if (nullptr == pLightDesc)
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir",
//		&pLightDesc->vDirection, sizeof(_float4))))
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse",
//		&pLightDesc->vDiffuse, sizeof(_float4))))
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient",
//		&pLightDesc->vAmbient, sizeof(_float4))))
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular",
//		&pLightDesc->vSpecular, sizeof(_float4))))
//		return E_FAIL;
//
//	Safe_Release(pGameInstance);
//
//	return S_OK;
//}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

}

HRESULT CPlayer::Add_TransitionRandomState()
{
	return S_OK;
}

_uint CPlayer::State_Tick(const _double& TimeDelta)
{
	return _uint();
}

CGameObject* CPlayer::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	return nullptr;
}

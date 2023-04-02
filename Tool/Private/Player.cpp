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
	m_SkillLoopDesc.m_eLoopState = CMonFSM::END_MOTION;

	return S_OK;
}

_uint CPlayer::Tick(_double TimeDelta)
{
	_float4x4 mat = {};
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		m_pModelCom->Play_Animation(TimeDelta);
		if (m_SkillLoopDesc.m_eLoopState != CMonFSM::END_MOTION)
		{
			if (m_SkillLoopDelay < 0.f)
			{
				Do_Skill(m_PokemonDesc.m_skillIDs[m_SkillLoopDesc.m_CurskillIndex], m_SkillLoopDesc.m_eLoopState, L"Layer_PlayerSkill");
				//m_pMonFSM->Transit_MotionState(m_SkillLoopDesc.m_eLoopState, m_pModelCom);
			}
			m_SkillLoopDelay -= TimeDelta;
		}
		break;
	case CMonFSM::ATK_NORMAL:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			if (m_bAttack)
				m_bAttack = false;

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
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
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
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			m_pTransformCom->Set_PosY(mat.m[3][2]);
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
		break;
	case CMonFSM::HAPPY:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			if (m_bAttack)
				m_bAttack = false;
		}
		break;
	case CMonFSM::DASH_SLE_START:
		m_pTransformCom->Go_Straight(TimeDelta * m_fAccel, m_pNavigationCom);
		m_fAccel += TimeDelta * 3.f;
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::DASH_SLE_LOOP, m_pModelCom);
		}
		break;
	case  CMonFSM::DASH_SLE_LOOP:
		m_pTransformCom->Go_Straight(TimeDelta * m_fAccel, m_pNavigationCom);
		m_fAccel += TimeDelta * 3.5f;
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::DASH_SLE_END, m_pModelCom);
		}
		break;
	case CMonFSM::DASH_SLE_END:
		m_fAccel -= TimeDelta * 3.f;
		m_pTransformCom->Go_Straight(TimeDelta * m_fAccel, m_pNavigationCom);
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			if (m_SkillLoopCount > 0)
			{
				--m_SkillLoopCount;
				m_SkillLoopDesc.m_eLoopState = CMonFSM::DASH_SLE_START;
				m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom); 
				m_SkillLoopDesc.m_CurskillIndex = 1;
				m_fAccel = 1.5f;

				break;
			}
			else
			{
				m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
				m_SkillLoopDesc.m_eLoopState = CMonFSM::END_MOTION;

				Do_Skill(220, CMonFSM::JUMPLANDING_SLE_LOOP, L"Layer_PlayerSkill");

				if (m_bAttack)
					m_bAttack = false;
			}
			m_fAccel = 1.5f;
		}
		break;
	default:
		break;
	}

	if (KEY_TAB(KEY::DOWN))
	{
		if (!m_bAttack)
		{
			if (m_pMonFSM->Get_MotionState() != CMonFSM::IDLE_FLOAT)
			{
				m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_FLOAT, m_pModelCom);
			}
		}
		m_pTransformCom->Go_Backward((_float)TimeDelta, m_pNavigationCom);
	}
	else if (KEY_HOLD(KEY::DOWN))
	{
		if (!m_bAttack)
		{
			m_pTransformCom->Go_Backward((_float)TimeDelta, m_pNavigationCom);
		}
	}
	else  if (KEY_AWAY(KEY::DOWN))
	{
		if (!m_bAttack)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta * -1.f);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta);
	}

	if (KEY_TAB(KEY::UP))
	{
		if (!m_bAttack)
		{
			if (m_pMonFSM->Get_MotionState() != CMonFSM::IDLE_FLOAT)
			{
				m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_FLOAT, m_pModelCom);
			}
		}

		m_pTransformCom->Go_Straight((_float)TimeDelta, m_pNavigationCom);
	}
	else if (KEY_HOLD(KEY::UP))
	{
		if (!m_bAttack)
		{
			m_pTransformCom->Go_Straight((_float)TimeDelta, m_pNavigationCom);
		}
	}
	else  if (KEY_AWAY(KEY::UP))
	{
		if (!m_bAttack)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
	}

	else  if (KEY_TAB(KEY::A))
	{
		Do_Skill(m_PokemonDesc.m_skillIDs[0], CMonFSM::ATK_NORMAL, L"Layer_PlayerSkill");
		m_SkillLoopDesc.m_CurskillIndex = 0;
	}

	else  if (KEY_TAB(KEY::S))
	{
		Do_Skill(m_PokemonDesc.m_skillIDs[1], CMonFSM::DASH_SLE_START, L"Layer_PlayerSkill");
		m_SkillLoopCount = 1;
		m_SkillLoopDelay = 1.f;
		m_SkillLoopDesc.m_CurskillIndex = 1;
	}

	else  if (KEY_TAB(KEY::D))
	{
		Do_Skill(m_PokemonDesc.m_skillIDs[2], CMonFSM::HAPPY, L"Layer_PlayerSkill");
		m_SkillLoopDesc.m_CurskillIndex = 2;
	}

	else  if (KEY_TAB(KEY::W))
	{
		Do_Skill(m_PokemonDesc.m_skillIDs[3], CMonFSM::ATK_NORMAL, L"Layer_PlayerSkill");
		m_SkillLoopDesc.m_CurskillIndex = 3;
	}

	else if (KEY_TAB(KEY::SPACE))
	{
		Do_TestSkill();
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

void CPlayer::Do_TestSkill()
{
	if (m_pMonFSM->Get_MotionState() != CMonFSM::ATK_NORMAL)
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom);

		CGameObject* pSkill_Mananger = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
		if (nullptr != pSkill_Mananger)
		{
			CSkill* pSkill = dynamic_cast<CSkill_Manager*>(pSkill_Mananger)->Create_Test_Skill(L"Layer_PlayerSkill", m_iLevelindex, m_TestSkillindex,
				m_pTransformCom->Get_WorldMatrix_Matrix());

			Safe_Release(pSkill);
		}
	}
}

void CPlayer::Jump_Rotate()
{
	m_pMonFSM->Transit_MotionState(CMonFSM::HAPPY, m_pModelCom);

}


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

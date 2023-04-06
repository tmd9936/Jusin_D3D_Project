#pragma once

#include "SkillEffect.h"

BEGIN(Engine)
class CCollider;
class CAttack;
class CHP;
END

BEGIN(Client)

class CSkill_Manager;

class CAttackEffect :
	public CSkillEffect
{
public:
	typedef struct Attack_Effect_Desc
	{
		//_bool					m_bRush = { false };
		//_vector					m_vRushDirection = {};
		//_double					m_RushSpeed = { 1.0 };

		//_bool					m_bHomming = { false };
		//_bool					m_bArriveHomeDead = { false };
		//HOMMING_STATE			m_eHommingState = { HOMMING_END };

		//_bool					m_IsParts = { false };
		//_bool					m_IsHomming = { false };
		//_bool					m_bParentRotateApply = { true };

		//_bool					m_SmallRotation = { false };
		//_float					m_SmallRotationSpeed = { 0.f };

		//_bool					m_BigRotation = { false };
		//_float					m_BigRotationRadius = { 0.f };
		//_float					m_BigRotationSpeed = { 0.f };

		_double					m_AttackTime = { 1.0 };
		_bool					m_bKnockBack = { false };
		_bool					m_bContinue = { false };
		_uint					m_CollisionEffectNum = { 0 };

		EFFECT_DESC	effectDesc;

	} ATTACK_EFFECT_DESC;

private:
	CAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAttackEffect(const CAttackEffect& rhs);
	virtual ~CAttackEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

protected:
	void		Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

protected:
	ATTACK_EFFECT_DESC	m_AttackEffectDesc = {};

protected:
	CCollider*			m_pColliderCom = { nullptr };
	CAttack*			m_pAttackCom = { nullptr };

protected:
	_double				m_AttackTimeAcc = { 0.0 };

public:
	friend CLoader;
	static CAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END
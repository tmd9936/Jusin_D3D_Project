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
		_double					m_AttackTime = { 1.0 };
		_bool					m_bKnockBack = { false };
		_bool					m_bContinue = { false };
		_uint					m_CollisionEffectType = { 0 };

		EFFECT_DESC	effectDesc;

	} ATTACK_EFFECT_DESC;

protected:
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
	virtual HRESULT Add_Components();

protected:
	void		Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void		Attack_Time_Check(const _double& TimeDelta);

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
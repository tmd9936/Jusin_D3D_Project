#pragma once

#include "SkillEffect.h"

#include "HommingAttackEffect.h"
#include "RushAttackEffect.h"

BEGIN(Client)

class CSkill_Manager;

class CChargeEffect :
    public CSkillEffect
{

public:
    typedef struct Charge_Effect_Desc
    {
		ATTACK_TYPE	m_NextAttackEffectType = { ATTACK_END };
		_uint       m_NextAttackEffect = { 0 };
		_uint       m_NextAttackEffectPower = { 0 };
		_double     m_ChargeTime = { 0.0 };

		CHommingAttackEffect::HOMMING_ATTACK_EFFECT_DESC  m_HommingAttacEffectDesc = {};
		CRushAttackEffect::RUSH_ATTACK_EFFECT_DESC  m_RushAttacEffectDesc = {};
		CAttackEffect::ATTACK_EFFECT_DESC m_NormalAtackDesc = {};

		EFFECT_DESC	effectDesc;

    } CHARGE_EFFECT_DESC;

private:
	CChargeEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChargeEffect(const CChargeEffect& rhs);
	virtual ~CChargeEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:

protected:
	void		Charge_Time_Check(const _double& TimeDelta);

protected:
	CHARGE_EFFECT_DESC	m_ChargeEffectDesc = {};

	_double				m_ChargeTimeAcc = { 0.0 };

public:
	friend CLoader;
	static CChargeEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END
#pragma once

#include "SkillEffect.h"

#include "HommingAttackEffect.h"
#include "RushAttackEffect.h"

BEGIN(Engine)
class CAttack;
END

BEGIN(Client)

class CSkill_Manager;
class CEffect_Manager;

class CChargeEffect :
    public CSkillEffect
{

public:
    typedef struct Charge_Effect_Desc
    {
		wstring			m_NextEffectPrototypeTag;
		_uint			m_NextEffectPower = { 0 };
		_double			m_ChargeTime = { 0.0 };
		_uint			m_NextEffectNum = { 1 };
		vector<_float>	m_NextEffectAngles;

		_uint			m_NextEffectTypeIndex = { 0 };
		EFFECT_TYPE		m_NextEffectType = { EFFECT_TYPE_END };

		_float3			m_vScale = { 1.f, 1.f, 1.f };
		
		//_uint			m_CollisionEffectType = { 0 };

		//CHommingAttackEffect::HOMMING_ATTACK_EFFECT_DESC  m_HommingAttackEffectDesc = {};
		//CRushAttackEffect::RUSH_ATTACK_EFFECT_DESC  m_RushAttackEffectDesc = {};
		CAttackEffect::ATTACK_EFFECT_DESC m_AttackDesc = {};

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

protected:
	void		Charge_Time_Check(const _double& TimeDelta);
	void		Attack_Effect_Add();

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
#pragma once

#include "AttackEffect.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSkill_Manager;

class CHommingAttackEffect :
	public CAttackEffect
{
public:
	enum HOMMING_STATE
	{
		HOMMING_OUT,
		HOMMING_IN,
		HOMMING_END
	};

public:
	typedef struct Homming_Attack_Effect_Desc
	{
		_bool					m_bArriveHomeDead = { false };
		_bool					m_bParentRotateApply = { true };
		HOMMING_STATE			m_eHommingState = { HOMMING_END };

		_bool					m_SmallRotation = { true };
		_float					m_SmallRotationSpeed = { 0.f };

		_bool					m_BigRotation = { true };
		_float					m_BigRotationRadius = { 0.f };
		_float					m_BigRotationSpeed = { 0.f };

		ATTACK_EFFECT_DESC		attackEffectDesc;

	} HOMMING_ATTACK_EFFECT_DESC;

private:
	CHommingAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHommingAttackEffect(const CHommingAttackEffect& rhs);
	virtual ~CHommingAttackEffect() = default;

public:
	virtual HRESULT Initialize_Prototype(Homming_Attack_Effect_Desc& desc); /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	//virtual _uint LateTick(_double TimeDelta) override;

public:
	void	Set_SmallRotation(_float speed) {
		m_HommingAttackEffectDesc.m_SmallRotation = true;
		m_HommingAttackEffectDesc.m_SmallRotationSpeed = speed;
		
	}

	void	Set_BigRotation(_float speed, _float radius) {
		m_HommingAttackEffectDesc.m_BigRotation = true;
		m_HommingAttackEffectDesc.m_BigRotationSpeed = speed;
		m_HommingAttackEffectDesc.m_BigRotationRadius = radius;
	}

	void	Set_Homming(_bool bHomming, _bool bArriveHomeDead, HOMMING_STATE eHommingState) {
		m_HommingAttackEffectDesc.m_bArriveHomeDead = bArriveHomeDead;
		m_HommingAttackEffectDesc.m_eHommingState = eHommingState;
	}

protected:
	void		Small_Rotation(const _double& TimeDelta);
	void		Homming(const _double& TimeDelta);
	void		Big_Rotation(const _double& TimeDelta);

protected:
	HOMMING_ATTACK_EFFECT_DESC	m_HommingAttackEffectDesc = {};

public:
	friend CLoader;
	static CHommingAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Homming_Attack_Effect_Desc& desc);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END
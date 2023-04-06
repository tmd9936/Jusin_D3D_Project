#pragma once
#include "Effect.h"
class CChargeEffect :
    public CEffect
{
public:
    typedef struct Charge_Effect_Desc
    {
        _uint       nextAttackEffect;
        _double     chargeTime;

		EFFECT_DESC	effectDesc;

    }CHARGE_EFFECT_DESC;

private:
	CChargeEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChargeEffect(const CChargeEffect& rhs);
	virtual ~CChargeEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

	friend CLoader;
	static CChargeEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

protected:
	void		Charge_Time_Check(const _double& TimeDelta);

protected:
	_double		m_ChargeTImeAcc = { 0.0 };

protected:
	CHARGE_EFFECT_DESC	m_ChargeEffectDesc = {};

public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};


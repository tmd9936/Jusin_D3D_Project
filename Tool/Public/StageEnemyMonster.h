#pragma once

#include "Client_Defines.h"
#include "Monster.h"

#include "Utility.h"

BEGIN(Client)
class CStageEnemyMonster : public CMonster
{
private:
	CStageEnemyMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStageEnemyMonster(const CMonster& rhs);
	virtual ~CStageEnemyMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Change_State_FSM(_uint eState) override;

public:
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

protected:
	virtual HRESULT Add_TransitionRandomState() override;
	virtual _uint State_Tick(const _double& TimeDelta) override;

protected:
	void		MotionChange_Random();
	void		Do_RandomSkill();

private:
	void		AI_Type_Long_Idle_Tick(const _double& TimeDelta, CTransform* pTargetTransform);

private:
	_float		m_MotionChangeDelay = { 3.f };
	_float		m_CurMotionChangeDelayTime = { 0.f };

	_bool		m_bTurn = { false };

	_vector		m_vTargetPos = {};

	_double		m_ChaseCoolTime = { 2.0 };
	_double		m_ChaseCoolTimeAcc = { 0.0 };
	_double		m_bChase = { true };

public:
	static CStageEnemyMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
	virtual void Free() override;

};

END


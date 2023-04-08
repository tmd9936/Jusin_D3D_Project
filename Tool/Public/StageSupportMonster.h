#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CFormation;

END

BEGIN(Client)


class CStageSupportMonster : public CMonster
{
private:
	CStageSupportMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStageSupportMonster(const CMonster& rhs);
	virtual ~CStageSupportMonster() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

public:
	virtual void			Change_State_FSM(_uint eState) override;

protected:
	virtual HRESULT			Add_TransitionRandomState() override;
	virtual _uint			State_Tick(const _double& TimeDelta) override;

protected:
	void					Init_RandomMotionChangeDelay();
	void					Check_Do_Change_RandomMotion(const _double& TimeDelta);
	void					MotionChange_Random();
	void					Do_RandomSkill();
	void					Go_To_RandomPosition(const _double& TimeDelta);

protected:
	HRESULT					Init_MainPlayer();

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

protected:
	virtual		HRESULT		Load_By_Json_PreAddComponents() override;

//protected:
//	virtual HRESULT			Add_Components() override;
//	virtual HRESULT			Add_Components_By_File() override;

private:
	CFormation*			m_pFormationCom = { nullptr };

private:
	_float				m_MotionChangeDelay = { 3.f };
	_float				m_CurMotionChangeDelayTime = { 0.f };

	_bool				m_bTurn = { false };
	_bool				m_bBattle = { false };
	_vector				m_vTargetPos = {};

	CGameObject*		m_pMainPlayer = { nullptr };
	CTransform*			m_pMainPlayerTransform = { nullptr };

	_double				m_FormationFightTime = { 1.5 };
	_double				m_FormationFightTimeAcc = { 0.0 };
	_bool				m_FormationChanger = { false };

public:
	static CStageSupportMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
	virtual void Free() override;

};

END


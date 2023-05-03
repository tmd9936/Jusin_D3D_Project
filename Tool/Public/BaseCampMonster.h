#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CBaseCampMonster : public CMonster
{
private:
	CBaseCampMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseCampMonster(const CMonster& rhs);
	virtual ~CBaseCampMonster() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Change_State_FSM(_uint eState) override;

protected:
	virtual HRESULT			Add_TransitionRandomState() override;
	virtual _uint			State_Tick(const _double& TimeDelta) override;

protected:
	void					Init_RandomMotionChangeDelay();
	void					Check_Do_Change_RandomMotion(const _double& TimeDelta);
	void					MotionChange_Random();
	void					Go_To_RandomPosition(const _double& TimeDelta);

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	void					Play_SignitureSound();

private:
	_float					m_MotionChangeDelay = { 3.f };
	_float					m_CurMotionChangeDelayTime = { 0.f };

	_bool					m_bTurn = { false };

public:
	static CBaseCampMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
	virtual void Free() override;

};

END


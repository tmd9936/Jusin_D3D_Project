#pragma once
#include "Component.h"

BEGIN(Engine)

class CModel;
class CGameObject;

class ENGINE_DLL CMonFSM :
    public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_LOGIC_STATE_MACHINE;

public:
	enum MONSTER_STATE {
		IDLE1, IDLE2,
		ATK_NORMAL, ATK_SLE_NORMAL_END, ATK_SLE_NORMAL_LOOP, ATK_SLE_NORMAL_START, BODYBLOW,
		DASH_SLE_END, DASH_SLE_LOOP, DASH_SLE_START, DEAD_BOSS, DEAD_LASTBOSS_ESC, DEAD_LASTBOSS, DEAD_ROTATE,
		HAPPY, IDLE_FLOAT, IDLE_FLY, IDLE_GROUND, IDLE_NO, JOY, JUMPLANDING_SLE_END,
		JUMPLANDING_SLE_LOOP, JUMPLANDING_SLE_START, POKING, RETURN, ROAR, ROTATE_CHANGE, ROTATE_LOOP,
		RUN_CRAWL, RUN_FLOAT, RUN_FLY, RUN_GOUND2, RUN_GOUND4, RUN_NO, SMALL_SLE_END, SMALL_SLE_LOOP, SMALL_SLE, START,
		SUFFER, TALK, TREMBLING, TURN360_SLE_END, TURN360_SLE_LOOP, TURN360_SLE_START, VERTICAL_JUMP, 
		FORMATION_NORMAL, FORMATION_RUN, ROAR_BEFORE, STAGE_CLEAR, END_MOTION
	};

protected:
	explicit CMonFSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CMonFSM(const CMonFSM& rhs, CGameObject* pOwner);
	virtual ~CMonFSM() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_int			Update_Component(const _float& fTimeDelta, CModel* pModel = nullptr);

	MONSTER_STATE	Get_MotionState(void) const { 
		return m_eCurrentMotion; 
	}
	HRESULT			Add_MotionState(MONSTER_STATE eState, _uint index);
	HRESULT			Transit_MotionState(MONSTER_STATE eState, CModel* pModel = nullptr);
	HRESULT			Add_RandomTransitionState(MONSTER_STATE eState);

	void			Get_RandomState(CModel* pModel = nullptr);
	_uint			Find_MotionState(MONSTER_STATE eState);

private:
	MONSTER_STATE								m_eCurrentMotion = { IDLE1 };
	MONSTER_STATE								m_ePrevMotion = { END_MOTION };
	vector<MONSTER_STATE>						m_RandomState;
	unordered_map<MONSTER_STATE, _uint>			m_MotionState;

public:
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;
	static CMonFSM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void	Free() override;

};

END
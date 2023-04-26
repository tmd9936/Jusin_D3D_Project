#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
class CPickingCube;
class CMonFSM;
class CAABB;
class COBB;
class CSphere;
class CHP;
class CAttack;
class CManualCollisionState;
class CNavigation;

END

BEGIN(Client)

class CSkill_Manager;
class CSkill;
class CHpBar;
class CSearcher;
class CDamageText;
class CMiscData;
class CStageCamera;
class CBuffState;

class CMonster abstract : public CGameObject
{
public:
	enum AI_TYPE {
		AI_TYPE_LONG_DISTACE,
		AI_TYPE_SHORT_DISTACE,
		AI_TYPE_END
	};

	enum LAYER_TYPE {
		LAYER_TYPE_PLAYER,
		LAYER_TYPE_MONSTER,
		LAYER_TYPE_END
	};

public:
	typedef	struct PokeMon_Desc
	{
		wstring						ModelPrototypeTag;
		_uint						ModelPrototypeLevelIndex;

		_float4						vPos;

		_float						moveSpeed;
		_float						rotateSpeed;

		_uint						m_level;
		_int						m_exp;

		_uint						m_monsterNo;
		_uint						m_hpBasis;
		_uint						m_attackBasis;
		_uint						m_hpGrow;
		_uint						m_attackGrow;
		_uint						m_type1;
		_uint						m_type2;
		_uint						m_visitWeightDefault;
		_uint						m_visitWeight;
		_uint						m_cookTableID;
		_uint						m_color;
		_uint						m_Rate;
		_uint						m_isLayer;
		_uint						m_meleePercent;
		_uint						m_slotTypeWeightHp;
		_uint						m_slotTypeWeightAttack;
		_uint						m_slotTypeWeightMulti;

		_uint						m_AIType;
		_uint						m_layerType;

		_uint						m_normalSkillType;

		vector<_uint>				m_skillIDs;

		vector<STONE_EQUIP_DESC>	m_stones;

	} POKEMON_DESC;

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);
	
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_MovePosition() {
		m_MovePosition = { rand() % 10 + 12.f, 0.f, rand() % 10 + 15.f, 1.f };
	}

public:
	const _bool		Get_CanSkillAttack() const {
		return m_bCanSkillAttack;
	}

	const _float	Get_SkillCoolTimeProgress() const {
		return _float(m_SkillCoolTimeAcc / m_SkillCoolTime);
	}

	const _double	Get_SkillCoomTime() const {
		return m_SkillCoolTime;
	}

	void					Do_Skill_By_Index(_uint skillindex, const _tchar* pLayer);

public:
	CBuffState*				Search_NoAction_DeBuffState(const _uint& buffType);
	_bool					Is_AbNormalState(const _uint& buffType);
	_bool					Check_Monster_Can_GetAbNormalState(const _uint& buffType);


protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

protected:
	virtual	HRESULT			Load_By_Json_PreAddComponents();

protected:
	virtual HRESULT			Add_MotionState();
	virtual	HRESULT			Add_TransitionRandomState() { return S_OK; }
	virtual	_uint			State_Tick(const _double& TimeDelta) PURE;

protected:
	HRESULT					Add_HpBar();
	HRESULT					Add_BuffState();
	HRESULT					Add_Searcher();
	HRESULT					Add_DamageText();

	_bool					Do_Skill(_uint skillType, CMonFSM::MONSTER_STATE eMotion, const _tchar* pLayer);
	_bool					Do_Skill(_uint skillType, const _tchar* pLayer);

	void					Do_Skill_After_Set_Motion(_uint skillType, const _tchar* pLayer);

protected:
	void					CoolTimeCheck(const _double& TimeDelta);
	void					HitTimeCheck(const _double& TimeDelta);
	void					HitCheck();

protected:
	_bool					Search_Target();
	void					SkillCoolTime_Start();

protected:
	CBuffState*				Search_NoAction_BuffState(const _uint& skillType);

protected:
	virtual HRESULT			Add_Components();
	virtual HRESULT			Add_Components_By_File();

	HRESULT					SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

protected:
	CTransform*				m_pTransformCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CPickingCube*			m_pPickingCube = { nullptr };
	CMonFSM*				m_pMonFSM = { nullptr };

	CAABB*					m_pAABB = { nullptr };
	COBB*					m_pOBB = { nullptr };
	CSphere*				m_pSphere = { nullptr };

	CNavigation*			m_pNavigationCom = { nullptr };
	CManualCollisionState*	m_pManualCollisionState = { nullptr };

	CHP*					m_pHPCom = { nullptr };
	CAttack*				m_pAttackCom = { nullptr };

protected:
	POKEMON_DESC			m_PokemonDesc = {};
	_float4					m_MovePosition = { 20.f, 0.f, 20.f, 1.f };
	
	_bool					m_bAttack = { false };

	_double					m_AttackCoolTime = { 1.0 };
	_double					m_SkillCoolTime = { 3.0 };

	_double					m_AttackCoolTimeAcc = { 1.0 };
	_double					m_SkillCoolTimeAcc = { 3.0 };

	_bool					m_bCanAttack = { true };
	_bool					m_bCanSkillAttack = { true };

	_double					m_hitTime = { 0.6 };
	_double					m_hitTimeAcc = { 0.0 };
	_bool					m_bHitState = { false };

	_float4					m_hitColor = { 1.f, 1.f, 1.f, 1.f };
	_float4					m_deadColor = { 1.f, 1.f, 1.f, 1.f };

protected:
	vector<CBuffState*>		m_buffStates;
	CHpBar*					m_pHpBar = { nullptr };
	CSearcher*				m_pSearcher = { nullptr };
	CDamageText*			m_pDamageText = { nullptr };

	CGameObject*			m_pTarget = { nullptr };

public:
	//static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) ;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) PURE;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex,	const char* filePath) PURE;
	virtual void Free() override;
};

END
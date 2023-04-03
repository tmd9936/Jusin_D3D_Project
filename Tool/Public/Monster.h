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

class CNavigation;

END

BEGIN(Client)

class CSkill_Manager;
class CSkill;
class CHpBar;

class CMonster abstract : public CGameObject
{
public:
	typedef	struct PokeMon_Desc
	{
		wstring			ModelPrototypeTag;
		_uint			ModelPrototypeLevelIndex;

		_float4			vPos;

		_float			moveSpeed;
		_float			rotateSpeed;

		_uint			m_monsterNo;
		_uint			m_hpBasis;
		_uint			m_attackBasis;
		_uint			m_hpGrow;
		_uint			m_attackGrow;
		_uint			m_type1;
		_uint			m_type2;
		_uint			m_visitWeightDefault;
		_uint			m_visitWeight;
		_uint			m_cookTableID;
		_uint			m_color;
		_uint			m_Rate;
		_uint			m_isLayer;
		_uint			m_meleePercent;
		_uint			m_slotTypeWeightHp;
		_uint			m_slotTypeWeightAttack;
		_uint			m_slotTypeWeightMulti;

		_uint			m_AIType;

		_uint			m_normalSkillType;

		vector<_uint>	m_skillIDs;

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

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

protected:
	virtual HRESULT Add_MotionState();
	virtual	HRESULT Add_TransitionRandomState() PURE;
	virtual	_uint	State_Tick(const _double& TimeDelta) PURE;

protected:
	HRESULT Add_BuffState();

protected:
	HRESULT	Add_HpBar();
	void	Do_Skill(_uint skillType, CMonFSM::MONSTER_STATE eMotion, const _tchar* pLayer);

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

protected:
	CTransform*			m_pTransformCom = { nullptr };
	CRenderer*			m_pRendererCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CPickingCube*		m_pPickingCube = { nullptr };
	CMonFSM*			m_pMonFSM = { nullptr };

	CAABB*				m_pAABB = { nullptr };
	COBB*				m_pOBB = { nullptr };
	CSphere*			m_pSphere = { nullptr };

	CNavigation*		m_pNavigationCom = { nullptr };

	CHP*				m_pHPCom = { nullptr };
	CAttack*			m_pAttackCom = { nullptr };

protected:
	POKEMON_DESC		m_PokemonDesc = {};
	_float4				m_MovePosition = { 20.f, 0.f, 20.f, 1.f };
	
	_bool				m_bAttack = { false };
	_double				m_SkillCoolTime = { 0.f };

protected:

	vector<CGameObject*>	m_Parts;
	CHpBar* m_pHpBar = { nullptr };

public:
	//static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) ;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) PURE;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex,	const char* filePath) PURE;
	virtual void Free() override;
};

END
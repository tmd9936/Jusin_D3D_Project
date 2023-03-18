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
END

BEGIN(Client)

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

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

protected:
	virtual HRESULT Add_MotionState();
	virtual	HRESULT Add_TransitionRandomState() PURE;
	virtual	HRESULT	Set_ChangeStates() PURE;
	virtual	_uint	State_Tick(const _double& TimeDelta) PURE;

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

protected:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CPickingCube* m_pPickingCube = { nullptr };
	CMonFSM* m_pMonFSM = { nullptr };

private:
	POKEMON_DESC		m_PokemonDesc = {};

public:
	//static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) ;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) PURE;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex,	const char* filePath) PURE;
	virtual void Free() override;
};

END
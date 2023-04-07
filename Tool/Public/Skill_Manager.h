#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Skill.h"

BEGIN(Engine)
END

BEGIN(Client)

class CEffect;
class CEffect_Manager;

class CSkill_Manager final : public CGameObject
{
public:
	typedef	struct SkillDependData
	{
		vector<_uint> m_effects;
		vector<_uint> m_conditions;
	} SKILL_DEPEND_DATA;

private:
	CSkill_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Manager(const CSkill_Manager& rhs);
	virtual ~CSkill_Manager() = default;

public:
	HRESULT Initialize_Prototype(const char* skillDepnedDataFilePath, const char* skillDataResourceFilePath); /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	CSkill* Create_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _uint damage,
		_fmatrix vParentMatrix, _float smallRotationSpeed = 0.f, _float bigRotationSpeed = 0.f,
		CBone* pParentBone = nullptr, CTransform* pParentTransform = nullptr, _fmatrix PivotMatrix = XMMatrixIdentity(),
		_bool bRush = false, _double rushSpeed = 1.0, _uint extendNum = 1);

public: 
	HRESULT CreateSkill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _uint damage, _fmatrix vParentMatrix,
		CBone* pBone, CTransform* pParentTransform, _fmatrix PivotMatrix);

public:
	CSkill* Do_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _uint damage,
		_fmatrix vParentMatrix, CModel* pModel, const char* boneTag, CTransform* pParentTransform);

public:
	CSkill* Create_Test_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType,
		_fmatrix vParentMatrix);
	
public:
	HRESULT	Reload_Datas();

public:
	void	Get_Skill_Desces(vector<CSkill::SKILL_DESC>& desc_Datas) {
		desc_Datas = m_Skill_Desc_Datas;
	}
	void	Get_Skill_Depends(vector<SKILL_DEPEND_DATA>& depend_Datas) {
		depend_Datas = m_Skill_Depend_Datas;
	}

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	bool Load_SkillDataResourcesSet_JsonFile(const char* filePath);


	void Create_No_ChargeEffect(_uint skillType, _vector vLook, _vector vPos, const _tchar* pLayerTag, _uint iLevelIndex,
		CBone* pBone, CTransform* pParentTransform, _fmatrix PivotMatrix);

	void	Set_NormalAttackDesc(CAttackEffect::ATTACK_EFFECT_DESC& desc, const _uint& skillType, CSkillEffect* pSkillEffect);
	void	Set_AttackPower(CSkillEffect* pSkillEffect, const _uint& damage);

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

private:
	vector<SKILL_DEPEND_DATA> m_Skill_Depend_Datas;
	vector<CSkill::SKILL_DESC> m_Skill_Desc_Datas;

	string m_skillDepnedDataFilePath = {};
	string m_skillDataResourceFilePath = {};

public:
	static CSkill_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* skillDepnedDataFilePath, const char* skillDataResourceFilePath);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
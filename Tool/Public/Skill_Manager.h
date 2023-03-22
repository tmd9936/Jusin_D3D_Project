#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Effect.h"
#include "Skill.h"

BEGIN(Engine)
END

BEGIN(Client)

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
	HRESULT	Reload_Datas();

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	bool Load_SkillDataResourcesSet_JsonFile(const char* filePath);

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
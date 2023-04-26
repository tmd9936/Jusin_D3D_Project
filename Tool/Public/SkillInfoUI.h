#pragma once
#include "UI.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CSkill_Manager;

class CSkillInfoUI :
	public CUI
{

public:
	typedef struct Skill_Info_UI_Desc
	{
		CTransform*		pParent = { nullptr }; // 기준이 되는 부모

		_uint			m_skillIndex;
		_uint			m_emuletType;

	} SKILLINFO_UI_DESC;

private:
	explicit CSkillInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CSkillInfoUI(const CSkillInfoUI& rhs);
	virtual ~CSkillInfoUI() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint				Tick(_double TimeDelta) override;
	virtual _uint				LateTick(_double TimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void						Set_ParentTransform(CTransform* pTransform) {
		Safe_Release(m_SkillInfoUI_Desc.pParent);
		m_SkillInfoUI_Desc.pParent = pTransform;
		Safe_AddRef(m_SkillInfoUI_Desc.pParent);
	}

public:
	HRESULT						Change_SkillIcon(const _uint& skillIndex);

protected:
	virtual HRESULT				SetUp_ShaderResources() override;

private:
	SKILLINFO_UI_DESC			m_SkillInfoUI_Desc = {};


private:
	_float4x4					m_FinalWorldMatrix; /* 원점기준 (내 월드 * 부모월드) */

private:
	static const _uint			m_skillBaseTextureIndex = { 0 };
	static const _uint			m_skillTextureIndex = { 1 };
	static const _uint			m_skillEmuletBaseTextureIndex = { 2 };
	static const _uint			m_skillEmuletTextureIndex = { 3 };
	static const _uint			m_pipeTextureIndex = { 4 };

public:
	static CSkillInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
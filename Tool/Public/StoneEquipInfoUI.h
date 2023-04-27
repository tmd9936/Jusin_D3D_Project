#pragma once
#include "UI.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CSkill_Manager;
class CStone;

class CStoneEquipInfoUI :
	public CUI
{

public:
	typedef struct Stone_Equip_Info_UI_Desc
	{
		CTransform* pParent = { nullptr }; // 기준이 되는 부모

	} STONEEQUIP_INFO_UI_DESC;

private:
	explicit CStoneEquipInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStoneEquipInfoUI(const CStoneEquipInfoUI& rhs);
	virtual ~CStoneEquipInfoUI() = default;

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
		Safe_Release(m_StoneEquipUI_Desc.pParent);
		m_StoneEquipUI_Desc.pParent = pTransform;
		Safe_AddRef(m_StoneEquipUI_Desc.pParent);
	}

public:
	HRESULT						Init_StoneEquipInfo(vector<STONE_EQUIP_DESC>& desc);
	HRESULT						Init_StoneDatas();

public:
	_bool						Equip(const POINT& mousePT, const CStone::STONE_DESC& stoneDesc);

public:
	virtual _bool				Check_Is_In(const POINT& mousePT) override;

protected:
	virtual HRESULT				SetUp_ShaderResources() override;

private:
	STONEEQUIP_INFO_UI_DESC		m_StoneEquipUI_Desc = {};

private:
	_float4x4					m_FinalWorldMatrix; /* 원점기준 (내 월드 * 부모월드) */

private:
	static const _uint			m_stone00BaseTextureIndex = { 0 };
	static const _uint			m_stone00TextureIndex = { 1 };

	static const _uint			m_stone01BaseTextureIndex = { 2 };
	static const _uint			m_stone01TextureIndex = { 3 };

	static const _uint			m_stone02BaseTextureIndex = { 4 };
	static const _uint			m_stone02TextureIndex = { 5 };

	static const _uint			m_stone10BaseTextureIndex = { 6 };
	static const _uint			m_stone10TextureIndex = { 7 };

	static const _uint			m_stone11BaseTextureIndex = { 8 };
	static const _uint			m_stone11TextureIndex = { 9 };

	static const _uint			m_stone12BaseTextureIndex = { 10 };
	static const _uint			m_stone12TextureIndex = { 11 };

	static const _uint			m_stone20BaseTextureIndex = { 12 };
	static const _uint			m_stone20TextureIndex = { 13 };

	static const _uint			m_stone21BaseTextureIndex = { 14 };
	static const _uint			m_stone21TextureIndex = { 15 };

	static const _uint			m_stone22BaseTextureIndex = { 16 };
	static const _uint			m_stone22TextureIndex = { 17 };

private:
	vector<STONE_EQUIP_DESC>	m_stoneEquipDeses;
	vector<CStone*>				m_stoneDatas;

public:
	static CStoneEquipInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
#pragma once
#include "UI.h"

BEGIN(Client)

class CStone :
	public CUI
{
public:
	enum TYPE
	{
		TYPE_ATK,
		TYPE_HP,
		TYPE_END
	};

public:
	enum STATE
	{
		STATE_IN_INVENTORY_NO_EQUIP,
		STATE_IN_INVENTORY_EQUIP,
		STATE_PICKING,
		STATE_IN_EQUIPINFO,
		STATE_END
	};

public:
	typedef struct Stone_Desc
	{
		CTransform*		pParent = nullptr;
		STATE			m_eCurState = { STATE_END };
		STATE			m_ePreState = { STATE_END };
		TYPE			m_stoneType;
		_uint			value;
		_uint			m_pokemonIconNumber;

		UI_DESC			m_UIDesc;
	} STONE_DESC;

private:
	explicit CStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStone(const CStone& rhs);
	virtual ~CStone() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Init_Stone(const STONE_DESC& stoneDesc);
	HRESULT Init_Text(const STONE_DESC& stoneDesc);
	HRESULT Init_Texture(const STONE_DESC& stoneDesc);
	HRESULT	Init_MaskTexture(const STONE_DESC& stoneDesc);
	HRESULT	Init_PokemonIconTexture(const STONE_DESC& stoneDesc);

	void	Change_Value(const wstring& text);

	void	Set_ParentTransform(CTransform* pTransform) {
		Safe_Release(m_Desc.pParent);
		m_Desc.pParent = pTransform;
		Safe_AddRef(m_Desc.pParent);
	}

private:
	void				Change_State();
	void				State_Tick(const _double& TimeDelta);

private:
	STONE_DESC			m_Desc = {};

public:
	static CStone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;

	virtual void Free() override;
};

END
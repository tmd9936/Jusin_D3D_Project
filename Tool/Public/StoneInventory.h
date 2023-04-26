#pragma once
#include "UI.h"

BEGIN(Client)

class CStone;

class CStoneInventory :
	public CUI
{
private:
	explicit CStoneInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStoneInventory(const CStoneInventory& rhs);
	virtual ~CStoneInventory() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	HRESULT				Init_PokemonData(const _uint& nowMonsterNumber);

private:
	HRESULT				Get_PokemonData();
	HRESULT				Get_NowMonsterData();

protected:
	virtual _bool		Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool		Load_By_JsonFile_Impl(Document& doc);

private:
	vector<CStone*>		m_stones;

private:
	const _uint			m_InventoryNums = 20;

public:
	static CStoneInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
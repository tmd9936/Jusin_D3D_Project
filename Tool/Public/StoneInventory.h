#pragma once
#include "UI.h"

#include "Stone.h"

BEGIN(Client)

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
	virtual _uint		Tick(_double TimeDelta) override;
	virtual _uint		LateTick(_double TimeDelta) override;
	virtual HRESULT		Render() override;

public:
	_bool				Check_Exist_Stone_Is_In(CStone::STONE_DESC& output, _uint& pickingStoneIndex, const POINT& mousePt);

public:
	_bool				Change_StoneIndex(const _uint& originIndex, const POINT& mousePt);

public:
	_bool				Change_StoneState_To_Equip(const _uint& index, const _uint& pokemonNo);
	_bool				Change_StoneState_To_UnEquip(const _uint& index);

protected:
	virtual _bool		Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool		Load_By_JsonFile_Impl(Document& doc);

private:
	vector<CStone*>		m_stones;

private:
	const _uint			m_InventoryNums = 20;
	
	const _float3		m_eqiupScale = { 55.f, 55.f, 1.f };
	const wstring		m_equipTextureProtoTypeTag = { L"Prototype_Component_Texture_Rectangle_Base" };

	const _float3		m_nonEqiupScale = { 45.f, 45.f, 1.f };
	const wstring		m_nonEquipTextureProtoTypeTag = { L"Prototype_Component_Texture_Window_Plane_Corner_Waku_Mini" };

public:
	static CStoneInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
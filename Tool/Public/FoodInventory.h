#pragma once
#include "UI.h"

#include "Food.h"
#include "FoodInfoUI.h"

BEGIN(Client)

class CFoodInventory :
	public CUI
{
private:
	explicit CFoodInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CFoodInventory(const CFoodInventory& rhs);
	virtual ~CFoodInventory() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint		Tick(_double TimeDelta) override;
	virtual _uint		LateTick(_double TimeDelta) override;
	virtual HRESULT		Render() override;

public:
	void				All_Object_RenderOff();

public:
	void				Set_LateTickState(const _bool& lateTickState) {
		m_lateTickState = lateTickState;
	}

protected:
	virtual _bool		Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool		Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT				Init_FoodInfoUIs();

private:
	vector<CFoodInfoUI*>		m_foodInfos;

private:
	_bool						m_lateTickState = { true };
	const _uint					m_maxFoodInfo = { CFood::TYPE_END };

	_uint						m_allItemNums = { 0 };

private:
	const _uint					m_foodTextureBase01Index = { 0 };
	const _uint					m_foodTextureBase02Index = { 1 };
	const _uint					m_foodTextureBase03Index = { 2 };
	const _uint					m_foodTextureBase04Index = { 3 };
	const _uint					m_cauldronHeadTextureIndex = { 4 };

private:
	const _uint					m_itemNumHeadTextIndex = { 0 };
	const _uint					m_itemNumsTextIndex = { 1 };

public:
	static CFoodInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
	virtual void Free() override;
};

END
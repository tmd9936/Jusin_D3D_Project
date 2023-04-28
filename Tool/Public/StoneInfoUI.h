#pragma once
#include "UI.h"

#include "Stone.h"

BEGIN(Client)

class CStoneInfoUI :
	public CUI
{
private:
	explicit CStoneInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStoneInfoUI(const CStoneInfoUI& rhs);
	virtual ~CStoneInfoUI() = default;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint	Tick(_double TimeDelta) override;
	virtual _uint	LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT			Change_StoneInfo(const CStone::STONE_DESC& desc);

private:
	_bool			m_bShowStoneInfo = { true };

private:
	const _uint		m_StoneTextureIndex = { 0 };
	const _uint		m_InfoBaseBoldTextureIndex = { 1 };
	const _uint		m_InfoBaseTextureIndex = { 2 };
	const _uint		m_NameBaseTextureIndex = { 3 };
	const _uint		m_ValueBaseTextureIndex = { 4 };

	const _uint		m_StoneValueText = { 0 };
	const _uint		m_StoneNameText = { 1 };
	const _uint		m_StoneTypeText = { 2 };
	const _uint		m_StoneValueText2 = { 3 };


public:
	static CStoneInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END
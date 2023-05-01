#pragma once

#include "UI.h"
#include "Food.h"

BEGIN(Client)

class CFoodInfoUI :
	public CUI
{

private:
	explicit CFoodInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CFoodInfoUI(const CFoodInfoUI& rhs);
	virtual ~CFoodInfoUI() = default;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint	Tick(_double TimeDelta) override;
	virtual _uint	LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT			Set_FoodInfo(CFood::TYPE eType, const _uint& foodNum);
	HRESULT			Set_FoodInfo(const _uint& foodNum);

private:
	const _uint		m_FoodTextureIndex = { 0 };

	const _uint		m_XMartTextIndex = { 0 };
	const _uint		m_FoodNumTextIndex = { 1 };

public:
	static CFoodInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END
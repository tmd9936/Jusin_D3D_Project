#pragma once
#include "UI.h"

#include "Stone.h"
#include "Food.h"

#include <queue>

BEGIN(Client)

class CStage_Manager;

class CGetItemShowUI :
	public CUI
{
private:
	explicit CGetItemShowUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CGetItemShowUI(const CGetItemShowUI& rhs);
	virtual ~CGetItemShowUI() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint				Tick(_double TimeDelta) override;
	virtual _uint				LateTick(_double TimeDelta) override;
	virtual HRESULT				Render() override;

private:
	HRESULT						Add_Stone(const CStone::STONE_DESC& stoneDesc, _fmatrix vStartWorldMatrix);
	HRESULT						Add_Food(const CFood::FOOD_DESC& foodDesc, _fmatrix vStartWorldMatrix);

private:
	_uint						GetStonesTick(const _double& TimeDelta);
	_uint						GetFoodsTick(const _double& TimeDelta);

private:
	_uint						m_StoneNum = { 0 };
	_uint						m_FoodNum = { 0 };
	
private:
	const _uint					m_StoneIconBaseTextureIndex = { 0 };
	const _uint					m_StoneIconTextureIndex = { 1 };
	const _uint					m_FoodIconBaseTextureIndex = { 2 };
	const _uint					m_FoodIconTextureIndex = { 3 };

	const _uint					m_StoneNumTextIndex = { 0 };
	const _uint					m_FoodNumTextIndex = { 1 };

private:
	vector<CStone::STONE_DESC>	m_stones;
	vector<_uint>				m_foods;

	list<CStone*>				m_GetStones;
	list<CFood*>				m_GetFoods;

private:


	const _uint					m_canMaxGetStoneNum = { 3 };

public:
	static CGetItemShowUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END
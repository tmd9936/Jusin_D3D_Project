#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "UI.h"

BEGIN(Engine)

END

BEGIN(Client)

class CMonster;
class CStageSupportMonster;
class CPlayer;
class CFood;
class CPokemonInfoUI;

class CFeeding_Manager final : public CUI
{
public:
	enum FEEDING_MANAGER_STATE {
		MANAGER_IDLE,
		MANAGER_FOOD_PICKING,
		MANAGER_EVOLUTION,
		MANAGER_END
	};

private:
	CFeeding_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFeeding_Manager(const CFeeding_Manager& rhs);
	virtual ~CFeeding_Manager() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT					Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint					Tick(_double TimeDelta) override;
	virtual _uint					LateTick(_double TimeDelta) override;
	virtual HRESULT					Render() override;

public:
	HRESULT							Init_NowMonsters();

private:
	HRESULT							Init_PickingFood();
	HRESULT							Init_PokemonInfoUI();

private:
	void							State_Tick(const _double& TimeDelta);
	void							Change_State();

private:
	void							Picking();

private:
	void							Inventory_Food_Picking_Tick();

private:
	HRESULT							SetUp_ShaderResources();

private:
	FEEDING_MANAGER_STATE			m_ePreState = { MANAGER_END };
	FEEDING_MANAGER_STATE			m_eCurState = { MANAGER_IDLE };

private:
	vector<CMonster*>				m_pNowMonsters = { nullptr };
	vector<CPokemonInfoUI*>			m_pPokemonInfoUIs = { nullptr };

	CFood*							m_pPickingFood = { nullptr };

public:
	static CFeeding_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
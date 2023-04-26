#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "UI.h"

BEGIN(Engine)

END

BEGIN(Client)

class CStageSupportMonster; 
class CPlayer;
class CPokemonSkillStoneUI;
class CSkillInfoUI;
class CStoneEquipInfoUI;
class CPokemonInfoUI;
class CStoneInventory;
class CStone;

class CPokemonState_Manager final : public CUI
{
public:
	typedef struct PokemonState_Manager_Desc
	{
		_uint m_nowMonsterNumber;
	} POKEMONSTATE_MANAGER_DESC;

public:
	enum POKEMONSTATE_MANAGER_STATE {
		MANAGER_IDLE,
		MANAGER_PIKING_STONE,
		MANAGER_END
	};

private:
	CPokemonState_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPokemonState_Manager(const CPokemonState_Manager& rhs);
	virtual ~CPokemonState_Manager() = default;

public:
	virtual HRESULT					Initialize_Prototype() override; 
	virtual HRESULT					Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; 
	virtual HRESULT					Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint					Tick(_double TimeDelta) override;
	virtual _uint					LateTick(_double TimeDelta) override;
	virtual HRESULT					Render() override;

public:
	HRESULT							Init_NowMonster(const _uint& nowMonsterNumber);

private:
	HRESULT							Init_PickingStone();
	HRESULT							Init_PokemonInfoUI();
	HRESULT							Init_PokemonSkillStoneUI();
	HRESULT							Init_PokemonSkillInfoUI();
	HRESULT							Init_StoneEquipInfoUI();
	HRESULT							Init_StoneInventory();

private:
	void							State_Tick(const _double& TimeDelta);
	void							Change_State();
	void							Picking();

private:
	HRESULT							SetUp_ShaderResources(); 

private:
	POKEMONSTATE_MANAGER_DESC		m_Desc = {};
	POKEMONSTATE_MANAGER_STATE		m_ePreState = { MANAGER_END };
	POKEMONSTATE_MANAGER_STATE		m_eCurState = { MANAGER_IDLE };

private:
	CStone*							m_pPickingInfoStone = { nullptr };

private:
	CMonster*						m_pNowMonster			= { nullptr };
	CPokemonSkillStoneUI*			m_pPokemonSkillStoneUI	= { nullptr };
	CSkillInfoUI*					m_pSkillInfoUI01		= { nullptr };	
	CSkillInfoUI*					m_pSkillInfoUI02		= { nullptr };
	CStoneEquipInfoUI*				m_pStoneEquipInfoUI		= { nullptr };
	CStoneInventory*				m_pStoneInventory		= { nullptr };
	CPokemonInfoUI*					m_pPokemonInfoUI		= { nullptr };

public:
	static CPokemonState_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
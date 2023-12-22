#pragma once
#include "UI.h"

BEGIN(Client)

class CPokemonPowerInfoUI :
	public CUI
{
private:
	explicit CPokemonPowerInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPokemonPowerInfoUI(const CPokemonPowerInfoUI& rhs);
	virtual ~CPokemonPowerInfoUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

private:
	HRESULT Init_PokemonPower();
	HRESULT Get_PokemonPower(_uint& outPower, const _uint& pokemonIndex);

public:
	static CPokemonPowerInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
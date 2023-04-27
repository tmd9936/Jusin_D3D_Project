#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CGoToMonStateButton;

class CLevel_PokemonState final : public CLevel
{
private:
	CLevel_PokemonState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_PokemonState() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_double TimeDelta) override;

public:
	static void Set_PokemonNumber(const _uint& number);

	static _uint Get_PokemonNumber() {
		return m_PokemonNumber;
	}

private:
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Manager(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

private:
	MYFLOAT4			m_PokemonStateLevelBackColor = { 0.968f, 0.741f, 0.125f, 1.f };

private:
	static	_uint		m_PokemonNumber;

public:
	static CLevel_PokemonState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
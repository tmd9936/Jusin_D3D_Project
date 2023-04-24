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
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

private:
	HRESULT Init_PokemonPower();
	HRESULT Get_PokemonPower(_uint& outPower, const _uint& pokemonIndex);

public:
	static CPokemonPowerInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
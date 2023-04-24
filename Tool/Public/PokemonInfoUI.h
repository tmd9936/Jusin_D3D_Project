#pragma once
#include "UI.h"

BEGIN(Client)

class CPokemonInfoUI :
	public CUI
{
public:
	typedef struct PokemonInfo_UI_Desc
	{
		_uint		m_pokemonNumber;
	} POKEMONINFO_UI_DESC;

private:
	explicit CPokemonInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPokemonInfoUI(const CPokemonInfoUI& rhs);
	virtual ~CPokemonInfoUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);


private:
	HRESULT Init_PokemonPower();
	HRESULT Get_PokemonPower(_uint& outPower, const _uint& pokemonIndex);

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	POKEMONINFO_UI_DESC		m_PokemonInfo_Desc = {};

public:
	static CPokemonInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
#pragma once
#include "UI.h"

BEGIN(Client)

class CPokemonData;

class CPokemonInfoUI :
	public CUI
{

public:
	typedef struct PokemonInfo_UI_Desc
	{
		_uint			m_nowMonsterNumber;
		_uint			m_pokemonNo;
		wstring			m_pokemonName;
		wstring			m_pokemonType;
		_uint  			m_pokemonLevel;
		_int			m_pokemonHP;
		_int			m_pokemonATK;
		_int			m_exp;
		_int			m_maxExp;

	} POKEMONINFO_UI_DESC;

private:
	explicit CPokemonInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPokemonInfoUI(const CPokemonInfoUI& rhs);
	virtual ~CPokemonInfoUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;


public:
	HRESULT Init_PokemonData(const _uint& nowMonsterNumber);

public:
	const _uint	Get_PokemonNo() const {
		return m_PokemonInfo_Desc.m_pokemonNo;
	}

public:
	void	Add_ATK(const _int& value);
	void	Add_HP(const _int& value);

public:
	const _int Get_PokemonHP() const {
		return m_PokemonInfo_Desc.m_pokemonHP;
	}

	const _int Get_PokemonATK() const {
		return m_PokemonInfo_Desc.m_pokemonATK;
	}

	const _int Get_Exp() const {
		return m_PokemonInfo_Desc.m_exp;
	}

	const _int Get_Level() const {
		return m_PokemonInfo_Desc.m_pokemonLevel;
	}

public:
	void		StartDisolve() {
		m_UIDesc.m_ShaderPass = 6;
		m_bDisolve = true;
		m_vMtrlDif = 1.f;
		m_fDissolveAmount = 0.f;
		m_fThreshold = 0.f;
	}

public:
	_bool					Check_CanEvolution();
	void					Add_Exp(const _int& exp);
	void					Set_Exp(const _int& exp) {
		m_PokemonInfo_Desc.m_exp = exp;
	}

public:
	void					All_Render_Off();
	void					All_Render_ON();

protected:
	virtual HRESULT			SetUp_ShaderResources();

private:
	HRESULT					Get_PokemonData();
	HRESULT					Get_NowMonsterData();

	HRESULT					Init_DisolveMask();

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	CTexture*				m_pDisolveTexture = { nullptr };

private:
	POKEMONINFO_UI_DESC		m_PokemonInfo_Desc = {};
	_float					m_vMtrlDif = { 0.f };

	_bool					m_bDisolve = { false };

	_float					m_fDissolveAmount = { 0.f };
	_float					m_fThreshold = { 0.f };

private:
	static const _uint		m_pokemonNameTextIndex = { 0 };
	static const _uint		m_pokemonTypeTextIndex = { 1 };
	static const _uint		m_pokemonLvTextIndex = { 2 };
	static const _uint		m_pokemonLevelTextIndex = { 3 };
	static const _uint		m_pokemonHPTextIndex = { 4 };
	static const _uint		m_pokemonHPValueTextIndex = { 5 };
	static const _uint		m_pokemonATKTextIndex = { 6 };
	static const _uint		m_pokemonATKValueTextIndex = { 7 };

	static const _uint		m_pokemonExpValueTextIndex = { 8 };
	static const _uint		m_pokemonDivTextIndex = { 9 };
	static const _uint		m_pokemonMaxExpValueTextIndex = { 10 };

private:
	static const _uint		m_pokemonIconTextureIndex = { 0 };
	static const _uint		m_pokemonNameBaseTextureIndex = { 1 };
	static const _uint		m_pokemonLevelBaseTextureIndex = { 2 };
	static const _uint		m_pokemonHPBaseTextureIndex = { 3 };
	static const _uint		m_pokemonHPIconTextureIndex = { 4 };
	static const _uint		m_pokemonATKBaseTextureIndex = { 5 };
	static const _uint		m_pokemonATKIconTextureIndex = { 6 };

public:
	static CPokemonInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
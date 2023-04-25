#pragma once
#include "UI.h"

BEGIN(Client)

class CPokemonData;

class CPokemonSkillStoneUI :
	public CUI
{

public:
	typedef struct PokemonSkillStone_Desc
	{
		_uint			m_nowMonsterNumber;
		_uint			m_pokemonNo;
		wstring			m_pokemonName;
		wstring			m_pokemonType;
		_uint  			m_pokemonLevel;
		_int			m_pokemonHP;
		_int			m_pokemonATK;
		_int			m_curExp;
	} POKEMONSKILLSTONE_UI_DESC;

private:
	explicit CPokemonSkillStoneUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPokemonSkillStoneUI(const CPokemonSkillStoneUI& rhs);
	virtual ~CPokemonSkillStoneUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	HRESULT Init_PokemonData(const _uint& nowMonsterNumber);

private:

	HRESULT Get_PokemonData();
	HRESULT Get_NowMonsterData();

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	POKEMONSKILLSTONE_UI_DESC	m_PokemonSkillStone_Desc = {};

private:
	static const _uint			m_baseTextureIndex = { 0 };
	static const _uint			m_skillBaseTextureIndex = { 1 };
	static const _uint			m_stoneBaseTextureIndex = { 2 };
	static const _uint			m_skill1BaseTextureIndex = { 3 };

	static const _uint			m_skill1TextureIndex = { 4 };
	static const _uint			m_skill1EmuletBaseTextureIndex = { 5 };
	static const _uint			m_skill1EmuletTextureIndex = { 6 };

	static const _uint			m_skill2TextureIndex = { 7 };
	static const _uint			m_skill2EmuletBaseTextureIndex = { 8 };
	static const _uint			m_skill2EmuletTextureIndex = { 9 };

public:
	static CPokemonSkillStoneUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
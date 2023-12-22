#pragma once
#include "UI.h"

BEGIN(Client)

class CPokemonData;
class CSkillInfoUI;
class CStoneEquipInfoUI;

class CPokemonSkillStoneUI :
	public CUI
{
public:
	typedef struct PokemonSkillStone_Desc
	{
		_uint	m_nowMonsterNumber;

	} POKEMONSKILLSTONE_UI_DESC;

private:
	explicit CPokemonSkillStoneUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPokemonSkillStoneUI(const CPokemonSkillStoneUI& rhs);
	virtual ~CPokemonSkillStoneUI() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	HRESULT				Init_PokemonData(const _uint& nowMonsterNumber);

private:
	HRESULT				Get_PokemonData();
	HRESULT				Get_NowMonsterData();

protected:
	virtual _bool		Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool		Load_By_JsonFile_Impl(Document& doc);

private:
	POKEMONSKILLSTONE_UI_DESC	m_PokemonSkillStone_Desc = {};

private:
	static const _uint			m_skillBaseTextureIndex = { 0 };
	static const _uint			m_stoneBaseTextureIndex = { 1 };

	static const _uint			m_bingo1TextureIndex = { 0 };
	static const _uint			m_bingo2TextureIndex = { 1 };
	static const _uint			m_bingo3TextureIndex = { 1 };


private:
	vector<CSkillInfoUI*>			m_skillInfoUIs;
	CStoneEquipInfoUI*				m_pStoneEquipInfoUI = { nullptr };

public:
	static CPokemonSkillStoneUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
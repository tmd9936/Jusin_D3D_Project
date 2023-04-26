#pragma once
#include "UI.h"

BEGIN(Client)

class CPokemonData;
class CSkillInfoUI;

class CPokemonSkillStoneUI :
	public CUI
{
public:
	typedef struct PokemonSkillStone_Desc
	{
		_uint						m_nowMonsterNumber;

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


	//static const _uint			m_skill1BaseTextureIndex = { 3 };
	//static const _uint			m_skill1TextureIndex = { 4 };
	//static const _uint			m_skill1EmuletBaseTextureIndex = { 5 };
	//static const _uint			m_skill1EmuletTextureIndex = { 6 };

	//static const _uint			m_skill2BaseTextureIndex = { 7 };
	//static const _uint			m_skill2TextureIndex = { 8 };
	//static const _uint			m_skill2EmuletBaseTextureIndex = { 9 };
	//static const _uint			m_skill2EmuletTextureIndex = { 10 };

	//static const _uint			m_StoneBase1TextureIndex = { 11 };
	//static const _uint			m_StoneBase2TextureIndex = { 12 };
	//static const _uint			m_StoneBase3TextureIndex = { 13 };

	//static const _uint			m_StoneBase4TextureIndex = { 14 };
	//static const _uint			m_StoneBase5TextureIndex = { 15 };
	//static const _uint			m_StoneBase6TextureIndex = { 16 };

	//static const _uint			m_StoneBase7TextureIndex = { 17 };
	//static const _uint			m_StoneBase8TextureIndex = { 18 };
	//static const _uint			m_StoneBase9TextureIndex = { 19 };

	//static const _uint			m_Stone1TextureIndex = { 20 };
	//static const _uint			m_Stone2TextureIndex = { 21 };
	//static const _uint			m_Stone3TextureIndex = { 22 };

	//static const _uint			m_Stone4TextureIndex = { 23 };
	//static const _uint			m_Stone5TextureIndex = { 24 };
	//static const _uint			m_Stone6TextureIndex = { 25 };

	//static const _uint			m_Stone7TextureIndex = { 25 };
	//static const _uint			m_Stone8TextureIndex = { 26 };
	//static const _uint			m_Stone9TextureIndex = { 27 };

//private:
	// 스킬 창 1, 2 백터로 만들고 초기화시 포켓몬 정보에 따라 가져오고 그리기
	// 스톤 정보 UI (칸 1개당 1객체) 만들어서 현재 UI에 나타낼 포켓몬의 정보 받아와서 Initialize해주고 그리기

private:
	vector<CSkillInfoUI*>			m_skillInfoUIs;

public:
	static CPokemonSkillStoneUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual void Free() override;
};

END
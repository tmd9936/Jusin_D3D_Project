#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPokemonData final : public CGameObject
{
public:
	typedef struct PokemonData_Desc
	{
		_uint id;
		wstring name;
		wstring type;

	} POKEMONDATA_DESC;

private:
	explicit CPokemonData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPokemonData(const CPokemonData& rhs);
	virtual ~CPokemonData() = default;

public:
	virtual HRESULT Initialize_Prototype(const char* pokemonnDataFilePath); 
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; 
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	POKEMONDATA_DESC Get_PokemonDesc(const _uint& index) const {
		if (m_PokemonData_Desc.size() <= index)
			return POKEMONDATA_DESC();

		return m_PokemonData_Desc[index];
	}

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

private:
	vector<POKEMONDATA_DESC>					m_PokemonData_Desc;

public:
	static CPokemonData* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pokemonnDataFilePath);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
#pragma once

#include "Button.h"

BEGIN(Client)

class CPokemonData;

class CGoToMonStateButton :
    public CButton
{
public:
    typedef struct MonStateButton_Desc
    {
        _int    m_monsterNumber;
    }MONSTATEBUTTON_DESC;
protected:
    explicit CGoToMonStateButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CGoToMonStateButton(const CGoToMonStateButton& rhs);
    virtual ~CGoToMonStateButton() = default;

protected:
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

public:
    HRESULT Change_MosnterNumber(const _uint& pokemonIndex, const _uint& pokemonIconIndex);

protected:
    virtual _uint On_Idle() override;
    virtual _uint On_Press() override;
    virtual _uint On_Select() override;
    virtual _uint On_Release() override;

protected:
    virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
    virtual _bool			Load_By_JsonFile_Impl(Document& doc);

protected:
    HRESULT Get_PokemonNumber(_uint& out, const _uint& pokemonIndex);
    HRESULT Init_Monster_Info(_uint pokemonIconIndex = 0);

private:
    MONSTATEBUTTON_DESC  m_MonStateButtonDesc = {};

public:
    static CGoToMonStateButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};

END
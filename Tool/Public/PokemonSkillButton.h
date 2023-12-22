#pragma once

#include "Button.h"

BEGIN(Client)

class CMonster;

class CPokemonSkillButton :
    public CButton
{
protected:
    explicit CPokemonSkillButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CPokemonSkillButton(const CPokemonSkillButton& rhs);
    virtual ~CPokemonSkillButton() = default;

public:
    virtual _uint Tick(_double TimeDelta) override;

public:
    void    Set_ParentMonster(CMonster* pMonster) {
        m_pMonster = pMonster;
        Safe_AddRef(m_pMonster);
    }

    void    Set_SkillNumber(_uint number) {
        m_SkillNumber = number;
    }

protected:
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

protected:
    virtual _uint On_Idle() override;
    virtual _uint On_Press() override;
    virtual _uint On_Select() override;
    virtual _uint On_Release() override;

private:
    CMonster*   m_pMonster = { nullptr };
    _uint       m_SkillNumber = { 0 };

public:
    /* Prototype */
    /* 원형 객체를 생성한다. */
    static CPokemonSkillButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    /* 사본 객체를 생성한다. */
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};

END
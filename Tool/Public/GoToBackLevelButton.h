#pragma once

#include "Button.h"

class CGoToBackLevelButton :
    public CButton
{
protected:
    CGoToBackLevelButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGoToBackLevelButton(const CGoToBackLevelButton& rhs);
    virtual ~CGoToBackLevelButton() = default;

protected:
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

protected:
    virtual _uint On_Idle() override;
    virtual _uint On_Press() override;
    virtual _uint On_Select() override;
    virtual _uint On_Release() override;

private:
    static LEVEL    m_BackLevel;

public:
    static void Set_BackLevel(LEVEL backLevel) {
        m_BackLevel = backLevel;
    }

public:
    static CGoToBackLevelButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};


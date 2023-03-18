#pragma once

#include "Button.h"

class CGoToWorldMapButton :
    public CButton
{
protected:
    CGoToWorldMapButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGoToWorldMapButton(const CGoToWorldMapButton& rhs);
    virtual ~CGoToWorldMapButton() = default;

protected:
    virtual _uint On_Idle() override;
    virtual _uint On_Press() override;
    virtual _uint On_Select() override;
    virtual _uint On_Release() override;

public:
    /* Prototype */
    /* 원형 객체를 생성한다. */
    static CGoToWorldMapButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    /* 사본 객체를 생성한다. */
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};


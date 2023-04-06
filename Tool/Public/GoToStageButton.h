#pragma once

#include "Button.h"

BEGIN(Client)

class CGoToStageButton :
    public CButton
{
protected:
    CGoToStageButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGoToStageButton(const CGoToStageButton& rhs);
    virtual ~CGoToStageButton() = default;

protected:
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
    virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

protected:
    virtual _uint On_Idle() override;
    virtual _uint On_Press() override;
    virtual _uint On_Select() override;
    virtual _uint On_Release() override;

public:
    /* Prototype */
    /* 원형 객체를 생성한다. */
    static CGoToStageButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    /* 사본 객체를 생성한다. */
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};

END
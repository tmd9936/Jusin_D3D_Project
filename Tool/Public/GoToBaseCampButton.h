#pragma once

#include "Button.h"

class CGoToBaseCampButton :
    public CButton
{
protected:
    explicit  CGoToBaseCampButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CGoToBaseCampButton(const CGoToBaseCampButton& rhs);
    virtual ~CGoToBaseCampButton() = default;

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
    /* ���� ��ü�� �����Ѵ�. */
    static CGoToBaseCampButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    /* �纻 ��ü�� �����Ѵ�. */
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};


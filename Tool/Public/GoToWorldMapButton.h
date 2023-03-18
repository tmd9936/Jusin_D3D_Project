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
    /* ���� ��ü�� �����Ѵ�. */
    static CGoToWorldMapButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    /* �纻 ��ü�� �����Ѵ�. */
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
    virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
    virtual void Free() override;
};


#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject * pOwner);
	explicit CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT Render() override;

private:
	ID3D11Buffer*	 m_pVBInstance = { nullptr }; // 인덱스는 어차피 같은 버퍼 여러개를 출력하기 때문에 선언x
	_uint			m_iNumInstances = { 0 };
	_uint			m_iInstanceStride = { 0 };


public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance = 1);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
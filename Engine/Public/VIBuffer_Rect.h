#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject* pOwner);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	const VTXTEX*			Get_VertexBuffer() { 
		return m_pVtxTex; }
	const FACEINDICES16*	Get_IndexBuffer() { 
		return m_pIndex; }

private:
	VTXTEX*			m_pVtxTex = { nullptr };
	FACEINDICES16*	m_pIndex = { nullptr };

public:
	static CVIBuffer_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
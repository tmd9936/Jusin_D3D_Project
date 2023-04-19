#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
private:
	explicit CVIBuffer_Trail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject * pOwner);
	explicit CVIBuffer_Trail(const CVIBuffer_Trail& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual	_uint		Tick(const _double& TimeDelta, _fmatrix parentMatrix);

public:
	const VTXTEX* Get_VertexBuffer() {
		return m_pVtxTex;
 }
const FACEINDICES16* Get_IndexBuffer() {
	return m_pIndex;
}

private:
	VTXTEX*			m_pVtxTex = { nullptr };
	list<VTXTEX>	m_worldVtxTex;

	FACEINDICES16*	m_pIndex = { nullptr };

	_double			m_fAccTime = { 0.0 };

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
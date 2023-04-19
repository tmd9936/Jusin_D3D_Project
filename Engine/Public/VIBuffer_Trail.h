#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
public:
	typedef struct VIBuffer_Trail_Desc
	{
		_uint m_iNumVertices;
	} VIBUFFER_TRAIL_DESC;

private:
	explicit CVIBuffer_Trail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject * pOwner);
	explicit CVIBuffer_Trail(const CVIBuffer_Trail& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual	_uint		Tick(const _double& TimeDelta, _fmatrix parentMatrix);

private:		
	VIBUFFER_TRAIL_DESC		m_Desc = {};
	ID3D11Buffer*			m_pVBInstance = { nullptr };

	list<VTXTEX>			m_worldVtxTex;

	_double					m_fAccTime = { 0.0 };

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
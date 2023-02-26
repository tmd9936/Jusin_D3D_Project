#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_FlatTerrain final : public CVIBuffer
{
public:
	typedef struct tagVIBuffer_FlatTerrain_DESC
	{
		_uint				m_iNumVerticesX;
		_uint				m_iNumVerticesZ;

	}VIBUFFER_FLAT_TERRAIN_DESC;
private:
	CVIBuffer_FlatTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject* pOwner);
	CVIBuffer_FlatTerrain(const CVIBuffer_FlatTerrain& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer_FlatTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize( void* pArg) override;

private:
	VIBUFFER_FLAT_TERRAIN_DESC	m_Vibuffer_Flat_Terrain_Desc = { };
	_float3* m_pPos = { nullptr };

public:
	static CVIBuffer_FlatTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
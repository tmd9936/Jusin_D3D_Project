#pragma once

#include "Component.h"

/* 모든 형태의 정점, 인덱스 버퍼의 부모가 되는 클래스다. */
/* 정점, 인덱스를 생성하는 행위, 그리는 행위등등이 흡사한경우가 많다. */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_VIBUFFER;

protected:
	explicit CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CVIBuffer(const CVIBuffer& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer() = default;

public:
	virtual const FamilyId	Get_FamilyId() const { return familyId; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT Render();

public:
	const _uint	Get_NumVertices() const {
		return m_iNumVertices;
	}

	const _uint	Get_Stride() const {
		return m_iStride;
	}

	const _uint	Get_NumBuffers() const {
		return m_iNumBuffers;
	}

	const _uint Get_IndexSizePrimitive() const {
		return m_iIndexSizePrimitive;
	}

	const _uint Get_NumPrimitives() const {
		return m_iNumPrimitives;
	}

	const _uint Get_NumIndicesPrimitive() const {
		return m_iNumIndicesPrimitive;
	}

	const DXGI_FORMAT Get_Format() const {
		return m_eFormat;
	}

	const D3D11_PRIMITIVE_TOPOLOGY Get_Topology() const {
		return m_eTopology;
	}

protected:
	ID3D11Buffer*					m_pVB = { nullptr };
	_uint							m_iNumVertices = { 0 };
	_uint							m_iStride = { 0 }; /* 정점 하나의 Byte크기 */
	_uint							m_iNumBuffers = { 0 };

protected:
	ID3D11Buffer*					m_pIB = { nullptr };
	_uint							m_iIndexSizePrimitive = { 0 };
	_uint							m_iNumPrimitives = { 0 };
	_uint							m_iNumIndicesPrimitive = { 0 };
	DXGI_FORMAT						m_eFormat = { };
	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology = { };

protected:
	D3D11_BUFFER_DESC				m_BufferDesc = { };
	D3D11_SUBRESOURCE_DATA			m_SubResourceData = { };

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END
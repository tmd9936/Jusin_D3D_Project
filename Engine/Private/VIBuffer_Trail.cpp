#include "VIBuffer_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
{

}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
{

}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 36;
	m_iIndexSizePrimitive = sizeof(FACEINDICES32);
	m_iNumPrimitives = m_iNumVertices - 2;
	m_iNumIndicesPrimitive = 3;
	m_iNumBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	/* D3D11_USAGE_DEFAULT : CPU에 의한 값 갱신이 불가능하다 */
	/* D3D11_USAGE_DYNAMIC : CPU(C++코드, Lock, unLock)에 의한 값 갱신이 가능하다. */
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;


	VTXTEX* m_pVtxTex = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVtxTex, m_iStride * m_iNumVertices);

	_float interval = 1.f / m_iNumVertices;
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		if (i % 2)
		{
			m_pVtxTex[i].vPosition = _float3(interval * i, 0.1f, 0.f);
			m_pVtxTex[i].vTexUV = _float2(interval * i, 0.f);
		}
		else
		{
			m_pVtxTex[i].vPosition = _float3(interval * i, -0.1f, 0.f);
			m_pVtxTex[i].vTexUV = _float2(interval * i, 1.f);
		}
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = m_pVtxTex;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVtxTex);

#pragma endregion

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexSizePrimitive * m_iNumPrimitives;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	FACEINDICES32* m_pIndices = new FACEINDICES32[m_iNumPrimitives];

	/* m_iNumVertices(정점 개수)를 짝수로 안해주면 인덱스 버퍼 만들어주고 해제시에 
	초과되는 메모리를 건드린다고 오류 메세지 뜸
	삼각형 2개로 이루어진 사각형 버퍼를 이어서 만들어주는 것이여서 무조건 짝수 맞춰줘야함*/
	for (_uint i = 0; i < m_iNumPrimitives; i += 2)
	{
		m_pIndices[i]._0 = i;
		m_pIndices[i]._1 = i + 2;
		m_pIndices[i]._2 = i + 3;

		m_pIndices[i + 1]._0 = i;
		m_pIndices[i + 1]._1 = i + 2;
		m_pIndices[i + 1]._2 = i + 1;
	}

	m_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pIndices);
#pragma endregion
	return S_OK;
}

_uint CVIBuffer_Trail::Tick(const _double& TimeDelta, _fmatrix parentMatrix)
{
	while (m_worldVtxTex.size() >= m_iNumVertices)
	{
		m_worldVtxTex.pop_front();
	}

	if (0.02 < m_fAccTime)
	{
		_float3 vPos[2];
		vPos[0] = _float3(0.f, 0.f, 0.5f);
		vPos[1] = _float3(0.f, 0.f, -0.5f);

		for (size_t i = 0; i < 2; i++)
		{
			XMStoreFloat3(&vPos[i],
				XMVector3TransformCoord(XMLoadFloat3(&vPos[i]), parentMatrix));

			VTXTEX data{};
			data.vPosition = vPos[i];
			m_worldVtxTex.push_back(data);
		}
		// ((VTXTEX*)SubResource.pData)[i].vPosition = m_pWorldVtxTex[i].vPosition;

		m_fAccTime = 0.0;

		D3D11_MAPPED_SUBRESOURCE		SubResource;
		m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		_uint index = 0;
		for (auto& data : m_worldVtxTex)
		{ 
			((VTXTEX*)SubResource.pData)[index++].vPosition = data.vPosition;
		}

		m_pContext->Unmap(m_pVB, 0);
	}

	m_fAccTime += TimeDelta;

	return _uint();
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}

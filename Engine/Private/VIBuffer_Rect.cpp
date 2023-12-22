#include "..\Public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
{

}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
	, m_pVtxTex(rhs.m_pVtxTex)
	, m_pIndex(rhs.m_pIndex)
{

}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iIndexSizePrimitive = sizeof(FACEINDICES16);
	m_iNumPrimitives = 2;
	m_iNumIndicesPrimitive = 3;
	m_iNumBuffers = 1;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	/* D3D11_USAGE_DEFAULT : CPU에 의한 값 갱신이 불가능하다 */
	/* D3D11_USAGE_DYNAMIC : CPU(C++코드, Lock, unLock)에 의한 값 갱신이 가능하다. */
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	m_pVtxTex = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVtxTex, m_iStride * m_iNumVertices);

	/* 로컬스페이스 상의 위치 정보를 채운다. */
	m_pVtxTex[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	m_pVtxTex[0].vTexUV = _float2(0.0f, 0.f);

	m_pVtxTex[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	m_pVtxTex[1].vTexUV = _float2(1.0f, 0.f);

	m_pVtxTex[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	m_pVtxTex[2].vTexUV = _float2(1.0f, 1.f);

	m_pVtxTex[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	m_pVtxTex[3].vTexUV = _float2(0.0f, 1.f);

	m_SubResourceData.pSysMem = m_pVtxTex;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

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

	m_pIndex = new FACEINDICES16[m_iNumPrimitives];

	m_pIndex[0]._0 = 0;
	m_pIndex[0]._1 = 1;
	m_pIndex[0]._2 = 2;

	m_pIndex[1]._0 = 0;
	m_pIndex[1]._1 = 2;
	m_pIndex[1]._2 = 3;

	m_SubResourceData.pSysMem = m_pIndex;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	if (!m_bClone)
	{
		Safe_Delete_Array(m_pIndex);
		Safe_Delete_Array(m_pVtxTex);
	}

	__super::Free();

}

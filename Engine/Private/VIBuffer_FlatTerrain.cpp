#include "VIBuffer_FlatTerrain.h"

CVIBuffer_FlatTerrain::CVIBuffer_FlatTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
{

}

CVIBuffer_FlatTerrain::CVIBuffer_FlatTerrain(const CVIBuffer_FlatTerrain& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
{

}

HRESULT CVIBuffer_FlatTerrain::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_FlatTerrain::Initialize(void* pArg)
{
	if (nullptr == pArg)
	{
		m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX = 128;
		m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ = 128;
	}
	else
	{
		memcpy(&m_Vibuffer_Flat_Terrain_Desc, pArg, sizeof VIBUFFER_FLAT_TERRAIN_DESC);
	}

	_ulong* pPixel = new _ulong[m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX * m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ];
	ZeroMemory(pPixel, sizeof(_ulong) * m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX * m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ);

	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX * m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ;
	m_iIndexSizePrimitive = sizeof(FACEINDICES32);
	m_iNumPrimitives = (m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX - 1) * (m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPrimitive = 3;
	m_iNumBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pPos = new _float3[m_iNumVertices];

#pragma region VERTEX_BUFFER
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	for (_uint i = 0; i < m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(float(j), 0.f, float(i));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX - 1.0f), i / (m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ - 1.0f));
			
			m_pPos[iIndex] = _float3(float(j), 0.f, float(i));
		}
	}

	m_SubResourceData.pSysMem = pVertices;

	Safe_Delete_Array(pPixel);
#pragma endregion

#pragma region INDEX_BUFFER
	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];

	_ulong		dwNumFaces = 0;

	_vector		vSourDir, vDestDir, vNormal;

	for (_uint i = 0; i < m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX,
				iIndex + m_Vibuffer_Flat_Terrain_Desc.m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFaces]._0 = iIndices[0];
			pIndices[dwNumFaces]._1 = iIndices[1];
			pIndices[dwNumFaces]._2 = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition) -
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._0].vPosition);

			vDestDir = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) -
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);

			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._0].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);

			++dwNumFaces;

			pIndices[dwNumFaces]._0 = iIndices[0];
			pIndices[dwNumFaces]._1 = iIndices[2];
			pIndices[dwNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition) -
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._0].vPosition);

			vDestDir = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) -
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);

			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._0].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);

			++dwNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexSizePrimitive * m_iNumPrimitives;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_FlatTerrain::ReSize(VIBUFFER_FLAT_TERRAIN_DESC Vibuffer_Flat_Terrain_Desc)
{
	Safe_Delete_Array(m_pPos);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	if (FAILED(Initialize(&Vibuffer_Flat_Terrain_Desc)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_FlatTerrain* CVIBuffer_FlatTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_FlatTerrain* pInstance = new CVIBuffer_FlatTerrain(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_FlatTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_FlatTerrain::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_FlatTerrain* pInstance = new CVIBuffer_FlatTerrain(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_FlatTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_FlatTerrain::Free()
{
	Safe_Delete_Array(m_pPos);

	__super::Free();

}

#include "..\Public\VIBuffer_Point_Instance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
{

}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
	, m_iNumInstances(rhs.m_iNumInstances)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_iInstanceStride(rhs.m_iInstanceStride)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Point_Instance::Initialize_Prototype(_uint iNumInstance)
{
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iIndexSizePrimitive = sizeof(_ushort);
	m_iNumInstances = iNumInstance;
	m_iNumPrimitives = iNumInstance;
	m_iNumIndicesPrimitive = 1;
	m_iNumBuffers = 2;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

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

	VTXPOINT* pVertices = new VTXPOINT;
	ZeroMemory(pVertices, m_iStride);

	/* 로컬스페이스 상의 위치 정보를 채운다. */
	pVertices->vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices->vPSize = _float2(1.0f, 1.f);

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

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

	_ushort* pIndices = new _ushort[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitives);

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

#pragma region INSTANCE_BUFFER
	m_iInstanceStride = sizeof(VTXMATRIX);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstances;
	/* D3D11_USAGE_DEFAULT : CPU에 의한 값 갱신이 불가능하다 */
	/* D3D11_USAGE_DYNAMIC : CPU(C++코드, Lock, unLock)에 의한 값 갱신이 가능하다. */
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXMATRIX* pInstanceVertices = new VTXMATRIX[m_iNumInstances];
	ZeroMemory(pInstanceVertices, m_iInstanceStride * m_iNumInstances);

	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstanceVertices[i].vTranslation = _float4(0.0f, 0.0f, 0.0f, 1.f);
	}

	m_SubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Initialize(void* pArg)
{
	memcpy(&m_PointInstanceDesc, pArg, sizeof(POINT_INSTANCE_DESC));

	m_pSpeed = new _float[m_iNumInstances];
	m_pRotateDirection = new _float[m_iNumInstances];
	m_pRotateSpeed = new _float[m_iNumInstances];
	m_pRotateAxis = new _vector[m_iNumInstances];

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		//  회전방향 
		{
			_int rotDir = rand() % 2;
			if (rotDir == 0)
				m_pRotateDirection[i] = 1.f;
			else
				m_pRotateDirection[i] = -1.f;
		}

		// 회전 속도
		{
			m_pRotateSpeed[i] = XMConvertToRadians(_float(rand() % 50 + 10));
		}

		// 회전 각도
		{
			_int rotAxis = rand() % 3;

			if (rotAxis == 0)
			{
				m_pRotateAxis[i] = XMVectorSet(1.f, 0.f, 0.f, 0.f);
			}
			else if (rotAxis == 1)
			{
				m_pRotateAxis[i] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			}
			else
			{
				m_pRotateAxis[i] = XMVectorSet(0.f, 0.f, 1.f, 0.f);
			}
		}

		m_pSpeed[i] = rand() % int(m_PointInstanceDesc.fMaxSpeed - m_PointInstanceDesc.fMinSpeed) + m_PointInstanceDesc.fMinSpeed;

		_float	fHalfWidth = m_PointInstanceDesc.vSize.x * 0.5f;
		_float	fHalfHeight = m_PointInstanceDesc.vSize.y * 0.5f;

		// (rand() % (max - min) + min)

		((VTXMATRIX*)SubResource.pData)[i].vTranslation = _float4(rand() %
			_int(m_PointInstanceDesc.vPosition.x +
				fHalfWidth - (m_PointInstanceDesc.vPosition.x - fHalfWidth)) + m_PointInstanceDesc.vPosition.x - fHalfWidth,
			m_PointInstanceDesc.vPosition.y,
			rand() % _int(m_PointInstanceDesc.vPosition.z + fHalfHeight - (m_PointInstanceDesc.vPosition.z - fHalfHeight)) + m_PointInstanceDesc.vPosition.z - fHalfHeight,
			1.f);
	}

	m_pContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Render()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint				iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumBuffers, pVertexBuffers, iStrides, iOffsets);

	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);

	m_pContext->IASetPrimitiveTopology(m_eTopology);

	m_pContext->DrawIndexedInstanced(m_iNumIndicesPrimitive * 2, m_iNumInstances, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Point_Instance::Update(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		((VTXMATRIX*)SubResource.pData)[i].vTranslation.x += m_pSpeed[i] * (_float)TimeDelta;
		((VTXMATRIX*)SubResource.pData)[i].vTranslation.y -= m_pSpeed[i] * (_float)TimeDelta;
		((VTXMATRIX*)SubResource.pData)[i].vTranslation.z += m_pSpeed[i] * (_float)TimeDelta;


		_vector		vRight = XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight);
		_vector		vUp = XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp);
		_vector		vLook = XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook);

		_matrix		RotationMatrix = XMMatrixRotationAxis(m_pRotateAxis[i], m_pRotateSpeed[i] * TimeDelta * m_pRotateDirection[i]);

		XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, XMVector3TransformNormal(vRight, RotationMatrix));
		XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, XMVector3TransformNormal(vUp, RotationMatrix));
		XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, XMVector3TransformNormal(vLook, RotationMatrix));

		if (((VTXMATRIX*)SubResource.pData)[i].vTranslation.y < -4.0f)
		{
			//m_pSpeed[i] = rand() % int(m_PointInstanceDesc.fMaxSpeed - m_PointInstanceDesc.fMinSpeed) + m_PointInstanceDesc.fMinSpeed;

			_float	fHalfWidth = m_PointInstanceDesc.vSize.x * 0.5f;
			_float	fHalfHeight = m_PointInstanceDesc.vSize.y * 0.5f;

			// (rand() % (max - min) + min)

			((VTXMATRIX*)SubResource.pData)[i].vTranslation = _float4(rand() %
				_int(m_PointInstanceDesc.vPosition.x +
					fHalfWidth - (m_PointInstanceDesc.vPosition.x - fHalfWidth)) + m_PointInstanceDesc.vPosition.x - fHalfWidth,
				m_PointInstanceDesc.vPosition.y,
				rand() % _int(m_PointInstanceDesc.vPosition.z + fHalfHeight - (m_PointInstanceDesc.vPosition.z - fHalfHeight)) + m_PointInstanceDesc.vPosition.z - fHalfHeight,
				1.f);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Point_Instance::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	if (!m_bClone)
	{
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
	}
	CComponent::Free();

	Safe_Delete_Array(m_pSpeed);
	Safe_Delete_Array(m_pRotateDirection);
	Safe_Delete_Array(m_pRotateSpeed);
	Safe_Delete_Array(m_pRotateAxis);

	if (!m_bClone)
		Safe_Release(m_pVBInstance);

}

#include "PickingCube.h"
#include "GameInstance.h"

_uint CPickingCube::g_iColliderID = 0;

CPickingCube::CPickingCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
	, m_iID(g_iColliderID++)
{
	m_vMin = { -1.f, -1.f, -1.f };
	m_vMax = { 1.f, 1.f, 1.f };
}

CPickingCube::CPickingCube(const CPickingCube& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
	, m_iID(g_iColliderID++)
	, m_bCustomScale(rhs.m_bCustomScale)
	, m_pVtxWire(rhs.m_pVtxWire)
	, m_pIndex(rhs.m_pIndex)
{
	m_vMin = rhs.m_vMin;
	m_vMax = rhs.m_vMax;

	for (_int i = 0; i < 8; ++i)
	{
		m_vVtx[i] = rhs.m_vVtx[i];
		m_vOriginVtx[i] = rhs.m_vOriginVtx[i];
	}

	m_vCenter = rhs.m_vCenter;
}

CPickingCube::~CPickingCube()
{
}

HRESULT CPickingCube::Initialize_Prototype()
{
	m_iStride = sizeof(VTXWIRE);
	m_iNumVertices = 8;
	m_iIndexSizePrimitive = sizeof(FACEINDICES16);
	m_iNumPrimitives = 12;
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

	m_pVtxWire = new VTXWIRE[m_iNumVertices];
	ZeroMemory(m_pVtxWire, m_iStride * m_iNumVertices);

	m_pVtxWire[0].vPosition = _float3(-1.f, 1.f, -1.f);
	m_pVtxWire[1].vPosition = _float3(1.f, 1.f, -1.f);
	m_pVtxWire[2].vPosition = _float3(1.f, -1.f, -1.f);
	m_pVtxWire[3].vPosition = _float3(-1.f, -1.f, -1.f);
					
	m_pVtxWire[4].vPosition = _float3(-1.f, 1.f, 1.f);
	m_pVtxWire[5].vPosition = _float3(1.f, 1.f, 1.f);
	m_pVtxWire[6].vPosition = _float3(1.f, -1.f, 1.f);
	m_pVtxWire[7].vPosition = _float3(-1.f, -1.f, 1.f);

	m_SubResourceData.pSysMem = m_pVtxWire;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

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

	m_pIndex[0]._0 = 1;
	m_pIndex[0]._1 = 5;
	m_pIndex[0]._2 = 6;

	m_pIndex[1]._0 = 1;
	m_pIndex[1]._1 = 6;
	m_pIndex[1]._2 = 2;

	m_pIndex[2]._0 = 4;
	m_pIndex[2]._1 = 0;
	m_pIndex[2]._2 = 3;

	m_pIndex[3]._0 = 4;
	m_pIndex[3]._1 = 3;
	m_pIndex[3]._2 = 7;

	m_pIndex[4]._0 = 4;
	m_pIndex[4]._1 = 5;
	m_pIndex[4]._2 = 1;

	m_pIndex[5]._0 = 4;
	m_pIndex[5]._1 = 1;
	m_pIndex[5]._2 = 0;

	m_pIndex[6]._0 = 3;
	m_pIndex[6]._1 = 2;
	m_pIndex[6]._2 = 6;

	m_pIndex[7]._0 = 3;
	m_pIndex[7]._1 = 6;
	m_pIndex[7]._2 = 7;

	m_pIndex[8]._0 = 7;
	m_pIndex[8]._1 = 6;
	m_pIndex[8]._2 = 5;

	m_pIndex[9]._0 = 7;
	m_pIndex[9]._1 = 5;
	m_pIndex[9]._2 = 4;

	m_pIndex[10]._0 = 0;
	m_pIndex[10]._1 = 1;
	m_pIndex[10]._2 = 2;

	m_pIndex[11]._0 = 0;
	m_pIndex[11]._1 = 2;
	m_pIndex[11]._2 = 3;

	m_SubResourceData.pSysMem = m_pIndex;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

#pragma endregion
	return S_OK;
}

HRESULT CPickingCube::Initialize(void* pArg)
{
	return S_OK;
}

void CPickingCube::On_Collision(CPickingCube* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CPickingCube::On_CollisionEnter(CPickingCube* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CPickingCube::On_CollisionExit(CPickingCube* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

_uint CPickingCube::LateTick(_double TimeDelta)
{
	if (m_bCustomScale)
	{

	}
	else
	{
		m_vMin = { -1.f, -1.f, -1.f };
		m_vMax = { 1.f, 1.f, 1.f };

		_matrix matWorld = XMMatrixIdentity();

		CTransform* pOwnerTransform = (CTransform*)CGameInstance::GetInstance()->Get_Component(CTransform::familyId, m_pOwner);
		
		if (nullptr == pOwnerTransform)
			return 0;

		matWorld = pOwnerTransform->Get_WorldMatrix_Matrix();

		for (_int i = 0; i < 8; ++i)
		{
			XMStoreFloat3(&m_vVtx[i], XMVector3TransformCoord(XMLoadFloat3(&m_vOriginVtx[i]), matWorld));
		}

		// center
		XMStoreFloat3(&m_vCenter, (XMLoadFloat3(&m_vMax) - XMLoadFloat3(&m_vMin)) * 0.5f + XMLoadFloat3(&m_vMin));
	}

	return _uint();
}

HRESULT CPickingCube::Render()
{
	return S_OK;
}

_bool CPickingCube::Picking_By_Ray(_fvector vRayOrigin, _fvector vRayDiretion)
{
	if (m_pVtxWire == nullptr || m_pIndex == nullptr)
		return false;

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (size_t i = 0; i < m_iNumPrimitives; ++i)
	{
		_float4 vtx1{ 0.f, 0.f, 0.f, 1.f };
		_float4 vtx2{ 0.f, 0.f, 0.f, 1.f };
		_float4 vtx3{ 0.f, 0.f, 0.f, 1.f };
		memcpy(&vtx1, &m_pVtxWire[m_pIndex[i]._1].vPosition, sizeof _float3);
		memcpy(&vtx2, &m_pVtxWire[m_pIndex[i]._2].vPosition, sizeof _float3);
		memcpy(&vtx3, &m_pVtxWire[m_pIndex[i]._0].vPosition, sizeof _float3);

		if (TriangleTests::Intersects(vRayOrigin,
			vRayDiretion,
			XMLoadFloat4(&vtx1),
			XMLoadFloat4(&vtx2),
			XMLoadFloat4(&vtx3), fDist))
		{
			return true;
		}
	}

	return false;
}


CPickingCube* CPickingCube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPickingCube* pInstance = new CPickingCube(pDevice, pContext, nullptr);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPickingCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPickingCube::Clone(CGameObject * pOwner, void* pArg)
{
	CPickingCube* pInstance = new CPickingCube(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPickingCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickingCube::Free()
{
	if (!m_bClone)
	{
		Safe_Delete_Array(m_pIndex);
		Safe_Delete_Array(m_pVtxWire);
	}

	__super::Free();
}

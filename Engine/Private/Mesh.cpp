#include "..\Public\Mesh.h"
#include "Bone.h"
#include "Model.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
{

}

CMesh::CMesh(const CMesh& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_Bones(rhs.m_Bones)
	, m_ViewZ(rhs.m_ViewZ)
{
	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);
}

void CMesh::Get_BoneMatrices(_float4x4* pBoneMatrices, _matrix PivotMatrix)
{
	_uint		iNumBones = 0;

	/* 이 메시에 영향을 주는 뼈들의 최종 렌더링 행렬을 배열에 담아 넘겨준다. */

	for (auto& pBone : m_Bones)
	{
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], pBone->Get_OffsetMatrix() * pBone->Get_CombinedTransformationMatrix() * PivotMatrix);
	}
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iIndexSizePrimitive = sizeof(FACEINDICES32);
	m_iNumPrimitives = pAIMesh->mNumFaces;
	m_iNumIndicesPrimitive = 3;
	m_iNumBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	HRESULT hr = E_FAIL;
		
	switch (eType)
	{
	case Engine::CModel::TYPE_NONANIM:
		hr = Ready_VertexBuffer_ForNonAnim(pAIMesh, PivotMatrix);
		break;
	case Engine::CModel::TYPE_ANIM:
		hr = Ready_VertexBuffer_ForAnim(pAIMesh, pModel);
		break;
	case Engine::CModel::TYPE_NONANIM_UI:
		hr = Ready_VertexBuffer_ForNonAnimUI(pAIMesh, PivotMatrix);
		break;
	case Engine::CModel::TYPE_ANIM_UI:
		hr = Ready_VertexBuffer_ForAnimUI(pAIMesh, pModel);
		break;
	case Engine::CModel::TYPE_MESH_COLOR_NONANIM:
		hr = Ready_VertexBuffer_ForColorNonAnim(pAIMesh, PivotMatrix);
		break;
	default:
		return E_FAIL;
		break;
	}

	if (FAILED(hr))
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

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];

	for (_uint i = 0; i < m_iNumPrimitives; ++i)
	{
		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}


HRESULT CMesh::Ready_VertexBuffer_ForNonAnim(aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	m_iStride = sizeof(VTXMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix)));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForAnim(aiMesh* pAIMesh, CModel* pModel)
{
	m_iStride = sizeof(VTXANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	/* 이 메시에 영향을 주는 뼈의 개수. */
	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		CBone* pBone = pModel->Get_BonePtr(pAIBone->mName.data);

		/* 뼈의 상태를 정점의 로컬스페이스로 변환하기위한 행렬. */
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBone->SetUp_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_Bones.push_back(pBone);

		Safe_AddRef(pBone);

		/* pAIBone->mNumWeights : 이 뼈는 몇개의 정점에 영향을 주는가? */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : i번째 뼈는 어떤 정점에게 영향을 줘야하는가? */

			if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_Bones.push_back(pModel->Get_BonePtr(m_szName));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForNonAnimUI(aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	m_iStride = sizeof(VTXTEX);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	_bool zSet = false;

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		if (!zSet)
		{
			m_ViewZ = min(max(pVertices[i].vPosition.z, 0.f), 1.f);
			zSet = true;
		}

		pVertices[i].vPosition.z = 0.f;
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForAnimUI(aiMesh* pAIMesh, CModel* pModel)
{
	m_iStride = sizeof(VTXANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	_bool zSet = false;
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		if (!zSet)
		{
			m_ViewZ = min(max(pVertices[i].vPosition.z, 0.f), 1.f);
			zSet = true;
		}
		pVertices[i].vPosition.z = 0.f;
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	/* 이 메시에 영향을 주는 뼈의 개수. */
	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		CBone* pBone = pModel->Get_BonePtr(pAIBone->mName.data);

		/* 뼈의 상태를 정점의 로컬스페이스로 변환하기위한 행렬. */
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBone->SetUp_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_Bones.push_back(pBone);

		Safe_AddRef(pBone);

		/* pAIBone->mNumWeights : 이 뼈는 몇개의 정점에 영향을 주는가? */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : i번째 뼈는 어떤 정점에게 영향을 줘야하는가? */

			if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_Bones.push_back(pModel->Get_BonePtr(m_szName));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForColorNonAnim(aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	m_iStride = sizeof(VTXCOLORMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXCOLORMODEL* pVertices = new VTXCOLORMODEL[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix)));

		memcpy(&pVertices[i].vColor, &pAIMesh->mColors[0][i], sizeof(_float4));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, pModel, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(CGameObject* pOwner, void* pArg)
{
	CMesh* pInstance = new CMesh(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
}

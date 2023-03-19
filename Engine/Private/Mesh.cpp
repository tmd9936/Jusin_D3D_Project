#include "..\Public\Mesh.h"
#include "Bone.h"
#include "Model.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CVIBuffer(pDevice, pContext, pOwner)
{

}

CMesh::CMesh(const CMesh& rhs, CGameObject* pOwner)
	: CVIBuffer(rhs, pOwner)
{
}

void CMesh::Get_BoneMatrices(_float4x4* pBoneMatrices, const vector<CBone*>& Bones, _matrix PivotMatrix)
{
	_uint		iNumBones = 0;

	/* �� �޽ÿ� ������ �ִ� ������ ���� ������ ����� �迭�� ��� �Ѱ��ش�. */

	for (auto& iBoneIndex : m_Bones)
	{
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], Bones[iBoneIndex]->Get_OffsetMatrix() * Bones[iBoneIndex]->Get_CombinedTransformationMatrix() * PivotMatrix);
	}
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix, _bool DataSave)
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

	if (DataSave)
		m_VertexBufferData.resize(m_iNumVertices);

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
	case Engine::CModel::TYPE_MESH_COLOR_ANIM:
		hr = Ready_VertexBuffer_ForColorAnim(pAIMesh, pModel);
		break;

	default:
		return E_FAIL;
	break;
	}

	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEX_BUFFER
	if (DataSave)
		m_IndexBufferData.resize(m_iNumPrimitives);

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

		if (DataSave)
			m_IndexBufferData.push_back(pIndices[i]);
	}

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eType, const Value& Mesh)
{
	strcpy_s(m_szName, Mesh["szName"].GetString());
	m_iMaterialIndex = Mesh["m_iMaterialIndex"].GetUint();
	m_iNumVertices = Mesh["m_iNumVertices"].GetUint();
	m_iIndexSizePrimitive = sizeof(FACEINDICES32);
	m_iNumPrimitives = Mesh["m_iNumPrimitives"].GetUint();
	m_iNumIndicesPrimitive = 3;
	m_iNumBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	HRESULT hr = E_FAIL;

	switch (eType)
	{
	case Engine::CModel::TYPE_NONANIM:
		//hr = Ready_VertexBuffer_ForNonAnim(pAIMesh, PivotMatrix);
		break;
	case Engine::CModel::TYPE_ANIM:
		//hr = Ready_VertexBuffer_ForAnim(pAIMesh, pModel);
		break;
	case Engine::CModel::TYPE_NONANIM_UI:
		//hr = Ready_VertexBuffer_ForNonAnimUI(pAIMesh, PivotMatrix);
		break;
	case Engine::CModel::TYPE_ANIM_UI:
		//hr = Ready_VertexBuffer_ForAnimUI(pAIMesh, pModel);
		break;
	case Engine::CModel::TYPE_MESH_COLOR_NONANIM:
		//hr = Ready_VertexBuffer_ForColorNonAnim(pAIMesh, PivotMatrix);
		break;
	case Engine::CModel::TYPE_MESH_COLOR_ANIM:
		hr = Ready_VertexBuffer_ForColorAnim_Json(Mesh);
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
	
	const Value& IndexBuffers = Mesh["IndexBuffers"].GetArray();
	assert(IndexBuffers.IsArray());
	for (size_t i = 0; i < m_iNumPrimitives; ++i)
	{
		pIndices[i]._0 = IndexBuffers[i]["_0"].GetUint();
		pIndices[i]._1 = IndexBuffers[i]["_1"].GetUint();
		pIndices[i]._2 = IndexBuffers[i]["_2"].GetUint();
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


HRESULT CMesh::Ready_VertexBuffer_ForNonAnim(aiMesh* pAIMesh, _fmatrix PivotMatrix, _bool DataSave)
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

		if (DataSave)
		{
			m_VertexBufferData[i].vPosition = pVertices[i].vPosition;
			m_VertexBufferData[i].vNormal = pVertices[i].vNormal;
			m_VertexBufferData[i].vTexUV = pVertices[i].vTexUV;
		}
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForAnim(aiMesh* pAIMesh, CModel* pModel, _bool DataSave)
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


		if (DataSave) 
		{
			m_VertexBufferData[i].vPosition = pVertices[i].vPosition;
			m_VertexBufferData[i].vNormal = pVertices[i].vNormal;
			m_VertexBufferData[i].vTexUV = pVertices[i].vTexUV;
		}
	}

	/* �� �޽ÿ� ������ �ִ� ���� ����. */
	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		CBone* pBone = pModel->Get_BonePtr(pAIBone->mName.data);

		/* ���� ���¸� ������ ���ý����̽��� ��ȯ�ϱ����� ���. */
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBone->SetUp_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_Bones.push_back(pModel->Get_BoneIndex(pAIBone->mName.data));

		/* pAIBone->mNumWeights : �� ���� ��� ������ ������ �ִ°�? */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : i��° ���� � �������� ������ ����ϴ°�? */

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

			if (DataSave)
			{
				m_VertexBufferData[pAIBone->mWeights[j].mVertexId].vBlendIndex = pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex;
				m_VertexBufferData[pAIBone->mWeights[j].mVertexId].vBlendWeight = pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_Bones.push_back(pModel->Get_BoneIndex(m_szName));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForNonAnimUI(aiMesh* pAIMesh, _fmatrix PivotMatrix, _bool DataSave)
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

		if (DataSave)
		{
			m_VertexBufferData[i].vPosition = pVertices[i].vPosition;
			m_VertexBufferData[i].vTexUV = pVertices[i].vTexUV;
		}
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForAnimUI(aiMesh* pAIMesh, CModel* pModel, _bool DataSave)
{
	m_iStride = sizeof(VTXUIANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXUIANIMMODEL* pVertices = new VTXUIANIMMODEL[m_iNumVertices];
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
		//memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));

		if (DataSave)
		{
			m_VertexBufferData[i].vPosition = pVertices[i].vPosition;
			m_VertexBufferData[i].vTexUV = pVertices[i].vTexUV;
		}
	}

	/* �� �޽ÿ� ������ �ִ� ���� ����. */
	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		CBone* pBone = pModel->Get_BonePtr(pAIBone->mName.data);

		/* ���� ���¸� ������ ���ý����̽��� ��ȯ�ϱ����� ���. */
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBone->SetUp_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_Bones.push_back(pModel->Get_BoneIndex(pAIBone->mName.data));

		/* pAIBone->mNumWeights : �� ���� ��� ������ ������ �ִ°�? */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : i��° ���� � �������� ������ ����ϴ°�? */

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

			if (DataSave)
			{
				m_VertexBufferData[pAIBone->mWeights[j].mVertexId].vBlendIndex = pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex;
				m_VertexBufferData[pAIBone->mWeights[j].mVertexId].vBlendWeight = pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_Bones.push_back(pModel->Get_BoneIndex(m_szName));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForColorNonAnim(aiMesh* pAIMesh, _fmatrix PivotMatrix, _bool DataSave)
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

		if (DataSave)
		{
			m_VertexBufferData[i].vPosition = pVertices[i].vPosition;
			m_VertexBufferData[i].vNormal = pVertices[i].vNormal;
			m_VertexBufferData[i].vColor = pVertices[i].vColor;
		}
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForColorAnim(aiMesh* pAIMesh, CModel* pModel, _bool DataSave)
{
	m_iStride = sizeof(VTXCOLORANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXCOLORANIMMODEL* pVertices = new VTXCOLORANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vColor, &pAIMesh->mColors[0][i], sizeof(_float4));

		if (DataSave)
		{
			m_VertexBufferData[i].vPosition = pVertices[i].vPosition;
			m_VertexBufferData[i].vNormal = pVertices[i].vNormal;
			m_VertexBufferData[i].vColor = pVertices[i].vColor;
		}
	}

	/* �� �޽ÿ� ������ �ִ� ���� ����. */
	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		CBone* pBone = pModel->Get_BonePtr(pAIBone->mName.data);

		/* ���� ���¸� ������ ���ý����̽��� ��ȯ�ϱ����� ���. */
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBone->SetUp_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_Bones.push_back(pModel->Get_BoneIndex(pAIBone->mName.data));

		/* pAIBone->mNumWeights : �� ���� ��� ������ ������ �ִ°�? */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : i��° ���� � �������� ������ ����ϴ°�? */

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

			if (DataSave)
			{
				m_VertexBufferData[pAIBone->mWeights[j].mVertexId].vBlendIndex = pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex;
				m_VertexBufferData[pAIBone->mWeights[j].mVertexId].vBlendWeight = pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_Bones.push_back(pModel->Get_BoneIndex(m_szName));
	}

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForColorAnim_Json(const Value& Mesh)
{
	m_iStride = sizeof(VTXCOLORANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXCOLORANIMMODEL* pVertices = new VTXCOLORANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, m_iStride * m_iNumVertices);

	const Value& VertexBuffers = Mesh["VertexBuffers"].GetArray();
	assert(VertexBuffers.IsArray());
	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition =
		{
			VertexBuffers[i]["vPositionX"].GetFloat(),
			VertexBuffers[i]["vPositionY"].GetFloat(),
			VertexBuffers[i]["vPositionZ"].GetFloat()
		};

		pVertices[i].vNormal =
		{
			VertexBuffers[i]["vNormalX"].GetFloat(),
			VertexBuffers[i]["vNormalY"].GetFloat(),
			VertexBuffers[i]["vNormalZ"].GetFloat()
		};

		pVertices[i].vColor =
		{
			VertexBuffers[i]["vColorX"].GetFloat(),
			VertexBuffers[i]["vColorY"].GetFloat(),
			VertexBuffers[i]["vColorZ"].GetFloat(),
			VertexBuffers[i]["vColorW"].GetFloat()

		};

		pVertices[i].vBlendIndex =
		{
			VertexBuffers[i]["vBlendIndexX"].GetUint(),
			VertexBuffers[i]["vBlendIndexY"].GetUint(),
			VertexBuffers[i]["vBlendIndexZ"].GetUint(),
			VertexBuffers[i]["vBlendIndexW"].GetUint()
		};

		pVertices[i].vBlendWeight =
		{
			VertexBuffers[i]["vBlendWeightX"].GetFloat(),
			VertexBuffers[i]["vBlendWeightY"].GetFloat(),
			VertexBuffers[i]["vBlendWeightZ"].GetFloat(),
			VertexBuffers[i]["vBlendWeightW"].GetFloat()
		};
	}

	/* �� �޽ÿ� ������ �ִ� ���� ����. */
	m_iNumBones = Mesh["m_iNumBones"].GetInt();

	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix, _bool DataSave)
{
	CMesh* pInstance = new CMesh(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, pModel, PivotMatrix, DataSave)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, const Value& Mesh)
{
	CMesh* pInstance = new CMesh(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, Mesh)))
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

	m_Bones.clear();
	m_IndexBufferData.clear();
	m_VertexBufferData.clear();
}

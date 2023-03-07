#include "..\Public\Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Bone.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CModel::CModel(const CModel& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_eType(rhs.m_eType)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_Bones(rhs.m_Bones)
	, m_PivotMatrix(rhs.m_PivotMatrix)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_AddRef(pTexture);
	}

	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);
}

CBone* CModel::Get_BonePtr(const char* pBoneName)
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)-> _bool
	{
		if (false == strcmp(pBoneName, pBone->Get_Name()))
			return true;

		return false;
	});

	if (iter == m_Bones.end())
		return nullptr;

	return *iter;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix)
{
	_uint		iFlag = 0;

	m_eType = eType;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	/* aiProcess_PreTransformVertices : 정점정보를 읽어서 저장할 당시에 필요한 변환을 미리 처리해놓는다. */
	/* 처리? 이 모델의 뼈들 중, 메시의 이름과 같은 이름을 가진 뼈를 찾아서 그 뼈의 행렬을 정점들에게 미리 적용한다. */
	if (TYPE_NONANIM == m_eType)
		iFlag = aiProcess_PreTransformVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded; 
	else
		iFlag = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode)))
		return E_FAIL;

	/* 모델을 구성하는 메시들을 생성한다.(버텍스, 인덱스버퍼를 생성하는 것이다.) */
	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::SetUp_ShaderResource(CShader* pShader, const char* pConstantName, _uint iMeshIndex, aiTextureType eType)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (nullptr != m_Materials[iMaterialIndex].pMtrlTexture[eType])
	{
		if (FAILED(m_Materials[iMaterialIndex].pMtrlTexture[eType]->Set_ShaderResource(pShader, pConstantName)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	if (nullptr != m_Meshes[iMeshIndex])
		m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Ready_Bones(aiNode* pAINode)
{
	CBone* pBone = CBone::Create(pAINode);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
	{
		Ready_Bones(pAINode->mChildren[i]);
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;


	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}


	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	/* 머테리얼을 표현하기위한 텍스쳐를 로드하자. */
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];
		MESH_MATERIAL	ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof ModelMaterial);

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			_uint	iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));

			aiString		TexturePath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &TexturePath)))
				continue;

			char		szFileName[MAX_PATH] = "";
			char		szEXT[MAX_PATH] = "";

			_splitpath_s(TexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			char		szDrive[MAX_PATH] = "";
			char		szDir[MAX_PATH] = "";

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);

			char		szFullPath[MAX_PATH] = "";

			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar		szFinalPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szFinalPath, MAX_PATH);

			ModelMaterial.pMtrlTexture[j] = CTexture::Create(m_pDevice, m_pContext, szFinalPath);
			if (nullptr == ModelMaterial.pMtrlTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(CGameObject* pOwner, void* pArg)
{
	CModel* pInstance = new CModel(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_Release(pTexture);
	}

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	m_Materials.clear();

	m_Importer.FreeScene();
}

#include "..\Public\Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Bone.h"
#include "Animation.h"
#include "Shader.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CModel::CModel(const CModel& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eType(rhs.m_eType)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_Bones(rhs.m_Bones)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_Animations(rhs.m_Animations)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_AddRef(pTexture);
	}
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

const _float CModel::Get_ViewZ(_uint iMeshIndex)
{
	if (nullptr != m_Meshes[iMeshIndex])
		return m_Meshes[iMeshIndex]->Get_ViewZ();
	return 0.f;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix)
{
	_uint		iFlag = 0;

	m_eType = eType;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	/* aiProcess_PreTransformVertices : ���������� �о ������ ��ÿ� �ʿ��� ��ȯ�� �̸� ó���س��´�. */
	/* ó��? �� ���� ���� ��, �޽��� �̸��� ���� �̸��� ���� ���� ã�Ƽ� �� ���� ����� �����鿡�� �̸� �����Ѵ�. */
	if (TYPE_NONANIM == m_eType || TYPE_NONANIM_UI == eType || TYPE_MESH_COLOR_NONANIM == eType)
		iFlag = aiProcess_PreTransformVertices | aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
	else
		iFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, nullptr)))
		return E_FAIL;

	/* ���� �����ϴ� �޽õ��� �����Ѵ�.(���ؽ�, �ε������۸� �����ϴ� ���̴�.) */
	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
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

HRESULT CModel::SetUp_BoneMatrices(CShader* pShader, const char* pConstantName, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[256];
	ZeroMemory(BoneMatrices, sizeof(_float4x4) * 256);

	m_Meshes[iMeshIndex]->Get_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

	return pShader->Set_MatrixArray(pConstantName, BoneMatrices, 256);
}

void CModel::Play_Animation(_double TimeDelta)
{
	/* ���� ���Ͽ� ����Ǿ��־��� ���� ������ �ð��� �´� ���·� ������ ���¸� �����ϰ�.
	�̷��� ����� ������ ���¸� �θ𿡼����� �ڽ����� ���������� �����Ͽ� �����س�����. */
	/*��� ���� ���¸� �����Ѵ�.*/

	/* CAnimation : Ư�� ������ ǥ���ϴ� ��ü. */
	/* : �� ������ �����ϱ����� �ʿ��� ������ ��������(�ð������� �ټ�, m_TransformationMatrix)�� ������. */
	m_Animations[m_iCurrentAnimationIndex]->Update(TimeDelta);

	/* �ֻ��� �θ������ �����Ͽ� ������ �ڽĻ����� ��ü�� ��ȸ�ϸ� Combine����� �ٽ� ������. */
	for (auto& pBone : m_Bones)
	{
		pBone->SetUp_CombinedTransformationMatrix();
	}
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	if (nullptr != m_Meshes[iMeshIndex])
		m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Ready_Bones(aiNode* pAINode, CBone* pParent)
{
	CBone* pBone = CBone::Create(pAINode, pParent);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
	{
		Ready_Bones(pAINode->mChildren[i], pBone);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	if (m_pAIScene->mNumCameras > 0)
	{
		//aiCamera camera = m_pAIScene->mCameras[0][0];
		//aiVector3D normalPos = camera.mLookAt.Normalize();

		//XMStoreFloat4x4(&m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix) * XMMatrixTranslation(-normalPos.x, -normalPos.y, -normalPos.z));

		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PivotMatrix));
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}

	}
	else 
	{
		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PivotMatrix));
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	/* ���׸����� ǥ���ϱ����� �ؽ��ĸ� �ε�����. */
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
	m_Materials.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	m_Importer.FreeScene();
}

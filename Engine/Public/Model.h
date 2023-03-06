#pragma once

#include "Component.h"

BEGIN(Engine)

class CBone;

class ENGINE_DLL CModel final : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_VIBUFFER;

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	CModel(const CModel& rhs, CGameObject* pOwner);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	CBone* Get_BonePtr(const char* pBoneName);

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT SetUp_ShaderResource(class CShader* pShader, const char* pConstantName, _uint iMeshIndex, aiTextureType eType);
	HRESULT Render(_uint iMeshIndex);
	HRESULT Ready_Bones(aiNode* pAINode);

private:
	Assimp::Importer				m_Importer; /* 경로의 파일을 읽어서 저장하는 기능을 한다. */
	const aiScene*					m_pAIScene = { nullptr }; /* 실제 데이터들을 저장하고 있으며 변수를 통해 접근 가능하도록 처리해주는 객체. */

private:
	TYPE					m_eType = { TYPE_END };

private:
	_uint					m_iNumMeshes = { 0 };
	vector<class CMesh*>	m_Meshes;

private:
	_float4x4				m_PivotMatrix = { };

private:
	/* 모델에게 정의되어있는 머테리얼의 갯수. */
	_uint					m_iNumMaterials = { 0 };
	vector<MESH_MATERIAL>	m_Materials;

private:
	/* 이 하나의 모델이 사용하는 모든 뼈들. */
	vector<CBone*>		m_Bones;

//private:
//	_uint						m_iNumAnimations = { 0 };
//	vector<class CAnimation*>	m_Animations;

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Materials(const char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END


#pragma once

#include "Component.h"

BEGIN(Engine)

class CBone;

class ENGINE_DLL CModel final : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_VIBUFFER;

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_NONANIM_UI, TYPE_ANIM_UI, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	CModel(const CModel& rhs, CGameObject* pOwner);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	CBone* Get_BonePtr(const char* pBoneName);

	const _float Get_ViewZ(_uint iMeshIndex);

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT SetUp_ShaderResource(class CShader* pShader, const char* pConstantName, _uint iMeshIndex, aiTextureType eType);
	HRESULT Render(_uint iMeshIndex);
	HRESULT Ready_Bones(aiNode* pAINode);

private:
	Assimp::Importer				m_Importer; /* ����� ������ �о �����ϴ� ����� �Ѵ�. */
	const aiScene*					m_pAIScene = { nullptr }; /* ���� �����͵��� �����ϰ� ������ ������ ���� ���� �����ϵ��� ó�����ִ� ��ü. */

private:
	TYPE					m_eType = { TYPE_END };

private:
	_uint					m_iNumMeshes = { 0 };
	vector<class CMesh*>	m_Meshes;

private:
	_float4x4				m_PivotMatrix = { };		

private:
	/* �𵨿��� ���ǵǾ��ִ� ���׸����� ����. */
	_uint					m_iNumMaterials = { 0 };
	vector<MESH_MATERIAL>	m_Materials;

private:
	/* �� �ϳ��� ���� ����ϴ� ��� ����. */
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


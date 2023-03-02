#pragma once

#include "Component.h"

BEGIN(Engine)

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
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Render();

private:
	Assimp::Importer				m_Importer; /* ����� ������ �о �����ϴ� ����� �Ѵ�. */
	const aiScene* m_pAIScene = { nullptr }; /* ���� �����͵��� �����ϰ� ������ ������ ���� ���� �����ϵ��� ó�����ִ� ��ü. */

private:
	TYPE					m_eType = { TYPE_END };

private:
	_uint					m_iNumMeshes = { 0 };
	vector<class CMesh*>	m_Meshes;

private:
	HRESULT Ready_Meshes();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END


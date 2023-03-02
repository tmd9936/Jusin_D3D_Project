#pragma once

#include "Model.h"
#include "VIBuffer.h"

/* ���� �����ϴ� �ϳ��� �޽�. �޽��� ��ü�� �븮������. */
/* �޽ú��� ����, �ε������۸� ������. */
BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	CMesh(const CMesh& rhs, CGameObject* pOwner);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eType, aiMesh* pAIMesh);
	virtual HRESULT Initialize(void* pArg) override;



public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, aiMesh* pAIMesh);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
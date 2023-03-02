#pragma once

#include "Model.h"
#include "VIBuffer.h"

/* 모델을 구성하는 하나의 메시. 메시의 교체를 노리기위해. */
/* 메시별로 정점, 인덱스버퍼를 가진다. */
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
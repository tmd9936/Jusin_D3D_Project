#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class CPickingCube;
END

BEGIN(Client)

class CWorldMapGrass final : public CGameObject
{

private:
	explicit CWorldMapGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CWorldMapGrass(const CWorldMapGrass& rhs);
	virtual ~CWorldMapGrass() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CPickingCube* m_pPickingCubCom = { nullptr };

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */

public:
	static CWorldMapGrass* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
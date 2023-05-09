#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class CPickingCube;
class CAABB;
END

BEGIN(Client)

class CStove final : public CGameObject
{

private:
	explicit CStove(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStove(const CStove& rhs);
	virtual ~CStove() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Laplacian() override;

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	CTransform*			m_pTransformCom = { nullptr };
	CRenderer*			m_pRendererCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CPickingCube*		m_pPickingCube = { nullptr };
	CAABB*				m_pAABB = { nullptr };

public:
	static CStove* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
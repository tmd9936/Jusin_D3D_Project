#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CRenderer;
class CShader;
class CTexture;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	_float4x4			m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
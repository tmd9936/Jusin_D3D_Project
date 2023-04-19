#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Trail;
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CBone;
END

BEGIN(Client)

class CTrail final : public CGameObject
{
public:
	typedef struct TrailDesc
	{
		CBone*		pBonePtr = { nullptr };
		CTransform* pParent = { nullptr };
		_float4x4	PivotMatrix;

	}TRAIL_DESC;

private:
	CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Trail* m_pVIBufferCom = { nullptr };

private:
	_float4x4			m_FinalWorldMatrix = {}; /* 원점기준 (내 월드 * 부모월드) */
	TRAIL_DESC			m_Desc = {};

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

	_matrix Remove_Scale(_fmatrix Matrix);

public:
	static CTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
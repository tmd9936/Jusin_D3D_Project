#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
END

BEGIN(Client)

class CAnimEnv final : public CGameObject
{
public:
	typedef struct Anim_Env_Desc
	{
		_float3		vPos;
		_float3		vScale;
		_tchar		ModelPrototypeTag[MAX_PATH];
	} ANIM_ENV_DESC;

private:
	CAnimEnv(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnimEnv(const CAnimEnv& rhs);
	virtual ~CAnimEnv() = default;

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

private:
	ANIM_ENV_DESC		m_Desc = { };

public:
	static CAnimEnv* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
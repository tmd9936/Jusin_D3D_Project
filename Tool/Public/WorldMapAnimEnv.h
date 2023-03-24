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

class CWorldMapAnimEnv final : public CGameObject
{
public:
	typedef struct WorldMap_Anim_Env_Desc
	{
		_float3		vPos;
		_tchar		ModelPrototypeTag[MAX_PATH];
	} WORLDMAP_ANIM_ENV_DESC;

private:
	CWorldMapAnimEnv(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorldMapAnimEnv(const CWorldMapAnimEnv& rhs);
	virtual ~CWorldMapAnimEnv() = default;

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

	WORLDMAP_ANIM_ENV_DESC		m_Desc = { };


private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */

public:
	static CWorldMapAnimEnv* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_FlatTerrain;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CFlatTerrain final : public CGameObject
{
public:
	enum TEXTURETYPE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };
private:
	CFlatTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFlatTerrain(const CFlatTerrain& rhs);
	virtual ~CFlatTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_FlatTerrain* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom[TYPE_END] = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* Prototype */
	/* ���� ��ü�� �����Ѵ�. */
	static CFlatTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
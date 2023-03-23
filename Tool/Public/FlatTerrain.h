#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_FlatTerrain;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
class CNavigation;
END

BEGIN(Client)

class CFlatTerrain final : public CGameObject
{
public:
	typedef	struct FlatTerrain_desc
	{
		_uint m_Level;
	}FLATTERRAIN_DESC;

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

public:
	void		Switch_Wire() { 
		m_bWire = !m_bWire;
	}

	void		Switch_Render() {
		m_bRender = !m_bRender;
	}


private:
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_FlatTerrain* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom[TEXTURETYPE_END] = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };


private:
	_bool				m_bWire = { true };
	FLATTERRAIN_DESC	m_desc = {};

	_bool				m_bRender = { true };

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
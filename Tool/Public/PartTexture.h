#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPartTexture final : public CGameObject
{
public:
	typedef struct UI_Desc
	{
		CTransform*			pParent = { nullptr }; // ������ �Ǵ� �θ�
		CModel*				pParentModel = { nullptr }; // ������ �Ǵ� �θ�

		_float				m_fX;
		_float				m_fY;
		_float				m_fSizeX;
		_float				m_fSizeY;
		_uint				m_TextureProtoTypeLevel;
		_tchar				m_TextureProtoTypeName[MAX_PATH];
	} UI_DESC;

private:
	CPartTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartTexture(const CPartTexture& rhs);
	virtual ~CPartTexture() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;	

public:
	void	Set_Parent_Model(CModel* pModel) {
		m_UIDesc.pParentModel = pModel;
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	UI_DESC		m_UIDesc = {};
	_float4x4	m_ViewMatrix = {};
	_float4x4	m_ProjMatrix = {};

	_float4x4	m_FinalWorldMatrix; /* �������� (�� ���� * �θ����) */



private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* Prototype */
	/* ���� ��ü�� �����Ѵ�. */
	static CPartTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
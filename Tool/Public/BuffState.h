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
class CBone;
END

BEGIN(Client)

class CBuffState final : public CGameObject
{
public:
	typedef struct BuffState_Desc
	{
		CBone*				pBonePtr = { nullptr };
		CTransform*			pParent = { nullptr };
		_float4x4			PivotMatrix;

		_tchar				m_TextureProtoTypeName[MAX_PATH];
		_uint				m_TextureLevelIndex;

	} BUFFSTATE_DESC;
private:
	CBuffState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBuffState(const CBuffState& rhs);
	virtual ~CBuffState() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	BUFFSTATE_DESC		m_Desc = {};
	_float4x4			m_FinalWorldMatrix; /* �������� (�� ���� * �θ����) */

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	_matrix Remove_Scale(_fmatrix Matrix);


public:
	/* Prototype */
	/* ���� ��ü�� �����Ѵ�. */
	static CBuffState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
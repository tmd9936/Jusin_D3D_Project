#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPartText final : public CGameObject
{
public:
	typedef struct Text_Desc
	{
		CTransform*			pParent = { nullptr }; // ������ �Ǵ� �θ�
		CModel*				pParentModel = { nullptr }; // ������ �Ǵ� �θ�

		_float				m_fX;
		_float				m_fY;
		_float4				m_vColor;
		_float				m_Rotation;
		_float2				m_vRotationOrigin;
		_float2				m_vScale;

		_tchar				m_FontTag[MAX_PATH];
		_tchar				m_Text[MAX_PATH];
		
	} TEXT_DESC;

private:
	CPartText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartText(const CPartText& rhs);
	virtual ~CPartText() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Parent_Model(CModel* pModel) {
		m_Text_Desc.pParentModel = pModel;
	}

public:
	void	Set_Color(_float4 vColor) {
		m_Text_Desc.m_vColor = vColor;
	}

	void	Set_Scale(_float2 vScale) {
		m_Text_Desc.m_vScale = vScale;
	}

	void	Set_Text(const wstring& text) {
		lstrcpy(m_Text_Desc.m_Text, text.c_str());
	}

	void	Set_Text(const wstring& text, const _float& vPositionX) {
		lstrcpy(m_Text_Desc.m_Text, text.c_str());
		m_Text_Desc.m_fX = vPositionX;
	}


	const wstring	Get_Text() const {
		return m_Text_Desc.m_Text;
	}

	const _float Get_PositionX() const {
		return m_Text_Desc.m_fX;
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };

private:
	TEXT_DESC	m_Text_Desc = {};
	_float4x4	m_ViewMatrix = {};
	_float4x4	m_ProjMatrix = {};

	_float4x4	m_FinalWorldMatrix; /* �������� (�� ���� * �θ����) */

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* Prototype */
	/* ���� ��ü�� �����Ѵ�. */
	static CPartText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
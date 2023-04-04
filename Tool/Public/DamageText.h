#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CDamageText final : public CGameObject
{
public:
	typedef struct DamageText_Desc
	{
		CTransform*			pParent = { nullptr };

		_float4x4			PivotMatrix;

		_float				m_fPositionX;
		_float				m_fPositinoY;
		_float				m_fPositinoZ;

		_float				m_Rotation;
		_float2				m_vRotationOrigin;

		_float4				m_vColor;
		_float2				m_vScale;
		_tchar				m_FontTag[MAX_PATH];

		_uint				m_Damage;

	} DAMAGETEXT_DESC;

private:
	CDamageText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageText(const CDamageText& rhs);
	virtual ~CDamageText() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	void Show_Damage(_uint damage, _float4 vColor, _float2 vScale, _float rotation, _float2 vRotationOrigin);

private:
	void Show_TimeCheck(const _double& TimeDelta);

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };

private:
	DAMAGETEXT_DESC			m_Desc = {};

	_float4x4				m_FinalWorldMatrix; /* 원점기준 (내 월드 * 부모월드) */

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};
	
	_double					m_ShowTImeAcc = { 0.0 };
	_double					m_ShowTime = { 1.5 };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	_matrix Remove_Scale(_fmatrix Matrix);


public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CDamageText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
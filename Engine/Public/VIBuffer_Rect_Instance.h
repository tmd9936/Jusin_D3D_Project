#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
public:
	typedef struct tagRectInstanceDesc
	{
		_float3		vPosition;
		_float2		vSize;
		_float		fLifeTime;
		_float		fMinSpeed, fMaxSpeed;
	}RECT_INSTANCE_DESC;

private:
	explicit CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject * pOwner);
	explicit CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs, CGameObject* pOwner);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT Render() override;

public:
	void			Update(_double TimeDelta);

private:
	ID3D11Buffer*			m_pVBInstance = { nullptr }; // �ε����� ������ ���� ���� �������� ����ϱ� ������ ����x
	_uint					m_iNumInstances = { 0 };
	_uint					m_iInstanceStride = { 0 };

	RECT_INSTANCE_DESC		m_RectInstanceDesc = {};
	_float*					m_pSpeed = { nullptr };

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance = 1);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
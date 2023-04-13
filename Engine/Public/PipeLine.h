#pragma once

#include "Base.h"
#include "Engine_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	void Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix);

	_float4x4 Get_Transform_Float4x4(TRANSFORMSTATE eState) const {
		return m_TransformStateMatrix[eState];
	}

	_matrix Get_Transform_Matrix(TRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_TransformStateMatrix[eState]);
	}

	_float4x4 Get_Transform_Float4x4_Inverse(TRANSFORMSTATE eState) const {
		return m_TransformStateMatrix_Inverse[eState];
	}

	_matrix Get_Transform_Matrix_Inverse(TRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_TransformStateMatrix_Inverse[eState]);
	}

	_float4 Get_CamPosition() const {
		return m_vCamPosition;
	}

	_matrix Get_ViewPort_Matrix(float x, float y, float w, float h, float minZ, float maxZ);

	void Set_CameraDesc(const CCamera::CAMERADESC& desc) {
		memcpy(&m_CamDesc, &desc, sizeof m_CamDesc);
	}

	const CCamera::CAMERADESC Get_CameraDesc() const {
		return m_CamDesc;
	}

	_float	Get_CameraFar() const {
		return m_CamDesc.fFar;
	}

public:
	void	Update();

private:
	_float4x4			m_TransformStateMatrix[D3DTS_END] = { };
	_float4x4			m_TransformStateMatrix_Inverse[D3DTS_END] = { };

	_float4				m_vCamPosition = { };

	
	CCamera::CAMERADESC			m_CamDesc = {};

public:
	virtual void Free(void) override;

};

END
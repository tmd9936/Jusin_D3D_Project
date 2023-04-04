#include "stdafx.h"
#include "DamageText.h"

#include "GameInstance.h"

CDamageText::CDamageText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CDamageText::CDamageText(const CDamageText& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDamageText::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageText::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.pParent = (*(DAMAGETEXT_DESC*)(pArg)).pParent;
		m_Desc.PivotMatrix = (*(DAMAGETEXT_DESC*)(pArg)).PivotMatrix;

		m_Desc.m_fPositionX = (*(DAMAGETEXT_DESC*)(pArg)).m_fPositionX;
		m_Desc.m_fPositinoY = (*(DAMAGETEXT_DESC*)(pArg)).m_fPositinoY;
		m_Desc.m_fPositinoZ = (*(DAMAGETEXT_DESC*)(pArg)).m_fPositinoZ;

		m_Desc.m_Rotation = (*(DAMAGETEXT_DESC*)(pArg)).m_Rotation;
		m_Desc.m_vRotationOrigin = (*(DAMAGETEXT_DESC*)(pArg)).m_vRotationOrigin;

		m_Desc.m_vColor = (*(DAMAGETEXT_DESC*)(pArg)).m_vColor;
		m_Desc.m_vScale = (*(DAMAGETEXT_DESC*)(pArg)).m_vScale;
		lstrcpy(m_Desc.m_FontTag, (*(DAMAGETEXT_DESC*)(pArg)).m_FontTag);

		//m_Desc.m_Damage = (*(DAMAGETEXT_DESC*)(pArg)).m_Damage;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled({ m_Desc.m_vScale.x, m_Desc.m_vScale.y, 1.f });
	m_pTransformCom->Set_Pos(m_Desc.m_fPositionX, m_Desc.m_fPositinoY, m_Desc.m_fPositinoZ);

	//m_pTransformCom->Set_Scaled({ m_Desc.m_fSizeX, m_Desc.m_fSizeY, 1.f });

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_eRenderId = RENDER_END;

	return S_OK;
}

_uint CDamageText::Tick(_double TimeDelta)
{
	Show_TimeCheck(TimeDelta);

	return _uint();
}

_uint CDamageText::LateTick(_double TimeDelta)
{
	if (m_eRenderId != RENDER_END)
	{
		// 부모 기준으로의 자식의 위치를 지정함
		_float4 vPos{};
		XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		_matrix		ViewPortMatrix = CGameInstance::GetInstance()->Get_ViewPort_Matrix(vPos.x, vPos.y, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);
		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		_matrix viewMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
		_matrix projMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

		_float4x4 ParentMat{};
		XMStoreFloat4x4(&ParentMat, m_Desc.pParent->Get_WorldMatrix_Matrix());
		XMStoreFloat4x4(&ParentMat, XMLoadFloat4x4(&ParentMat) * viewMatrix * projMatrix * ViewPortMatrix);

		_float3 vScale = m_pTransformCom->Get_Scaled();
		_float4 r = { vScale.x, 0.f, 0.f, 0.f };
		_float4 u = { 0.f, vScale.y, 0.f, 0.f };
		_float4 l = { 0.f, 0.f, 1.f, 0.f };
		_float4 p = { ParentMat.m[3][0], ParentMat.m[3][1], 0.1f, 1.f };

		memcpy(ParentMat.m[0], &r, sizeof _float4);
		memcpy(ParentMat.m[1], &u, sizeof _float4);
		memcpy(ParentMat.m[2], &l, sizeof _float4);
		memcpy(ParentMat.m[3], &p, sizeof _float4);

		m_FinalWorldMatrix = ParentMat;

		m_FinalWorldMatrix.m[3][0] = m_FinalWorldMatrix.m[3][0] + g_iWinSizeX * 0.5f;
		m_FinalWorldMatrix.m[3][1] = -m_FinalWorldMatrix.m[3][1] + g_iWinSizeY * 0.5f;
		//m_FinalWorldMatrix.m[3][1] *= -1.f;
		//m_FinalWorldMatrix.m[3][2] = 0.1f;

		m_pRendererCom->Add_RenderGroup(m_eRenderId, this);
	}
	return _uint();
}

HRESULT CDamageText::Render()
{
	//if (FAILED(SetUp_ShaderResources()))
	//	return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Render_Font(m_Desc.m_FontTag,
		to_wstring(m_Desc.m_Damage).c_str(),
		_float2(m_FinalWorldMatrix.m[3][0], m_FinalWorldMatrix.m[3][1]),
		XMLoadFloat4(&m_Desc.m_vColor),
		m_Desc.m_Rotation, m_Desc.m_vRotationOrigin, _float2(m_FinalWorldMatrix.m[0][0], m_FinalWorldMatrix.m[1][1]))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CDamageText::Show_Damage(_uint damage, _float4 vColor, _float2 vScale, _float rotation, _float2 vRotationOrigin)
{
	m_Desc.m_Damage = damage;
	m_Desc.m_vColor = vColor;
	m_Desc.m_vScale = vScale;
	m_Desc.m_Rotation = rotation;
	m_Desc.m_vRotationOrigin = vRotationOrigin;

	m_pTransformCom->Set_Scaled({ m_Desc.m_vScale.x, m_Desc.m_vScale.y, 1.f });
	m_ShowTImeAcc = m_ShowTime;
	m_eRenderId = RENDER_UI;
}

void CDamageText::Show_TimeCheck(const _double& TimeDelta)
{
	if (m_ShowTImeAcc > 0.0)
	{
		m_ShowTImeAcc -= TimeDelta;

		if (m_ShowTImeAcc < 0.0)
		{
			m_ShowTImeAcc = 0.0;
			m_eRenderId = RENDER_END;
		}
	}
}		

HRESULT CDamageText::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CDamageText::SetUp_ShaderResources()
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

_matrix CDamageText::Remove_Scale(_fmatrix Matrix)
{
	_matrix		Result = Matrix;

	Result.r[0] = XMVector3Normalize(Result.r[0]);
	Result.r[1] = XMVector3Normalize(Result.r[1]);
	Result.r[2] = XMVector3Normalize(Result.r[2]);

	return Result;
}

CDamageText* CDamageText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageText* pInstance = new CDamageText(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CDamageText");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageText::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CDamageText* pInstance = new CDamageText(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CDamageText");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageText::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pParent);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

}

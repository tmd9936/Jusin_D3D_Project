#include "stdafx.h"
#include "BuffState.h"

#include "GameInstance.h"
#include "Bone.h"


/*
https://namu.wiki/w/%ED%8F%AC%EC%BC%93%EB%AA%AC%20%ED%80%98%EC%8A%A4%ED%8A%B8/%ED%8F%AC%EC%BC%93%EB%AA%AC#toc
 
상태이상은 능력치 상승·하락을 제외하면 한 포켓몬에게 하나만 걸릴 수 있으며, 오로지 시간이 지나야만 풀린다. 그리고 본가와는 다르게 타입에 상관 없이 모든 상태이상에 걸릴 수 있다. 불꽃 포켓몬이 화상을 입거나, 독 또는 강철 포켓몬이 중독되거나, 얼음 포켓몬이 얼어붙거나, 전기 포켓몬이 마비될 수 있다.
독: 본가와 동일하게 일정 주기마다 대미지를 입는다.
마비: 이동속도가 느려지고, 종종 기술을 사용할 수 없다(정확히는, 기술을 사용해도 종종 실패한다). 종종 흩어지기 명령을 사용해도 도망치지 못한다.
화상: 본가와 동일하게 일정 주기마다 대미지를 입고 공격력이 감소한다.
잠듦: 본가와 동일하게 이동이 불가능하고 기술을 사용할 수 없다.
얼음: 본가와 비슷하나 불꽃 기술을 맞았다고 지속시간이 짧아지지는 않는다.
혼란: 지속 시간 동안 일정 확률로 자해하며, 기술을 사용할 수 없다. 흩어지기 명령을 받으면 정상적으로 움직인다. 왕콘치나 골뱃이 초음파 또는 이상한빛으로 특히 자주 걸어대므로 주의할 것.
사슬묶기: 기술 하나를 일정 시간 동안 쓸 수 없게 한다. 에스퍼 타입의 포켓몬이나 골덕이 이 기술을 자주 사용한다.

*/

CBuffState::CBuffState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBuffState::CBuffState(const CBuffState& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBuffState::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuffState::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.pParentTransform = (*(BUFFSTATE_DESC*)(pArg)).pParentTransform;
		m_Desc.pParentHP = (*(BUFFSTATE_DESC*)(pArg)).pParentHP;
		m_Desc.pParentAttack = (*(BUFFSTATE_DESC*)(pArg)).pParentAttack;
		m_Desc.pParentMonFSM = (*(BUFFSTATE_DESC*)(pArg)).pParentMonFSM;
		m_Desc.pParentModel = (*(BUFFSTATE_DESC*)(pArg)).pParentModel;

		m_Desc.PivotMatrix = (*(BUFFSTATE_DESC*)(pArg)).PivotMatrix;

		m_Desc.m_fPositionX = (*(BUFFSTATE_DESC*)(pArg)).m_fPositionX;
		m_Desc.m_fPositinoY = (*(BUFFSTATE_DESC*)(pArg)).m_fPositinoY;
		m_Desc.m_fPositinoZ = (*(BUFFSTATE_DESC*)(pArg)).m_fPositinoZ;

		m_Desc.m_fSizeX = (*(BUFFSTATE_DESC*)(pArg)).m_fSizeX;
		m_Desc.m_fSizeY = (*(BUFFSTATE_DESC*)(pArg)).m_fSizeY;

		lstrcpy(m_Desc.m_TextureProtoTypeName, (*(BUFFSTATE_DESC*)(pArg)).m_TextureProtoTypeName);
		m_Desc.m_TextureLevelIndex = (*(BUFFSTATE_DESC*)(pArg)).m_TextureLevelIndex;

		//m_Desc.m_eBuffType = (*(BUFFSTATE_DESC*)(pArg)).m_eBuffType;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	Safe_AddRef(m_Desc.pParentTransform);
	Safe_AddRef(m_Desc.pParentHP);
	Safe_AddRef(m_Desc.pParentAttack);
	Safe_AddRef(m_Desc.pParentMonFSM);
	Safe_AddRef(m_Desc.pParentModel);

	// 뷰포트 기준으로 나타낼 크기를 지정함
	m_pTransformCom->Set_Scaled({ m_Desc.m_fSizeX, m_Desc.m_fSizeY, 1.f });
	// 부모 기준으로의 자식의 위치를 지정함
	// UI의 경우에는 뷰포트 기준으로 위치를 지정해줘야함
	m_pTransformCom->Set_Pos(m_Desc.m_fPositionX, m_Desc.m_fPositinoY, m_Desc.m_fPositinoZ);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_eRenderId = RENDER_UI;

	return S_OK;
}

_uint CBuffState::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	EndTime_Check(TimeDelta);

	Change_State();

	return _uint();
}

_uint CBuffState::LateTick(_double TimeDelta)
{
	//if (m_Desc.m_eBuffType != BUFF_TYPE_NONE)
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	// 부모 기준으로의 자식의 위치를 지정함
	_float4 BuffPos{};
	XMStoreFloat4(&BuffPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	// 뷰 투영 뷰포트 행렬을 가져올 때 UI지만 직교투영이 아닌 카메라 기준으로 가져오는 이유
	// 몬스터 기준으로 일단 UI 위치를 특정하기위함 
	_matrix		ViewPortMatrix = CGameInstance::GetInstance()->Get_ViewPort_Matrix(BuffPos.x, BuffPos.y, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	_matrix viewMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix projMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	_float4x4 ParentMat{};
	XMStoreFloat4x4(&ParentMat, m_Desc.pParentTransform->Get_WorldMatrix_Matrix());

	XMStoreFloat4x4(&ParentMat, XMLoadFloat4x4(&ParentMat) * viewMatrix * projMatrix * ViewPortMatrix);

	// x와 y에 z빼기를 해주면 뷰포트 기준으로의 위치가 만들어짐
	// z까지 z뺴기를 해주면 부모의 뒤로 랜더가 될 경우가 생기기 때문에 z는 임의의 값으로 놔둠
	_float3 vScale = m_pTransformCom->Get_Scaled();
	XMStoreFloat4x4(&m_FinalWorldMatrix, XMMatrixSet(
		vScale.x, 0.f, 0.f, 0.f,
		0.f, vScale.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		(ParentMat.m[3][0]) / ParentMat.m[3][2], (ParentMat.m[3][1]) / ParentMat.m[3][2], 0.f, 1.f
	));

	m_FinalWorldMatrix.m[3][0] = (m_FinalWorldMatrix.m[3][0]) - g_iWinSizeX * 0.5f;
	m_FinalWorldMatrix.m[3][1] = -(m_FinalWorldMatrix.m[3][1]) + g_iWinSizeY * 0.5f;
	//m_FinalWorldMatrix.m[3][1] *= -1.f;
	m_FinalWorldMatrix.m[3][2] = m_Desc.m_fPositinoZ;

	return _uint();
}

HRESULT CBuffState::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(SetUp_CoolTimeMask_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBuffState::Set_BuffState(_uint buffType, _uint skillType, BUFF_STATE eState, const _tchar* textureName, 
	_float valueA, _float valueB, _float endTime, _float ratio)
{
	if (buffType == m_CurBuffType && false == m_bCanBuffSet)
	{
		m_EndTimeAcc = 0.0;
		return S_OK;
	}

	if (nullptr == textureName)
		return E_FAIL;

	_tchar prototypeTag[MAX_PATH] = L"Prototype_Component_Texture_";
	lstrcat(prototypeTag, textureName);
	if (FAILED(Change_Texture(prototypeTag)))
		return E_FAIL;

	m_EndTimeAcc = 0.0;
	m_EndTime = (_double)endTime;
	m_eCurBuffState = eState;
	m_CurSkillType = skillType;
	m_ratio = ratio;
	m_valueA = valueA;
	m_valueB = valueB;
	m_CurBuffType = buffType;
	m_DeBuffTick = (_double)ratio;

	return S_OK;
}

HRESULT CBuffState::Change_Texture(const _tchar* prototypeTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (FAILED(pGameInstance->Change_Component(CTexture::familyId, this, LEVEL_STATIC, prototypeTag, (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CBuffState::EndTime_Check(const _double& TimeDelta)
{
	if (!m_bCanBuffSet)
	{
		if (m_EndTimeAcc >= m_EndTime)
		{
			m_eCurBuffState = BUFF_STATE_NONE;
		}
	}

	if (m_EndTimeAcc < m_EndTime)
		m_EndTimeAcc += TimeDelta;
}

void CBuffState::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurBuffState)
	{
	case BUFF_STATE_NONE:
		break;
	case BUFF_STATE_DAMAGE_UP:
		break;
	case BUFF_STATE_DAMAGE_DOWN:
		break;
	case BUFF_STATE_DEFENSE_UP:
		break;
	case BUFF_STATE_DEFENSE_DOWN:
		break;
	case BUFF_STATE_SPEED_UP:
		break;
	case BUFF_STATE_SPEED_DOWN:
		break;
	case BUFF_STATE_RESIST_UP:
		break;
	case BUFF_STATE_RESIST_DOWN:
		break;
	case BUFF_STATE_DOKU:
		break;
	case BUFF_STATE_MAHI:
		break;
	case BUFF_STATE_NEMURI:
		break;
	case BUFF_STATE_KOORI:
		break;
	case BUFF_STATE_YAKEDO:
		break;
	case BUFF_STATE_KONRAN:
		break;
	case BUFF_STATE_KANASIBARI:
		break;
	case BUFF_STATE_NEMURI2:
		break;
	case BUFF_STATE_END:
		break;
	default:
		break;
	}
}

void CBuffState::Change_State()
{
	if (m_eCurBuffState != m_ePreBuffState)
	{
		switch (m_eCurBuffState)
		{
		case BUFF_STATE_NONE:
			Return_Original_State(m_ePreBuffState);
			m_eRenderId = RENDER_END;
			m_bCanBuffSet = true;
			break;
		case BUFF_STATE_DAMAGE_UP:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_DAMAGE_DOWN:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_DEFENSE_UP:
			Change_State_Buff_On();
			Add_ParentSpeedPercent(m_valueA);
			break;
		case BUFF_STATE_DEFENSE_DOWN:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_SPEED_UP:
			Change_State_Buff_On();
			Add_ParentSpeedPercent(m_valueA);
			break;
		case BUFF_STATE_SPEED_DOWN:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_RESIST_UP:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_RESIST_DOWN:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_DOKU:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_MAHI:
			Change_State_Buff_On();
			Add_ParentSpeedPercent(-m_valueA);
			Set_ParentAttackFailProbability(4);
			break;
		case BUFF_STATE_NEMURI:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_KOORI:
			Change_State_Buff_On();
			Set_ParentState(CMonFSM::MONSTER_STATE::IDLE_NO);
			break;
		case BUFF_STATE_YAKEDO:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_KONRAN:
			Change_State_Buff_On();
			Set_ParentState(CMonFSM::MONSTER_STATE::IDLE_NO);
			break;
		case BUFF_STATE_KANASIBARI:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_NEMURI2:
			Change_State_Buff_On();
			break;
		case BUFF_STATE_END:
			Change_State_Buff_On();
			break;
		default:
			break;
		}

		m_ePreBuffState = m_eCurBuffState;
	}
}

void CBuffState::Change_State_Buff_On()
{
	m_eRenderId = RENDER_UI;
	m_bCanBuffSet = false;
	m_EndTimeAcc = 0.0;
	m_DeBuffTickAcc = 0.0;
}

void CBuffState::Set_ParentSpeedPercent(_float percent)
{
	if (nullptr == m_Desc.pParentTransform)
		return;

	m_Desc.pParentTransform->Set_SpeedPercent(percent);
}

void CBuffState::Add_ParentSpeedPercent(_float percent)
{
	if (nullptr == m_Desc.pParentTransform)
		return;

	m_returnValue = m_Desc.pParentTransform->Add_SpeedPercent(percent);
}

void CBuffState::Set_ParentDefensePercent(_float percent)
{
	if (nullptr == m_Desc.pParentHP)
		return;

	m_Desc.pParentHP->Set_DamageGetPercent(percent);
}

void CBuffState::Set_ParentAttackFailProbability(_int value)
{
	if (nullptr == m_Desc.pParentAttack)
		return;

	m_Desc.pParentAttack->Set_AttackFailProbability(value);
}

void CBuffState::Set_ParentState(CMonFSM::MONSTER_STATE eState)
{
	if (nullptr == m_Desc.pParentMonFSM || nullptr == m_Desc.pParentModel)
		return;

	m_Desc.pParentMonFSM->Transit_MotionState(eState, m_Desc.pParentModel);
}


void CBuffState::Return_Original_State(BUFF_STATE preState)
{
	switch (preState)
	{
	case BUFF_STATE_NONE:
		break;
	case BUFF_STATE_DAMAGE_UP:
		break;
	case BUFF_STATE_DAMAGE_DOWN:
		break;
	case BUFF_STATE_DEFENSE_UP:
		Set_ParentDefensePercent(1.f);
		break;
	case BUFF_STATE_DEFENSE_DOWN:
		Set_ParentDefensePercent(1.f);
		break;
	case BUFF_STATE_SPEED_UP:
		Add_ParentSpeedPercent(m_returnValue);
		break;
	case BUFF_STATE_SPEED_DOWN:
		Add_ParentSpeedPercent(m_returnValue);
		break;
	case BUFF_STATE_RESIST_UP:
		break;
	case BUFF_STATE_RESIST_DOWN:
		break;
	case BUFF_STATE_DOKU:
		break;
	case BUFF_STATE_MAHI:
		Set_ParentAttackFailProbability(1);
		Add_ParentSpeedPercent(m_returnValue);
		break;
	case BUFF_STATE_NEMURI:
		break;
	case BUFF_STATE_KOORI:
		Set_ParentState(CMonFSM::MONSTER_STATE::IDLE1);
		break;
	case BUFF_STATE_YAKEDO:
		break;
	case BUFF_STATE_KONRAN:
		Set_ParentState(CMonFSM::MONSTER_STATE::IDLE1);
		break;
	case BUFF_STATE_KANASIBARI:
		break;
	case BUFF_STATE_NEMURI2:
		break;
	case BUFF_STATE_END:
		break;
	default:
		break;
	}
}

HRESULT CBuffState::Add_Components()
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

	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_Rect::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		(CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, m_Desc.m_TextureProtoTypeName,
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	/* For.Com_MaskTexture */
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_TEXTURE_MASK, this, LEVEL_STATIC, TEXT("Prototype_Component_Texture_UIMask"),
		(CComponent**)&m_pMaskTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuffState::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	//if (FAILED(m_pMaskTextureCom->Set_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBuffState::SetUp_CoolTimeMask_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pMaskTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float progress = _float(m_EndTimeAcc / m_EndTime);
	m_pShaderCom->Set_RawValue("g_Progress", &progress, sizeof(_float));

	Safe_Release(pGameInstance);

	return S_OK;
}

//_matrix CBuffState::Remove_Scale(_fmatrix Matrix)
//{
//	_matrix		Result = Matrix;
//
//	Result.r[0] = XMVector3Normalize(Result.r[0]);
//	Result.r[1] = XMVector3Normalize(Result.r[1]);
//	Result.r[2] = XMVector3Normalize(Result.r[2]);
//
//	return Result;
//}

CBuffState* CBuffState::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBuffState* pInstance = new CBuffState(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBuffState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuffState::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CBuffState* pInstance = new CBuffState(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CBuffState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuffState::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pParentTransform);
	Safe_Release(m_Desc.pParentHP);
	Safe_Release(m_Desc.pParentAttack);
	Safe_Release(m_Desc.pParentMonFSM);
	Safe_Release(m_Desc.pParentModel);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pMaskTextureCom);
}

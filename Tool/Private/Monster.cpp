#include "stdafx.h"
#include "Monster.h"

#include "GameInstance.h"

#include "BuffState.h"

#include "HpBar.h"
#include "Searcher.h"

#include "Skill_Manager.h"
#include "Skill.h"

#include "Searcher.h"
#include "DamageText.h"

#include "MiscData.h"

#include "Utility.h"

#include "StageCamera.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_PokemonDesc.ModelPrototypeTag = (*(POKEMON_DESC*)(pArg)).ModelPrototypeTag;
		m_PokemonDesc.ModelPrototypeLevelIndex = (*(POKEMON_DESC*)(pArg)).ModelPrototypeLevelIndex;
		m_PokemonDesc.vPos = (*(POKEMON_DESC*)(pArg)).vPos;
		m_PokemonDesc.moveSpeed = (*(POKEMON_DESC*)(pArg)).moveSpeed;
		m_PokemonDesc.rotateSpeed = (*(POKEMON_DESC*)(pArg)).rotateSpeed;
		m_PokemonDesc.m_monsterNo = (*(POKEMON_DESC*)(pArg)).m_monsterNo;
		m_PokemonDesc.m_hpBasis = (*(POKEMON_DESC*)(pArg)).m_hpBasis;
		m_PokemonDesc.m_attackBasis = (*(POKEMON_DESC*)(pArg)).m_attackBasis;
		m_PokemonDesc.m_attackGrow = (*(POKEMON_DESC*)(pArg)).m_attackGrow;
		m_PokemonDesc.m_type1 = (*(POKEMON_DESC*)(pArg)).m_type1;
		m_PokemonDesc.m_type2 = (*(POKEMON_DESC*)(pArg)).m_type2;
		m_PokemonDesc.m_visitWeightDefault = (*(POKEMON_DESC*)(pArg)).m_visitWeightDefault;
		m_PokemonDesc.m_visitWeight = (*(POKEMON_DESC*)(pArg)).m_visitWeight;
		m_PokemonDesc.m_cookTableID = (*(POKEMON_DESC*)(pArg)).m_cookTableID;
		m_PokemonDesc.m_color = (*(POKEMON_DESC*)(pArg)).m_color;

		m_PokemonDesc.m_Rate = (*(POKEMON_DESC*)(pArg)).m_Rate;
		m_PokemonDesc.m_isLayer = (*(POKEMON_DESC*)(pArg)).m_isLayer;
		m_PokemonDesc.m_meleePercent = (*(POKEMON_DESC*)(pArg)).m_meleePercent;
		m_PokemonDesc.m_slotTypeWeightHp = (*(POKEMON_DESC*)(pArg)).m_slotTypeWeightHp;
		m_PokemonDesc.m_slotTypeWeightAttack = (*(POKEMON_DESC*)(pArg)).m_slotTypeWeightAttack;
		m_PokemonDesc.m_slotTypeWeightMulti = (*(POKEMON_DESC*)(pArg)).m_slotTypeWeightMulti;

		m_PokemonDesc.m_normalSkillType = (*(POKEMON_DESC*)(pArg)).m_normalSkillType;
		m_PokemonDesc.m_AIType = (*(POKEMON_DESC*)(pArg)).m_AIType;

		m_PokemonDesc.m_skillIDs = (*(POKEMON_DESC*)(pArg)).m_skillIDs;
	}
	else
	{
		m_PokemonDesc.m_monsterNo = 6;
		m_PokemonDesc.vPos = _float4(rand() % 10 + 12.f, 0.f, rand() % 10 + 19.f, 1.f);
	}

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_MotionState()))
		return E_FAIL;

	if (FAILED(Add_DamageText()))
		return E_FAIL;

	if (FAILED(Add_BuffState()))
		return E_FAIL;

	if (FAILED(Add_HpBar()))
		return E_FAIL;

	if (FAILED(Add_Searcher()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	m_pTransformCom->Set_Pos(m_PokemonDesc.vPos.x, m_PokemonDesc.vPos.y, m_PokemonDesc.vPos.z);

	m_pNavigationCom->Set_Index_By_Position({ m_PokemonDesc.vPos.x, m_PokemonDesc.vPos.y, m_PokemonDesc.vPos.z });

	Add_TransitionRandomState();

	m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1);

	return S_OK;
}

HRESULT CMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Load_By_Json_PreAddComponents()))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	m_pTransformCom->Set_TransforDesc({ m_PokemonDesc.moveSpeed, XMConvertToRadians(180.0f) });

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	if (FAILED(Add_MotionState()))
		return E_FAIL;

	if (FAILED(Add_BuffState()))
		return E_FAIL;

	if (FAILED(Add_DamageText()))
		return E_FAIL;

	if (FAILED(Add_HpBar()))
		return E_FAIL;

	if (FAILED(Add_Searcher()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	m_pTransformCom->Set_Pos(m_PokemonDesc.vPos.x, m_PokemonDesc.vPos.y, m_PokemonDesc.vPos.z);

	m_pNavigationCom->Set_Index_By_Position({ m_PokemonDesc.vPos.x, m_PokemonDesc.vPos.y, m_PokemonDesc.vPos.z });
	
	Add_TransitionRandomState();

	m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1);

	return S_OK;
}

_uint CMonster::Tick(_double TimeDelta)
{
	//m_pMonFSM->Update_Component((_float)TimeDelta, m_pModelCom);

	m_pManualCollisionState->Tick(TimeDelta);
	
	for (auto& pBuffStates : m_buffStates)
		pBuffStates->Tick(TimeDelta);

	if (m_pHpBar)
		m_pHpBar->Tick(TimeDelta);

	if (m_pHPCom)
		m_pHPCom->Tick(TimeDelta);

	if (m_pDamageText)
		m_pDamageText->Tick(TimeDelta);

	CoolTimeCheck(TimeDelta);

	HitTimeCheck(TimeDelta);

	m_pAABB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());

	return State_Tick(TimeDelta);
}

_uint CMonster::LateTick(_double TimeDelta)
{
	HitCheck();

	if (true == CGameInstance::GetInstance()->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		for (auto& pBuffStates : m_buffStates)
			pBuffStates->LateTick(TimeDelta);

		if (m_pHpBar)
			m_pHpBar->LateTick(TimeDelta);

		if (m_pHPCom)
		{ 
			if (m_pHPCom->Is_DamageEvent())
			{
				m_pDamageText->Show_Damage(m_pHPCom->Get_DamageRecieved(), { 1.f, 0.14f, 0.0f, 1.f }, { 0.75f, 0.75f }, 0.f, { 0.f, 0.f });
			}
		}
		//
		if (m_pDamageText)
			m_pDamageText->LateTick(TimeDelta);

		m_pRendererCom->Add_RenderGroup(m_eRenderId, this);
		
		m_bBeCulling = false;
	}
	else
	{
		m_bBeCulling = true;
	}

	return _uint();
}

HRESULT CMonster::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/*if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;*/

		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (m_pMonFSM->Get_MotionState() == CMonFSM::MONSTER_STATE::DEAD_BOSS ||
			m_pMonFSM->Get_MotionState() == CMonFSM::MONSTER_STATE::DEAD_ROTATE)
		{
			m_pShaderCom->Begin(1);
		}
		else
		{
			if (m_bHitState)
				m_pShaderCom->Begin(2);
			else
				m_pShaderCom->Begin(0);
		}

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pAABB->Render();
	//m_pOBB->Render();
	//m_pSphere->Render();

	if (m_pNavigationCom)
		m_pNavigationCom->Render();

#endif // _DEBUG

	return S_OK;
}


HRESULT CMonster::Add_BuffState()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"MiscData");
	if (nullptr == pObject)
		return E_FAIL;

	CMiscData* pMiscData = dynamic_cast<CMiscData*>(pObject);
	if (nullptr == pMiscData)
		return E_FAIL;

	vector<CMiscData::BUFFSTATEMISC_DESC> buffStateMiscDesces = pMiscData->Get_BuffStateDesces();

	for (size_t i = 0; i < buffStateMiscDesces.size(); ++i)
	{
		CBuffState* pBuffState = nullptr;

		CBuffState::BUFFSTATE_DESC		BuffStateDesc;
		ZeroMemory(&BuffStateDesc, sizeof BuffStateDesc);

		BuffStateDesc.pParentTransform = m_pTransformCom;
		BuffStateDesc.pParentAttack = m_pAttackCom;
		BuffStateDesc.pParentHP = m_pHPCom;
		BuffStateDesc.pParentMonFSM = m_pMonFSM;

		XMStoreFloat4x4(&BuffStateDesc.PivotMatrix, m_pModelCom->Get_PivotMatrix());

		BuffStateDesc.m_fSizeX = buffStateMiscDesces[i].SizeX;
		BuffStateDesc.m_fSizeY = buffStateMiscDesces[i].SizeY;

		BuffStateDesc.m_fPositionX = buffStateMiscDesces[i].PositionX;
		BuffStateDesc.m_fPositinoY = buffStateMiscDesces[i].PositionY;
		BuffStateDesc.m_fPositinoZ = buffStateMiscDesces[i].PositionZ;

		lstrcpy(BuffStateDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_UI_ss_n_damagedown");

		BuffStateDesc.m_TextureLevelIndex = LEVEL_STATIC;

		pGameInstance->Clone_GameObject(L"Layer_BuffState", m_iLevelindex, TEXT("Prototype_GameObject_BuffState"), (CGameObject**)&pBuffState, &BuffStateDesc);
		if (nullptr == pBuffState)
			return E_FAIL;
		
		if (i == 0)
		{
			if (FAILED(pBuffState->Change_Texture(L"Prototype_Component_Texture_UI_ss_p_speedup")))
				return E_FAIL;
		}

		m_buffStates.push_back(pBuffState);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CMonster::Add_HpBar()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject =  pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"MiscData");
	if (nullptr == pObject)
		return E_FAIL;

	CMiscData* pMiscData = dynamic_cast<CMiscData*>(pObject);
	if (nullptr == pMiscData)
		return E_FAIL;

	CMiscData::HPBARMISC_DESC hpBarMisc_Desc = pMiscData->Get_HpBarMisc_Desc();

	CHpBar::HPBAR_DESC		HpDesc{};
	ZeroMemory(&HpDesc, sizeof HpDesc);

	HpDesc.pParent = m_pTransformCom;
	Safe_AddRef(m_pTransformCom);

	HpDesc.pParentHpCom = m_pHPCom;
	Safe_AddRef(m_pHPCom);

	XMStoreFloat4x4(&HpDesc.PivotMatrix, m_pModelCom->Get_PivotMatrix());

	HpDesc.m_fSizeX = hpBarMisc_Desc.SizeX;
	HpDesc.m_fSizeY = hpBarMisc_Desc.SizeY;

	HpDesc.m_fPositionX = hpBarMisc_Desc.PositionX;
	HpDesc.m_fPositinoY = hpBarMisc_Desc.PositionY;
	HpDesc.m_fPositinoZ = hpBarMisc_Desc.PositionZ;

	if (0 == Get_LayerTag().compare(L"Layer_Player"))
		HpDesc.m_vHpColor = hpBarMisc_Desc.playerColor;
	else
		HpDesc.m_vHpColor = hpBarMisc_Desc.enemyColor;

	lstrcpy(HpDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Window_Plane_Corner_Bar");

	HpDesc.m_TextureLevelIndex = LEVEL_STATIC;

	pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_HpBar"), (CGameObject**)&m_pHpBar, & HpDesc);
	if (nullptr == m_pHpBar)
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Add_Searcher()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSearcher::SEARCHER_DESC		searcherDesc{};

	searcherDesc.pParentTransformCom = m_pTransformCom;
	Safe_AddRef(searcherDesc.pParentTransformCom);

	if (m_PokemonDesc.m_layerType == LAYER_TYPE_PLAYER)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Searcher"), Get_Levelindex(), L"Layer_PlayerSearcher",
			(CGameObject**)&m_pSearcher, nullptr, &searcherDesc)))
				return E_FAIL;

	}
	else
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Searcher"), Get_Levelindex(), L"Layer_MonsterSearcher", 
			(CGameObject**)&m_pSearcher, nullptr, &searcherDesc)))
				return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMonster::Add_DamageText()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"MiscData");
	if (nullptr == pObject)
		return E_FAIL;
	CMiscData* pMiscData = dynamic_cast<CMiscData*>(pObject);
	if (nullptr == pMiscData)
		return E_FAIL;
	CMiscData::DAMAGETETXTMISC_DESC damageTextMisc_Desc = pMiscData->Get_DamageTextMisc_Desc();

	CDamageText::DAMAGETEXT_DESC		DamageDesc{};
	ZeroMemory(&DamageDesc, sizeof DamageDesc);

	DamageDesc.pParent = m_pTransformCom;
	Safe_AddRef(m_pTransformCom);

	XMStoreFloat4x4(&DamageDesc.PivotMatrix, m_pModelCom->Get_PivotMatrix());

	DamageDesc.m_vScale = { damageTextMisc_Desc.SizeX, damageTextMisc_Desc.SizeY };

	DamageDesc.m_fPositionX = damageTextMisc_Desc.PositionX;
	DamageDesc.m_fPositinoY = damageTextMisc_Desc.PositionY;
	DamageDesc.m_fPositinoZ = damageTextMisc_Desc.PositionZ;

	DamageDesc.m_vColor = _float4(1.f, 1.f, 1.f, 1.f);

	lstrcpy(DamageDesc.m_FontTag, L"Font_NanumBarunGothicBold");

	pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_DamageText"), (CGameObject**)&m_pDamageText, &DamageDesc);
	if (nullptr == m_pDamageText)
		return E_FAIL;

	return S_OK;
}

void CMonster::Do_Skill(_uint skillType, CMonFSM::MONSTER_STATE eMotion, const _tchar* pLayer)
{
	if (!m_bAttack)
	{
		m_pMonFSM->Transit_MotionState(eMotion, m_pModelCom);

		Do_Skill(skillType, pLayer);
	}
}

void CMonster::Do_Skill(_uint skillType, const _tchar* pLayer)
{
	if (!m_bAttack)
	{
		CGameObject* pManager = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
		if (nullptr == pManager)
			return;

		CSkill_Manager* pSkill_Mananger = dynamic_cast<CSkill_Manager*>(pManager);
		if (nullptr != pSkill_Mananger)
		{
			//  ���� ��ųŸ���� ����/������� �̹� �������̸�,
			//  �������� ������ ��Ÿ���� �ʱ�ȭ ��Ű��

			pSkill_Mananger->Do_Skill(pLayer, m_iLevelindex, skillType,
				m_pAttackCom->Get_AttackPower(), m_pTransformCom->Get_WorldMatrix_Matrix(), m_pModelCom, "effect00", m_pTransformCom, Search_NoAction_BuffState(skillType));
		}

		CGameObject* pStageCamera = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Camera", L"Main_Camera");
		if (nullptr != pStageCamera)
		{
			dynamic_cast<CStageCamera*>(pStageCamera)->Do_Skill_Zoom_In(this);
		}
	}
}

void CMonster::Do_Skill_After_Set_Motion(_uint skillType, const _tchar* pLayer)
{
	Do_Skill(skillType, pLayer);

	if (skillType <= 35 && skillType % 2 == 1)
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom); // ���Ÿ� ����
	}
	else if (skillType <= 35 && skillType % 2 == 0) // �ٰŸ� ����
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom);
	}
	else if (skillType == 50) // ���̵巳 ����
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::POKING, m_pModelCom);
	}
	else if (skillType == 72) // �õ���
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::POKING, m_pModelCom);
	}
	else if (skillType == 79) // ��ٹٶ�
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::POKING, m_pModelCom);
	}
	else if (skillType == 100) // ����
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_START, m_pModelCom);
	}
	else if (skillType == 164) // ����
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::BODYBLOW, m_pModelCom);
	}
	else if (skillType == 168) // �ް��� ��ġ
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ATK_SLE_NORMAL_START, m_pModelCom);
	}
	else if (skillType == 188) // ��������
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::POKING, m_pModelCom);
	}
	else
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::POKING, m_pModelCom);
	}
}

void CMonster::CoolTimeCheck(const _double& TimeDelta)
{
	if (m_SkillCoolTimeAcc <= m_SkillCoolTime)
	{
		m_SkillCoolTimeAcc += TimeDelta;
		if (m_SkillCoolTimeAcc >= m_SkillCoolTime)
		{
			m_bCanSkillAttack = true;
		}
	}

	if (m_AttackCoolTimeAcc <= m_AttackCoolTime)
	{
		m_AttackCoolTimeAcc += TimeDelta;
		if (m_AttackCoolTimeAcc >= m_AttackCoolTime)
		{
			m_bCanAttack = true;
		}
	}
}

void CMonster::HitTimeCheck(const _double& TimeDelta)
{
	if (m_hitTimeAcc <= m_hitTime)
	{
		m_hitTimeAcc += TimeDelta;
		if (m_hitTimeAcc >= m_hitTime)
		{
			m_bHitState = false;
		}
	}
}

void CMonster::HitCheck()
{
	CManualCollisionState::COLLISION_STATE collisionState = m_pManualCollisionState->Get_State();

	if (collisionState == CAABB::COLLISION_STATE::COLLISION_STATE_ENTER)
	{
		m_bHitState = true;
		m_hitTimeAcc = 0.0;
	}
}

_bool CMonster::Search_Target()
{
	if (nullptr == m_pSearcher)
		return false;
	if (CSearcher::COLLISION_STATE_ENTER == m_pSearcher->Get_Collision_State() || CSearcher::COLLISION_STATE_ON == m_pSearcher->Get_Collision_State())
	{
		return true;
	}
	return false;
}

void CMonster::SkillCoolTime_Start()
{
	m_bCanSkillAttack = false;
	m_bCanAttack = false;
	m_AttackCoolTimeAcc = 0.0;
	m_SkillCoolTimeAcc = 0.0;
}


HRESULT CMonster::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_PickingCube */
	if (FAILED(pGameInstance->Add_Component(CPickingCube::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		(CComponent**)&m_pPickingCube, nullptr)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(180.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	m_PokemonDesc.ModelPrototypeTag = L"Prototype_Component_Model_Pokemon_PM";
	m_PokemonDesc.ModelPrototypeTag += to_wstring((int)m_PokemonDesc.m_monsterNo);
	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_PokemonDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_MonFSM */
	if (FAILED(pGameInstance->Add_Component(CMonFSM::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_MonFSM"),
		(CComponent**)&m_pMonFSM, nullptr)))
		return E_FAIL;

	/* For.Com_ManualCollisionState */
	if (FAILED(pGameInstance->Add_Component(CManualCollisionState::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_ManualCollisionState"),
		(CComponent**)&m_pManualCollisionState, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(0.8f, 1.5f, 0.8f);
	ColliderDesc.vPosition = _float3(0.0f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	///* For.Com_OBB*/
	//ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	//ColliderDesc.vScale = _float3(1.2f, 2.0f, 1.2f);
	//ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	//if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_OBB, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
	//	(CComponent**)&m_pOBB, &ColliderDesc)))
	//	return E_FAIL;

	///* For.Com_Sphere */
	//ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	//ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	//ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	//if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_SPHERE, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
	//	(CComponent**)&m_pSphere, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iIndex = 1;
	if (FAILED(pGameInstance->Add_Component(CNavigation::familyId, this, m_iLevelindex, TEXT("Prototype_Component_Navigation"),
		(CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	CHP::HP_DESC hpDesc = {};
	hpDesc.m_MaxHp = m_PokemonDesc.m_hpBasis;
	hpDesc.bDeadAfterOwnerDead = false;
	if (FAILED(pGameInstance->Add_Component(CHP::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_HP"),
		(CComponent**)&m_pHPCom, &hpDesc)))
		return E_FAIL;

	CAttack::ATTACK_DESC attakDesc = {};
	attakDesc.m_AttackPower = m_PokemonDesc.m_attackBasis;
	if (FAILED(pGameInstance->Add_Component(CAttack::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Attack"),
		(CComponent**)&m_pAttackCom, &attakDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_PickingCube */
	if (FAILED(pGameInstance->Add_Component(CPickingCube::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		(CComponent**)&m_pPickingCube, nullptr)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	m_PokemonDesc.ModelPrototypeTag = L"Prototype_Component_Model_Pokemon_PM";
	m_PokemonDesc.ModelPrototypeTag += to_wstring((int)m_PokemonDesc.m_monsterNo);
	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_PokemonDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_MonFSM */
	if (FAILED(pGameInstance->Add_Component(CMonFSM::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_MonFSM"),
		(CComponent**)&m_pMonFSM, nullptr)))
		return E_FAIL;

	/* For.Com_ManualCollisionState */
	if (FAILED(pGameInstance->Add_Component(CManualCollisionState::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_ManualCollisionState"),
		(CComponent**)&m_pManualCollisionState, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(0.8f, 1.5f, 0.8f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId	, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	///* For.Com_OBB*/
	//ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	//ColliderDesc.vScale = _float3(1.2f, 2.0f, 1.2f);
	//ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	//if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_OBB, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
	//	(CComponent**)&m_pOBB, &ColliderDesc)))
	//	return E_FAIL;

	///* For.Com_Sphere */
	//ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	//ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	//ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	//if (FAILED(pGameInstance->Add_Component(FAMILY_ID_COLLISION_SPHERE, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
	//	(CComponent**)&m_pSphere, &ColliderDesc)))
	//	return E_FAIL;

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iIndex = 1;
	if (FAILED(pGameInstance->Add_Component(CNavigation::familyId, this, m_iLevelindex, TEXT("Prototype_Component_Navigation"),
		(CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	CHP::HP_DESC hpDesc = {};
	hpDesc.m_MaxHp = m_PokemonDesc.m_hpBasis;
	hpDesc.bDeadAfterOwnerDead = false;
	if (FAILED(pGameInstance->Add_Component(CHP::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_HP"),
		(CComponent**)&m_pHPCom, &hpDesc)))
		return E_FAIL;

	CAttack::ATTACK_DESC attakDesc = {};
	attakDesc.m_AttackPower = m_PokemonDesc.m_attackBasis;
	if (FAILED(pGameInstance->Add_Component(CAttack::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Attack"),
		(CComponent**)&m_pAttackCom, &attakDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderResources()
{
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (m_pMonFSM->Get_MotionState() == CMonFSM::MONSTER_STATE::DEAD_BOSS ||
		m_pMonFSM->Get_MotionState() == CMonFSM::MONSTER_STATE::DEAD_ROTATE)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor",
			&m_deadColor, sizeof(_float4))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor",
			&m_hitColor, sizeof(_float4))))
			return E_FAIL;
	}

	_float ratio = fabs(m_hitTimeAcc - m_hitTime) / m_hitTime;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Ratio",
		&ratio, sizeof(_float))))
		return E_FAIL;

	_float cameraFar = pGameInstance->Get_CameraFar();
	if (FAILED(m_pShaderCom->Set_RawValue("g_CameraFar",
		&cameraFar, sizeof(_float))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CBuffState* CMonster::Search_NoAction_BuffState(const _uint& skillType)
{
	if (m_buffStates.empty())
		return nullptr;

	for (auto& buffState : m_buffStates)
	{
		if (buffState->Get_CurSkillType() == skillType)
		{
			return buffState;
		}
		else
		{
			if (buffState->Get_CanBuffSet())
			{
				return buffState;
			}
		}
	}

	return nullptr;
}

void CMonster::Do_Skill_By_Index(_uint skillindex, const _tchar* pLayer)
{
	if (nullptr == pLayer)
		return;

	if (!m_bCanSkillAttack || m_PokemonDesc.m_skillIDs.size() <= 0 || m_PokemonDesc.m_skillIDs.size() <= skillindex)
	{
		return;
	}

	Do_Skill_After_Set_Motion(m_PokemonDesc.m_skillIDs[skillindex], pLayer);
}

_bool CMonster::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (m_pTransformCom)
	{
		Value PokemonDesc(kObjectType);
		{
			Value ModelPrototypeTag;
			string tag = convert.to_bytes(m_PokemonDesc.ModelPrototypeTag.c_str());
			ModelPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			PokemonDesc.AddMember("ModelPrototypeTag", ModelPrototypeTag, allocator);

			PokemonDesc.AddMember("ModelPrototypeLevelIndex", m_PokemonDesc.ModelPrototypeLevelIndex, allocator);

			Value vPos(kObjectType);
			{
				_float4 pos = {};
				XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				vPos.AddMember("x", pos.x, allocator);
				vPos.AddMember("y", pos.y, allocator);
				vPos.AddMember("z", pos.z, allocator);
				vPos.AddMember("w", pos.w, allocator);
			}
			PokemonDesc.AddMember("vPos", vPos, allocator);

			PokemonDesc.AddMember("moveSpeed", m_PokemonDesc.moveSpeed, allocator);
			PokemonDesc.AddMember("rotateSpeed", m_PokemonDesc.rotateSpeed, allocator);

			PokemonDesc.AddMember("m_monsterNo", m_PokemonDesc.m_monsterNo, allocator);
			PokemonDesc.AddMember("m_attackBasis", m_PokemonDesc.m_attackBasis, allocator);
			PokemonDesc.AddMember("m_hpGrow", m_PokemonDesc.m_hpGrow, allocator);
			PokemonDesc.AddMember("m_attackGrow", m_PokemonDesc.m_attackGrow, allocator);
			PokemonDesc.AddMember("m_type1", m_PokemonDesc.m_type1, allocator);
			PokemonDesc.AddMember("m_type2", m_PokemonDesc.m_type2, allocator);
			PokemonDesc.AddMember("m_visitWeightDefault", m_PokemonDesc.m_visitWeightDefault, allocator);
			PokemonDesc.AddMember("m_visitWeight", m_PokemonDesc.m_visitWeight, allocator);
			PokemonDesc.AddMember("m_cookTableID", m_PokemonDesc.m_cookTableID, allocator);
			PokemonDesc.AddMember("m_color", m_PokemonDesc.m_color, allocator);
			PokemonDesc.AddMember("m_Rate", m_PokemonDesc.m_Rate, allocator);
			PokemonDesc.AddMember("m_isLayer", m_PokemonDesc.m_isLayer, allocator);
			PokemonDesc.AddMember("m_meleePercent", m_PokemonDesc.m_meleePercent, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightHp", m_PokemonDesc.m_slotTypeWeightHp, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightAttack", m_PokemonDesc.m_slotTypeWeightAttack, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightMulti", m_PokemonDesc.m_slotTypeWeightMulti, allocator);
			PokemonDesc.AddMember("m_normalSkillType", m_PokemonDesc.m_normalSkillType, allocator);
			PokemonDesc.AddMember("m_AIType", m_PokemonDesc.m_AIType, allocator);
			PokemonDesc.AddMember("m_layerType", m_PokemonDesc.m_layerType, allocator);

			Value m_skillIDs(kArrayType);
			{
				for (size_t i = 0; i < m_PokemonDesc.m_skillIDs.size(); ++i)
				{
					m_skillIDs.PushBack(m_PokemonDesc.m_skillIDs[i], allocator);
				}
			}
			PokemonDesc.AddMember("m_skillIDs", m_skillIDs, allocator);

		}
		doc.AddMember("PokemonDesc", PokemonDesc, allocator);
	}

	return true;
}

_bool CMonster::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& PokemonDesc = doc["PokemonDesc"];

		string ModelPrototypeTag = PokemonDesc["ModelPrototypeTag"].GetString();
		m_PokemonDesc.ModelPrototypeTag = convert.from_bytes(ModelPrototypeTag);

		m_PokemonDesc.ModelPrototypeLevelIndex = PokemonDesc["ModelPrototypeLevelIndex"].GetUint();

		const Value& vPos = PokemonDesc["vPos"];
		m_pTransformCom->Set_Pos(vPos["x"].GetFloat(), vPos["y"].GetFloat(), vPos["z"].GetFloat());
		m_PokemonDesc.vPos.x = vPos["x"].GetFloat();
		m_PokemonDesc.vPos.y = vPos["y"].GetFloat();
		m_PokemonDesc.vPos.z = vPos["z"].GetFloat();
		m_PokemonDesc.vPos.w = vPos["w"].GetFloat();

		m_PokemonDesc.moveSpeed = PokemonDesc["moveSpeed"].GetFloat();
		m_PokemonDesc.rotateSpeed = PokemonDesc["rotateSpeed"].GetFloat();

		m_PokemonDesc.m_monsterNo = PokemonDesc["m_monsterNo"].GetUint();
		m_PokemonDesc.m_hpBasis = PokemonDesc["m_hpBasis"].GetUint();
		m_PokemonDesc.m_attackBasis = PokemonDesc["m_attackBasis"].GetUint();
		m_PokemonDesc.m_hpGrow = PokemonDesc["m_hpGrow"].GetUint();
		m_PokemonDesc.m_attackGrow = PokemonDesc["m_attackGrow"].GetUint();
		m_PokemonDesc.m_type1 = PokemonDesc["m_type1"].GetUint();
		m_PokemonDesc.m_type2 = PokemonDesc["m_type2"].GetUint();
		m_PokemonDesc.m_visitWeightDefault = PokemonDesc["m_visitWeightDefault"].GetUint();
		m_PokemonDesc.m_visitWeight = PokemonDesc["m_visitWeight"].GetUint();
		m_PokemonDesc.m_cookTableID = PokemonDesc["m_cookTableID"].GetUint();
		m_PokemonDesc.m_color = PokemonDesc["m_color"].GetUint();
		m_PokemonDesc.m_Rate = PokemonDesc["m_Rate"].GetUint();
		m_PokemonDesc.m_isLayer = PokemonDesc["m_isLayer"].GetUint();
		m_PokemonDesc.m_meleePercent = PokemonDesc["m_meleePercent"].GetUint();
		m_PokemonDesc.m_slotTypeWeightHp = PokemonDesc["m_slotTypeWeightHp"].GetUint();
		m_PokemonDesc.m_slotTypeWeightAttack = PokemonDesc["m_slotTypeWeightAttack"].GetUint();
		m_PokemonDesc.m_slotTypeWeightMulti = PokemonDesc["m_slotTypeWeightMulti"].GetUint();

		m_PokemonDesc.m_normalSkillType = PokemonDesc["m_normalSkillType"].GetUint();
		m_PokemonDesc.m_AIType = PokemonDesc["m_AIType"].GetUint();
		m_PokemonDesc.m_layerType = PokemonDesc["m_layerType"].GetUint();

		const Value& skillIDs = PokemonDesc["m_skillIDs"];
		for (SizeType i = 0; i < skillIDs.Size(); ++i)
		{
			m_PokemonDesc.m_skillIDs.push_back(skillIDs[i].GetInt());
		}
	}

	return true;
}

HRESULT CMonster::Load_By_Json_PreAddComponents()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Add_MotionState()
{
	if (nullptr == m_pMonFSM)
		return E_FAIL;

	for (_uint i = 0; i < CMonFSM::FORMATION_NORMAL; ++i)
	{
		m_pMonFSM->Add_MotionState(CMonFSM::MONSTER_STATE(i), i);
	}

	m_pMonFSM->Add_MotionState(CMonFSM::MONSTER_STATE::FORMATION_NORMAL, CMonFSM::MONSTER_STATE::IDLE_GROUND);
	m_pMonFSM->Add_MotionState(CMonFSM::MONSTER_STATE::FORMATION_RUN, CMonFSM::MONSTER_STATE::RUN_GOUND2);

	return S_OK;
}


void CMonster::Free()
{
	__super::Free();

	for (auto& pGameObject : m_buffStates)
		Safe_Release(pGameObject);

	m_buffStates.clear();

	Safe_Release(m_pHpBar);
	Safe_Release(m_pSearcher);
	Safe_Release(m_pDamageText);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCube);
	Safe_Release(m_pMonFSM);
	Safe_Release(m_pAABB);
	//Safe_Release(m_pOBB);
	//Safe_Release(m_pSphere);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pHPCom);
	Safe_Release(m_pAttackCom);
	Safe_Release(m_pManualCollisionState);
}

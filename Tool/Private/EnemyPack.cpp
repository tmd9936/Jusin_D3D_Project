#include "stdafx.h"
#include "EnemyPack.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

#include "StageEnemyMonster.h"

#include "StageProgressUI.h"

#include "StageClearUI.h"

CEnemyPack::CEnemyPack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CEnemyPack::CEnemyPack(const CEnemyPack& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEnemyPack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemyPack::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.m_Name = (*(ENEMY_PACK_DESC*)(pArg)).m_Name;
		m_Desc.m_registDatas = (*(ENEMY_PACK_DESC*)(pArg)).m_registDatas;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemyPack::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_Json()))
		return E_FAIL;

	if (FAILED(Create_EnemyPack()))
		return E_FAIL;

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	return S_OK;
}

_uint CEnemyPack::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CEnemyPack::LateTick(_double TimeDelta)
{
	Check_CanNextSpawn();

	return _uint();
}

HRESULT CEnemyPack::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

_bool CEnemyPack::Next_Spawn(_float3 vPosition, const _float& radius)
{
	vector<CStageEnemyMonster*>* currentPack = m_EnemyPack.at(m_NextEnemyPack);

	if (nullptr == currentPack)
		return false;

	_int index = 0;
	_float addRadion = 360.f / currentPack->size();
	for (auto& enemy : *currentPack)
	{
		if (nullptr != enemy)
		{
			CTransform* pTransform = enemy->Get_As<CTransform>();
			if (nullptr != pTransform)
			{
				_float3 vPos = vPosition;

				_float randAddRadian = XMConvertToRadians(index * addRadion);
				vPos.x += sin(randAddRadian) * radius;
				vPos.z += cos(randAddRadian) * radius;

				pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

				CNavigation* pNavigation = enemy->Get_As<CNavigation>();
				if (nullptr != pNavigation)
				{
					pNavigation->Set_Index_By_Position(vPos);
				}

				CHP* pHpCom = enemy->Get_As<CHP>();
				if (nullptr != pHpCom)
				{
					pHpCom->Set_CanGetDamage(true);
				}
			}
			enemy->Be_Spawn();
		}
		++index;
	}

	m_CanNextSpawn = false;

	return true;
}

void CEnemyPack::Check_CanNextSpawn()
{
	if (m_NextEnemyPack >= m_EnemyPack.size())
		return;

	if (true == m_CanNextSpawn)
		return;

	vector<CStageEnemyMonster*>* currentPack = m_EnemyPack.at(m_NextEnemyPack);

	if (nullptr == currentPack)
		return;

	for (auto& enemy : *currentPack)
	{
		if (false == enemy->Is_Dead())
		{
			return;
		}
	}

	++m_NextEnemyPack;
	Set_WaveProgress();
	m_CanNextSpawn = true;
}

_bool CEnemyPack::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Value EnemyPackDesc(kObjectType);
	{
		Value m_NameValue;
		string name = convert.to_bytes(m_Desc.m_Name);
		m_NameValue.SetString(name.c_str(), (SizeType)name.size(), allocator);
		EnemyPackDesc.AddMember("m_Name", m_NameValue, allocator);

		Value m_registDatas(kArrayType);
		for (size_t i = 0; i < m_Desc.m_registDatas.size(); ++i)
		{
			Value registData(kObjectType);
			{
				Value m_enemyFilePath;
				m_enemyFilePath.SetString(m_Desc.m_registDatas[i].m_enemyFilePath.c_str(), (SizeType)m_Desc.m_registDatas[i].m_enemyFilePath.size(), allocator);
				registData.AddMember("m_enemyFilePath", m_enemyFilePath, allocator);

				registData.AddMember("m_enemyNumber", m_Desc.m_registDatas[i].m_enemyNumber, allocator);
				registData.AddMember("m_isBoss", m_Desc.m_registDatas[i].m_isBoss, allocator);
				registData.AddMember("m_waveIndex", m_Desc.m_registDatas[i].m_waveIndex, allocator);
				registData.AddMember("m_setIndex", m_Desc.m_registDatas[i].m_setIndex, allocator);
				registData.AddMember("m_spawnMode", m_Desc.m_registDatas[i].m_spawnMode, allocator);
				registData.AddMember("m_progress", m_Desc.m_registDatas[i].m_progress, allocator);


			}
			m_registDatas.PushBack(registData, allocator);
		}
		EnemyPackDesc.AddMember("m_registDatas", m_registDatas, allocator);

	}
	doc.AddMember("EnemyPackDesc", EnemyPackDesc, allocator);

	return true;
}

_bool CEnemyPack::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	const Value& EnemyPackDesc = doc["EnemyPackDesc"];

	string m_Name = EnemyPackDesc["m_Name"].GetString();
	m_Desc.m_Name = convert.from_bytes(m_Name);

	const Value& m_registDatas = EnemyPackDesc["m_registDatas"].GetArray();
	assert(m_registDatas.IsArray());

	m_Desc.m_registDatas.reserve((size_t)m_registDatas.Size());

	for (SizeType i = 0; i < m_registDatas.Size(); ++i)
	{
		REGIST_DATA_DESC registDataDesc{};

		registDataDesc.m_enemyFilePath = m_registDatas[i]["m_enemyFilePath"].GetString();
		registDataDesc.m_enemyNumber = m_registDatas[i]["m_enemyNumber"].GetUint();
		registDataDesc.m_isBoss = m_registDatas[i]["m_isBoss"].GetBool();
		registDataDesc.m_waveIndex = m_registDatas[i]["m_waveIndex"].GetUint();
		registDataDesc.m_setIndex = m_registDatas[i]["m_setIndex"].GetUint();
		registDataDesc.m_spawnMode = m_registDatas[i]["m_spawnMode"].GetUint();
		registDataDesc.m_progress = m_registDatas[i]["m_progress"].GetFloat();

		m_Desc.m_registDatas.push_back(move(registDataDesc));
	}

	return true;
}

HRESULT CEnemyPack::Insert_In_Stage_Manager()
{
	CGameObject* pObject = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");
	if (nullptr == pObject)
		return E_FAIL;

	CStage_Manager* pStageManager = dynamic_cast<CStage_Manager*>(pObject);
	if (nullptr == pStageManager)
		return E_FAIL;

	pStageManager->Set_EnemyPack(this);

	return S_OK;
}

HRESULT CEnemyPack::Create_EnemyPack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	_uint setIndex = 0;

	vector<CStageEnemyMonster*>* pEnemys = new vector<CStageEnemyMonster*>;
	m_EnemyPack.push_back(pEnemys);

	for (size_t i = 0; i < m_Desc.m_registDatas.size(); ++i)
	{
		if (m_Desc.m_registDatas.at(i).m_setIndex != setIndex)
		{
			WAVE_PROGRESS_DESC waveDesc{};
			waveDesc.m_progress = m_Desc.m_registDatas.at(i - 1).m_progress;
			waveDesc.m_setIndex = setIndex;
			waveDesc.m_waveIndex = m_Desc.m_registDatas.at(i - 1).m_waveIndex;

			m_progresses.push_back(waveDesc);

			setIndex = m_Desc.m_registDatas.at(i).m_setIndex;
			pEnemys = new vector<CStageEnemyMonster*>;
			m_EnemyPack.push_back(pEnemys);
		}

		_uint enemyNumber = m_Desc.m_registDatas.at(i).m_enemyNumber;

		for (size_t j = 0; j < enemyNumber; ++j)
		{
			CStageEnemyMonster* pMonster = nullptr;
			if (false == m_Desc.m_registDatas.at(i).m_isBoss)
			{
				if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE,
					L"Layer_Monster", (CGameObject**)&pMonster, nullptr, (void*)m_Desc.m_registDatas[i].m_enemyFilePath.c_str(), CLONE_FILEPATH)))
					return E_FAIL;
			}
			else
			{
				if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE,
					L"Layer_Monster", (CGameObject**)&pMonster, L"Boss", (void*)m_Desc.m_registDatas[i].m_enemyFilePath.c_str(), CLONE_FILEPATH)))
					return E_FAIL;
			}

			pEnemys->push_back(pMonster);
		}
	}

	WAVE_PROGRESS_DESC waveDesc{};
	waveDesc.m_progress = m_Desc.m_registDatas.at(m_Desc.m_registDatas.size() - 1).m_progress;
	waveDesc.m_setIndex = setIndex;
	waveDesc.m_waveIndex = m_Desc.m_registDatas.at(m_Desc.m_registDatas.size() - 1).m_waveIndex;

	m_progresses.push_back(waveDesc);

	return S_OK;
}

void CEnemyPack::Set_WaveProgress()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_UI", L"StageProgressUI");
	if (nullptr == pObject)
		return;
	
	_uint waveIndex = m_progresses[m_NextEnemyPack-1].m_waveIndex;
	_float progress = m_progresses[m_NextEnemyPack-1].m_progress;

	_bool waveClear = false;
	waveClear = dynamic_cast<CStageProgressUI*>(pObject)->Set_Wave_Progress(waveIndex, progress);

	if (waveClear)
	{
		CGameObject* pStageClearUI = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageMessageInfo", L"StageClearUI");
		if (nullptr != pStageClearUI)
		{
			if (waveIndex < 2)
				dynamic_cast<CStageClearUI*>(pStageClearUI)->Open_Message(L"WAVE CLEAR!", 330.f);
			else
				dynamic_cast<CStageClearUI*>(pStageClearUI)->Open_Message(L"STAGE CLEAR!", 305.f);
		}
	}
}

HRESULT CEnemyPack::Add_Components()
{
	return S_OK;
}

HRESULT CEnemyPack::Add_Components_By_Json()
{
	return S_OK;
}

HRESULT CEnemyPack::SetUp_ShaderResources()
{
	return S_OK;
}

CEnemyPack* CEnemyPack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnemyPack* pInstance = new CEnemyPack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CEnemyPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemyPack::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CEnemyPack* pInstance = new CEnemyPack(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CEnemyPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemyPack::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CEnemyPack* pInstance = new CEnemyPack(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CEnemyPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CEnemyPack::Free()
{
	__super::Free();

	for (vector<CStageEnemyMonster*>* enemys : m_EnemyPack)
	{
		if (nullptr != enemys)
		{
			for (auto& enemy : *enemys)
			{
				Safe_Release(enemy);
			}
			Safe_Delete(enemys);
		}
	}

	m_EnemyPack.clear();
}

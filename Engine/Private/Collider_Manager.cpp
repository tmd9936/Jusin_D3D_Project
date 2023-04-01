#include "Collider_Manager.h"

#include "GameInstance.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(CCollider_Manager)

CCollider_Manager::CCollider_Manager()
{
}

void CCollider_Manager::Update_CollisionMgr(_uint iLevelIndex)
{
	for (auto& pair : m_ColLayers)
	{
		Collision_GroupUpdate(pair.first, pair.second, iLevelIndex);
	}
}

void CCollider_Manager::Collision_GroupUpdate(const _tchar* LeftTag, const _tchar* RightTag, _uint iLevelIndex)
{
	vector<CGameObject*> vecLeft, vecRight;

	CGameInstance::GetInstance()->Get_ObjectList<CCollider>(vecLeft, iLevelIndex, LeftTag);
	CGameInstance::GetInstance()->Get_ObjectList<CCollider>(vecRight, iLevelIndex, RightTag);

	map<ULONGLONG, _bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			if (vecLeft[i] == vecRight[j])
				continue;

			CCollider* pLeftCol = dynamic_cast<CCollider*>(vecLeft[i]->Get_As<CCollider>());
			CCollider* pRightCol = dynamic_cast<CCollider*>(vecRight[j]->Get_As<CCollider>());

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->Get_ID();
			ID.Right_id = pRightCol->Get_ID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert({ ID.ID, false });
				iter = m_mapColInfo.find(ID.ID);
			}

			_float fX(0.f), fY(0.f), fZ(0.f);

			if (Is_Collision(pLeftCol, pRightCol, &fX, &fY, &fZ))
			{
				if (iter->second) // 충돌했고 이미 충돌 true인 경우
				{
					if (vecLeft[i]->Is_Dead() || vecRight[j]->Is_Dead())
					{
						pLeftCol->On_CollisionExit(pRightCol, fX, fY, fZ);
						pRightCol->On_CollisionExit(pLeftCol, fX, fY, fZ);
						iter->second = false;
					}

					else
					{
						pLeftCol->On_Collision(pRightCol, fX, fY, fZ);
						pRightCol->On_Collision(pLeftCol, fX, fY, fZ);
					}
				}

				else // 충돌했는데 충돌 정보에 false로 기록된 경우
				{
					if (!vecLeft[i]->Is_Dead() && !vecRight[j]->Is_Dead())
					{
						pLeftCol->On_CollisionEnter(pRightCol, fX, fY, fZ);
						pRightCol->On_CollisionEnter(pLeftCol, fX, fY, fZ);
						iter->second = true;
					}
				}
			}

			else // 충돌하지 않은 경우
			{
				if (iter->second)
				{
					pLeftCol->On_CollisionExit(pRightCol, fX, fY, fZ);
					pRightCol->On_CollisionExit(pLeftCol, fX, fY, fZ);
					iter->second = false;
				}
			}
		}
	}

}

bool CCollider_Manager::Is_Collision(CCollider* pLeftCol, CCollider* pRightCol, _float* fX, _float* fY, _float* fZ)
{
	_bool result = pLeftCol->Collision(pRightCol);

	// 1차 충돌 검사 - AABB
	_vector vLeftMin = Get_Min_Vector(pLeftCol);
	_vector vRightMin = Get_Min_Vector(pRightCol);

	_vector vLeftMax = Get_Max_Vector(pLeftCol);
	_vector vRightMax = Get_Max_Vector(pRightCol);

	//if (Get_Max(XMVectorGetX(vLeftMin), XMVectorGetX(vRightMin)) > Get_Min(XMVectorGetX(vLeftMax), XMVectorGetX(vRightMax)))
	//	return false;

	//if (Get_Max(XMVectorGetY(vLeftMin), XMVectorGetY(vRightMin)) > Get_Min(XMVectorGetY(vLeftMax), XMVectorGetY(vRightMax)))
	//	return false;

	//if (Get_Max(XMVectorGetZ(vLeftMin), XMVectorGetZ(vRightMin)) > Get_Min(XMVectorGetZ(vLeftMax), XMVectorGetZ(vRightMax)))
	//	return false;

	if (result && (pLeftCol->Get_Type() == CCollider::TYPE_AABB || pLeftCol->Get_Type() == CCollider::TYPE_OBB || pRightCol->Get_Type() == CCollider::TYPE_AABB || pRightCol->Get_Type() == CCollider::TYPE_OBB))
	{
		// 밀어낼 정도를 계산하여 반환
		*fX = Get_Min(XMVectorGetX(vLeftMax), XMVectorGetX(vRightMax) - Get_Max(XMVectorGetX(vLeftMin), XMVectorGetX(vRightMin)));
		*fY = Get_Min(XMVectorGetY(vLeftMax), XMVectorGetY(vRightMax) - Get_Max(XMVectorGetY(vLeftMin), XMVectorGetY(vRightMin)));
		*fZ = Get_Min(XMVectorGetZ(vLeftMax), XMVectorGetZ(vRightMax) - Get_Max(XMVectorGetZ(vLeftMin), XMVectorGetZ(vRightMin)));
	}

	return result;
}

void CCollider_Manager::Get_Min_Max(CCollider* Col, _vector& vMin, _vector& vMax)
{
	XMVECTOR Min{}, Max{}, Corner{};

	if (nullptr == Col)
		return;

	if (Col->Get_Type() == CCollider::TYPE_AABB)
	{
		for (size_t i = 1; i < Col->Get_AABB()->CORNER_COUNT; ++i)
		{
			vMin = XMVectorMin(vMin, Corner);
			vMax = XMVectorMax(vMax, Corner);
		}
	}

	if (Col->Get_Type() == CCollider::TYPE_OBB)
	{
		for (size_t i = 1; i < Col->Get_OBB()->CORNER_COUNT; ++i)
		{
			vMin = XMVectorMin(vMin, Corner);
			vMax = XMVectorMax(vMax, Corner);
		}
	}
}

XMVECTOR CCollider_Manager::Get_Min_Vector(CCollider* Col)
{
	XMVECTOR Min{}, Corner{};

	if (nullptr == Col)
		return Min;

	if (Col->Get_Type() == CCollider::TYPE_AABB)
	{
		_float3 coners[8];
		Col->Get_AABB()->GetCorners(coners);
		for (size_t i = 1; i < Col->Get_AABB()->CORNER_COUNT; ++i)
		{
			Min = XMVectorMin(Min, XMLoadFloat3(&coners[i]));
		}
	}

	if (Col->Get_Type() == CCollider::TYPE_OBB)
	{
		_float3 coners[8];
		Col->Get_OBB()->GetCorners(coners);
		for (size_t i = 1; i < Col->Get_OBB()->CORNER_COUNT; ++i)
		{
			Min = XMVectorMin(Min, XMLoadFloat3(&coners[i]));
		}
	}

	return Min;
}

XMVECTOR CCollider_Manager::Get_Max_Vector(CCollider* Col)
{
	XMVECTOR Max{}, Corner{};

	if (nullptr == Col)
		return Max;

	if (Col->Get_Type() == CCollider::TYPE_AABB)
	{
		_float3 coners[8];
		Col->Get_AABB()->GetCorners(coners);
		for (size_t i = 1; i < Col->Get_AABB()->CORNER_COUNT; ++i)
		{
			Max = XMVectorMax(Max, XMLoadFloat3(&coners[i]));
		}
	}

	if (Col->Get_Type() == CCollider::TYPE_OBB)
	{
		_float3 coners[8];
		Col->Get_OBB()->GetCorners(coners);
		for (size_t i = 1; i < Col->Get_OBB()->CORNER_COUNT; ++i)
		{
			Max = XMVectorMax(Max, XMLoadFloat3(&coners[i]));
		}
	}

	return Max;
}

_float CCollider_Manager::Get_Min(_float fSour, _float fDest)
{
	_float fMin = fSour < fDest ? fSour : fDest;

	return fMin;
}

_float CCollider_Manager::Get_Max(_float fSour, _float fDest)
{
	_float fMax = fSour > fDest ? fSour : fDest;

	return fMax;
}


HRESULT CCollider_Manager::Add_Check_CollisionGroup(const _tchar* LeftLayerTag, const _tchar* RightLayerTag)
{
	for (auto& pair : m_ColLayers)
	{
		if ((!lstrcmp(pair.first, LeftLayerTag) && !lstrcmp(pair.second,RightLayerTag))
			|| (!lstrcmp(pair.second, LeftLayerTag) && !lstrcmp(pair.first, RightLayerTag)))
			return E_FAIL;
	}

	m_ColLayers.push_back({ LeftLayerTag, RightLayerTag });

	return S_OK;
}

//콜라이더 매니저 프리
void CCollider_Manager::Free(void)
{
	m_ColLayers.clear();
	m_mapColInfo.clear();
}

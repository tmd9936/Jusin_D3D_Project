#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{

}

CNavigation::CNavigation(const CNavigation& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG

}

HRESULT CNavigation::Initialize_Prototype_HFile(const _tchar* pNavigationData)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(pNavigationData, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3		vPoints[3];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG


	return S_OK;
}

HRESULT CNavigation::Initialize_Prototype_Json(const char* pNavigationData)
{
	FILE* fp = fopen(pNavigationData, "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return E_FAIL;
	}
	else
	{
		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		const Value& Cells = document["Cells"];
		assert(Cells.IsArray());

		_float3		vPoints[3];

		for (SizeType i = 0; i < Cells.Size(); ++i)
		{
			vPoints[0] = _float3(Cells[i]["PointA_X"].GetFloat(), Cells[i]["PointA_Y"].GetFloat(), Cells[i]["PointA_Z"].GetFloat());
			vPoints[1] = _float3(Cells[i]["PointB_X"].GetFloat(), Cells[i]["PointB_Y"].GetFloat(), Cells[i]["PointB_Z"].GetFloat());
			vPoints[2] = _float3(Cells[i]["PointC_X"].GetFloat(), Cells[i]["PointC_Y"].GetFloat(), Cells[i]["PointC_Z"].GetFloat());

			CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_Cells.size());
			if (nullptr == pCell)
				return E_FAIL;

			m_Cells.push_back(pCell);
		}

		fclose(fp);
		Safe_Delete_Array(readBuffer);
	}

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_NaviDesc, pArg, sizeof m_NaviDesc);

	return S_OK;
}

HRESULT CNavigation::Render()
{
#ifdef _DEBUG
	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &pPipeLine->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &pPipeLine->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	Safe_Release(pPipeLine);

	WorldMatrix._42 = 0.2f;

	_float4			vColor = _float4(0.f, 0.f, 1.f, 1.f);

	if (0 <= m_NaviDesc.iIndex)
	{
		vColor = _float4(1.f, 0.f, 0.f, 1.f);
		WorldMatrix._42 = 0.3f;
	}

	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	m_pShader->Begin(0);

	if (0 <= m_NaviDesc.iIndex)
	{
		m_Cells[m_NaviDesc.iIndex]->Render();
	}
	else
	{
		for (auto& pCell : m_Cells)
			pCell->Render();
	}

#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Change_Cell_Data_By_Json(const char* pNavigationData)
{
	Free_Cells();

	return Initialize_Prototype_Json(pNavigationData);
}

void CNavigation::Free_Cells()
{
	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
}

void CNavigation::Set_Index_By_Position(_float3 vPosition)
{
	for (int i = 0; i < m_Cells.size(); ++i)
	{
		_int		iNeighborIndex = i;
		if (true == m_Cells[iNeighborIndex]->isIn(XMLoadFloat3(&vPosition), iNeighborIndex))
		{
			m_NaviDesc.iIndex = iNeighborIndex;
			break;
		}
	}
}

void CNavigation::Set_Index_By_Position(_float4 vPosition)
{
	for (int i = 0; i < m_Cells.size(); ++i)
	{
		_int		iNeighborIndex = i;
		if (true == m_Cells[iNeighborIndex]->isIn(XMLoadFloat4(&vPosition), iNeighborIndex))
		{
			m_NaviDesc.iIndex = iNeighborIndex;
			break;
		}
	}
}

void CNavigation::Get_Cells_Point(vector<CELL_POINT_DESC>& points)
{
	points.reserve(m_Cells.size());
	for (auto& cell : m_Cells)
	{
		CELL_POINT_DESC point;
		point.vPointA = *cell->Get_Point(CCell::POINT_A);
		point.vPointB = *cell->Get_Point(CCell::POINT_B);
		point.vPointC = *cell->Get_Point(CCell::POINT_C);

		points.push_back(point);
	}
}

// 움직임이 발생 했을 때 네비게이션 안에 있는지 판단
// @result 움직인 장소에 움직임 가능한 Cell이 있다면 true, 아니면 false
// @param vPosition: 움직일 장소
_bool CNavigation::Move_OnNavigation(_fvector vPosition)
{
	if (m_NaviDesc.iIndex >= m_Cells.size())
		return false;

	_int		iNeighborIndex = { 0 };

	if (true == m_Cells[m_NaviDesc.iIndex]->isIn(vPosition, iNeighborIndex))
	{
		return true;
	}
	else /* 쎌 밖으로 움직였어. */
	{
		/* 나간 방향에 이웃이 있다면?*/
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, iNeighborIndex))
				{
					m_NaviDesc.iIndex = iNeighborIndex;
					break;
				}
			}

			return true;
		}

		/* 나간 방향에 이웃이 없다면?*/
		return false;
	}
}

_bool CNavigation::Move_OnNavigation_Sliding(_fvector vPosition, _fvector vLook, _vector& vOutSlidLook)
{
	if (m_NaviDesc.iIndex >= m_Cells.size())
		return false;

	_int		iNeighborIndex = { 0 };

	if (true == m_Cells[m_NaviDesc.iIndex]->isIn(vPosition, iNeighborIndex))
	{
		return true;
	}
	else /* 쎌 밖으로 움직였어. */
	{
		/* 나간 방향에 이웃이 있다면?*/
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, iNeighborIndex))
				{
					m_NaviDesc.iIndex = iNeighborIndex;
					break;
				}
			}

			return true;
		}

		vOutSlidLook = m_Cells[m_NaviDesc.iIndex]->Get_SlidePowerV2(vPosition, vLook);

		/* 나간 방향에 이웃이 없다면?*/
		return false;
	}
}

_bool CNavigation::Move_OnNavigation_Set_Y(_fvector vPosition, _float& fY)
{
	if (m_NaviDesc.iIndex >= m_Cells.size())
		return false;

	_int		iNeighborIndex = { 0 };

	_float3 ComputePosition{};
	XMStoreFloat3(&ComputePosition, vPosition);

	if (true == m_Cells[m_NaviDesc.iIndex]->isIn(vPosition, iNeighborIndex))
	{
		m_Cells[m_NaviDesc.iIndex]->Compute_Height(ComputePosition, fY);
		return true;
	}
	else /* 쎌 밖으로 움직였어. */
	{
		/* 나간 방향에 이웃이 있다면?*/
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, iNeighborIndex))
				{
					m_NaviDesc.iIndex = iNeighborIndex;
					m_Cells[m_NaviDesc.iIndex]->Compute_Height(ComputePosition, fY);
					break;
				}
			}

			return true;
		}

		/* 나간 방향에 이웃이 없다면?*/
		return false;
	}
}

HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}

		}
	}

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pNavigationData)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype_Json(pNavigationData)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(CGameObject* pOwner, void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG
}

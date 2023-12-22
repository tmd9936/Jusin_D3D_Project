#include "Utility.h"

#include "GameInstance.h"

HRESULT CUtility::Load_Args_Data(const _tchar* filePath, void* args, _uint size)
{
	if (nullptr == filePath || nullptr == args)
		return E_FAIL;

	HANDLE		hFile = CreateFile(filePath, GENERIC_READ,
		NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	ReadFile(hFile, args, size, &dwByte, NULL);

	CloseHandle(hFile);
	return S_OK;
}

void CUtility::Save_Matrix_in_json(Value& arrayValue, _float4x4& matrix, Document::AllocatorType& allocator)
{
	size_t index = 0;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			arrayValue.PushBack(matrix.m[i][j], allocator);
		}
	}
}

void CUtility::CollisionPushingOut(CCollider* pSour, CCollider* pDest, const _float& fX, const _float& fY, const _float& fZ,
	CTransform* pDestTransform, CNavigation* pDestNavigation)
{
	if (fX > fZ)
	{
		_vector vDestCenter = pDest->Get_Center();
		_vector vSourCenter = pSour->Get_Center();

		if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
		{
			//pOtherTransform->Move(0.f, 0.f, fZ * 0.0166f, pNavigationCom);
			pDestTransform->Move(0.f, 0.f, -fZ * 0.2f, pDestNavigation);
		}
		else
		{
			//pOtherTransform->Move(0.f, 0.f, -fZ * 0.0166f, pNavigationCom);
			pDestTransform->Move(0.f, 0.f, fZ * 0.2f, pDestNavigation);
		}
		//pDest->Tick(pDestTransform->Get_WorldMatrix_Matrix());
	}
	else
	{
		_vector vDestCenter = pDest->Get_Center();
		_vector vSourCenter = pSour->Get_Center();

		if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
		{
			//pOtherTransform->Move(fX * 0.0166f, 0.f, 0.f, pNavigationCom);
			pDestTransform->Move(-fX * 0.2f, 0.f, 0.f, pDestNavigation);
		}
		else
		{
			//pOtherTransform->Move(-fX * 0.0166f, 0.f, 0.f, pNavigationCom);
			pDestTransform->Move(fX * 0.2f, 0.f, 0.f, pDestNavigation);

		}
	}
}

void CUtility::CollisionPushingOutNormal(CCollider * pSour, CCollider * pDest, const _float & fX, const _float & fY, const _float & fZ,
	CTransform * pDestTransform, CNavigation * pDestNavigation)
{
	if (fX > fZ)
	{
		_vector vDestCenter = pDest->Get_Center();
		_vector vSourCenter = pSour->Get_Center();

		if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
		{
			//pOtherTransform->Move(0.f, 0.f, fZ * 0.0166f, pNavigationCom);
			pDestTransform->Move(0.f, 0.f, -fZ, pDestNavigation);
		}
		else
		{
			//pOtherTransform->Move(0.f, 0.f, -fZ * 0.0166f, pNavigationCom);
			pDestTransform->Move(0.f, 0.f, fZ, pDestNavigation);
		}
		//pDest->Tick(pDestTransform->Get_WorldMatrix_Matrix());
	}
	else
	{
		_vector vDestCenter = pDest->Get_Center();
		_vector vSourCenter = pSour->Get_Center();

		if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
		{
			//pOtherTransform->Move(fX * 0.0166f, 0.f, 0.f, pNavigationCom);
			pDestTransform->Move(-fX, 0.f, 0.f, pDestNavigation);
		}
		else
		{
			//pOtherTransform->Move(-fX * 0.0166f, 0.f, 0.f, pNavigationCom);
			pDestTransform->Move(fX, 0.f, 0.f, pDestNavigation);

		}
	}
	
}

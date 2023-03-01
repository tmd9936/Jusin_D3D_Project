#include "Calculator.h"

#include "GameInstance.h"

CCalculator::CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CCalculator::CCalculator(const CCalculator& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(_float3* pPos, const _float3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	if (dwVtxItv < 0 || dwCntX < 0 || dwCntZ < 0)
		return 0.f;

	if (nullptr == pPos)
		return 0.f;

	int pTerrainVtxPosSize = dwCntX * dwCntZ;

	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	if (dwIndex < 0 || (*pPos).z < 0.f || (*pPos).x < 0.f ||
		size_t(dwIndex + dwCntX + 1) >= pTerrainVtxPosSize)
		return 0.f;

	_float	fRatioX = pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x;
	_float	fRatioZ = pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z;

	_vector		Plane{};

	if (fRatioX > fRatioZ)
	{
		Plane = XMPlaneFromPoints(
			XMLoadFloat3(& pTerrainVtxPos[dwIndex + dwCntX]),
			XMLoadFloat3(& pTerrainVtxPos[dwIndex + dwCntX + 1]),
			XMLoadFloat3(& pTerrainVtxPos[dwIndex + 1]));
	}

	else
	{
		Plane = XMPlaneFromPoints(
			XMLoadFloat3(&pTerrainVtxPos[dwIndex + dwCntX]),
			XMLoadFloat3(&pTerrainVtxPos[dwIndex + 1]),
			XMLoadFloat3(&pTerrainVtxPos[dwIndex]));
	}

	//ax + by + cz + d

	//by = -ax - cz - d

	//y = (-ax - cz - d) / b

	_float fY = 0.f;
	_float4 resultPlane{};

	XMStoreFloat4(&resultPlane, Plane);
	if (resultPlane.y > 0.f)
		fY = (-resultPlane.x * pPos->x - resultPlane.z * pPos->z - resultPlane.w) / resultPlane.y;

	return fY;
}

_float3 CCalculator::Picking_OnTerrain(HWND hWnd, _float2 viewPortSize, const CVIBuffer_FlatTerrain* pTerrainBufferCom, const CTransform* pTerrainTransCom)
{
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, inverseWorldMatrix;
	XMFLOAT4 direction, origin, rayOrigin, rayDirection;

	_float4 vRayPos = CGameInstance::GetInstance()->Get_CamPosition();

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_float4	vMousePos{0.f, 0.f, 0.f, 0.f};

	D3D11_VIEWPORT ViewPort[1];
	ZeroMemory(ViewPort, sizeof(D3D11_VIEWPORT));
	UINT iNumViewPorts = { 1 };
	m_pContext->RSGetViewports(&iNumViewPorts, ViewPort);

	//vMousePos.x = ptMouse.x / (ViewPort[0].Width * 0.5f) - 1.f;
	//vMousePos.y = ptMouse.y / -(ViewPort[0].Height * 0.5f) + 1.f;
	//vMousePos.z = 0.f;

	// 마우스 커서 좌표를 -1에서 +1 범위로 이동합니다
	float pointX = ((2.0f * (float)ptMouse.x) / (float)ViewPort[0].Width) - 1.0f;
	float pointY = (((2.0f * (float)ptMouse.y) / (float)ViewPort[0].Height) - 1.0f) * -1.0f;

	projectionMatrix = CGameInstance::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	XMFLOAT4X4 projectionMatrix4;
	XMStoreFloat4x4(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	viewMatrix = CGameInstance::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);	

	XMFLOAT4X4 inverseViewMatrix4;
	XMStoreFloat4x4(&inverseViewMatrix4, inverseViewMatrix);

	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	origin.x = vRayPos.x;
	origin.y = vRayPos.y;
	origin.z = vRayPos.z;

	inverseWorldMatrix = pTerrainTransCom->Get_WorldMatrix_Inverse();
	XMStoreFloat4(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorldMatrix));
	XMStoreFloat4(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorldMatrix));

	// 광선 방향을 표준화합니다.
	XMStoreFloat4(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));

	_vector vRayOrigin = XMVectorSet(rayOrigin.x, rayOrigin.y, rayOrigin.z, 1.f);
	_vector vRayDiretion = XMVectorSet(rayDirection.x, rayDirection.y, rayDirection.z, 0.f);


	const _float3* pTerrainVtx = pTerrainBufferCom->Get_VtxPos();
	const _ulong	dwCntZ = pTerrainBufferCom->Get_VtxCntZ();
	const _ulong	dwCntX = pTerrainBufferCom->Get_VtxCntX();

	_ulong		dwVtxIdx[3];

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwCntX + j;

			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + dwCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			_float4 vtx1{ 0.f, 0.f, 0.f, 1.f };
			_float4 vtx2{ 0.f, 0.f, 0.f, 1.f };
			_float4 vtx3{ 0.f, 0.f, 0.f, 1.f };
			memcpy(&vtx1, &pTerrainVtx[dwVtxIdx[1]], sizeof _float3);
			memcpy(&vtx2, &pTerrainVtx[dwVtxIdx[2]], sizeof _float3);
			memcpy(&vtx3, &pTerrainVtx[dwVtxIdx[0]], sizeof _float3);


			if(TriangleTests::Intersects(vRayOrigin,
				vRayDiretion,
				XMLoadFloat4(&vtx1),
				XMLoadFloat4(&vtx2),
				XMLoadFloat4(&vtx3), fDist))
			{
				return _float3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[1]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}


			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			memcpy(&vtx1, &pTerrainVtx[dwVtxIdx[2]], sizeof _float3);
			memcpy(&vtx2, &pTerrainVtx[dwVtxIdx[0]], sizeof _float3);
			memcpy(&vtx3, &pTerrainVtx[dwVtxIdx[1]], sizeof _float3);

			if (TriangleTests::Intersects(vRayOrigin,
				vRayDiretion,
				XMLoadFloat4(&vtx1),
				XMLoadFloat4(&vtx2),
				XMLoadFloat4(&vtx3), fDist))
			{
				return _float3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _float3{ 0.f, 0.f, 0.f };
}

CGameObject* CCalculator::Picking_Cube_Object(HWND hWnd, const _tchar* pLayerTag)
{
	return nullptr;
}

CGameObject* CCalculator::Picking_UI_Object(HWND hWnd, const _tchar* pLayerTag)
{
	return nullptr;
}

_bool CCalculator::Picking_UI_Check(HWND hWnd, const _tchar* pLayerTag, _float3* _v)
{
	return _bool();
}

CGameObject* CCalculator::Picking_Environment_Object(HWND hWnd, const _tchar* pLayerTag, _uint iLevelindex)
{
	vector<CGameObject*> vecResults;
	vector<CGameObject*> vecObj;

	//CGameObject::World->Get_ObjectList<CRcTex>(vecObj, eLayerId);
	CGameInstance::GetInstance()->Get_ObjectList<CVIBuffer_Rect>(vecObj, iLevelindex, pLayerTag);
	
	if (vecObj.empty())
		return nullptr;

	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, inverseWorldMatrix;
	XMFLOAT4 direction, origin, rayOrigin, rayDirection;

	_float4 vRayPos = CGameInstance::GetInstance()->Get_CamPosition();

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_float4	vMousePos{ 0.f, 0.f, 0.f, 0.f };

	D3D11_VIEWPORT ViewPort[1];
	ZeroMemory(ViewPort, sizeof(D3D11_VIEWPORT));
	UINT iNumViewPorts = { 1 };
	m_pContext->RSGetViewports(&iNumViewPorts, ViewPort);

	//vMousePos.x = ptMouse.x / (ViewPort[0].Width * 0.5f) - 1.f;
	//vMousePos.y = ptMouse.y / -(ViewPort[0].Height * 0.5f) + 1.f;
	//vMousePos.z = 0.f;

	// 마우스 커서 좌표를 -1에서 +1 범위로 이동합니다
	float pointX = ((2.0f * (float)ptMouse.x) / (float)ViewPort[0].Width) - 1.0f;
	float pointY = (((2.0f * (float)ptMouse.y) / (float)ViewPort[0].Height) - 1.0f) * -1.0f;

	projectionMatrix = CGameInstance::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	XMFLOAT4X4 projectionMatrix4;
	XMStoreFloat4x4(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	viewMatrix = CGameInstance::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	XMFLOAT4X4 inverseViewMatrix4;
	XMStoreFloat4x4(&inverseViewMatrix4, inverseViewMatrix);

	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	origin.x = vRayPos.x;
	origin.y = vRayPos.y;
	origin.z = vRayPos.z;

	for (auto& pObj : vecObj)
	{
		_float4		vRayPosTemp{};
		_float4		vRayDirTemp{};

		CTransform* pTransCom = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, pObj));
		
		inverseWorldMatrix = pTransCom->Get_WorldMatrix_Inverse();
		XMStoreFloat4(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorldMatrix));
		XMStoreFloat4(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorldMatrix));

		// 광선 방향을 표준화합니다.
		XMStoreFloat4(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));

		_vector vRayOrigin = XMVectorSet(rayOrigin.x, rayOrigin.y, rayOrigin.z, 1.f);
		_vector vRayDiretion = XMVectorSet(rayDirection.x, rayDirection.y, rayDirection.z, 0.f);

		CVIBuffer_Rect* RcTex = dynamic_cast<CVIBuffer_Rect*>(CGameInstance::GetInstance()->Get_Component(CVIBuffer_Rect::familyId, pObj));

		const VTXTEX* pVtxTex = RcTex->Get_VertexBuffer();
		const FACEINDICES16* pIndex = RcTex->Get_IndexBuffer();

		if (pVtxTex == nullptr || pIndex == nullptr)
			return nullptr;

		_float		fU = 0.f, fV = 0.f, fDist = 0.f;

		for (size_t i = 0; i < 2; ++i)
		{
			_float4 vtx1{ 0.f, 0.f, 0.f, 1.f };
			_float4 vtx2{ 0.f, 0.f, 0.f, 1.f };
			_float4 vtx3{ 0.f, 0.f, 0.f, 1.f };
			memcpy(&vtx1, &pVtxTex[pIndex[i]._1].vPosition, sizeof _float3);
			memcpy(&vtx2, &pVtxTex[pIndex[i]._2].vPosition, sizeof _float3);
			memcpy(&vtx3, &pVtxTex[pIndex[i]._0].vPosition, sizeof _float3);

			if (TriangleTests::Intersects(vRayOrigin,
				vRayDiretion,
				XMLoadFloat4(&vtx1),
				XMLoadFloat4(&vtx2),
				XMLoadFloat4(&vtx3), fDist))
			{
				vecResults.push_back(pObj);
				break;
			}
		}
	}

	if (vecResults.empty())
		return nullptr;

	_vector vViewPos = XMLoadFloat4(&CGameInstance::GetInstance()->Get_CamPosition());

	sort(vecResults.begin(), vecResults.end(),
		[vViewPos](CGameObject* pSour, CGameObject* pDest) -> bool
		{
			CTransform* pSourTransCom = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, pSour));;
			CTransform* pDestTransCom = dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_Component(CTransform::familyId, pDest));;

			_vector vSourPos = {};
			_vector vDestPos = {};

			vSourPos = pSourTransCom->Get_State(CTransform::STATE_POSITION);
			vDestPos = pDestTransCom->Get_State(CTransform::STATE_POSITION);

			_float3 sourLength{};
			_float3 destLength{};

			XMStoreFloat3(&sourLength, XMVector3Length(vViewPos - vSourPos));
			XMStoreFloat3(&destLength, XMVector3Length(vViewPos - vDestPos));

			return sourLength.x > destLength.x;
		});

	return vecResults.front();
}

CCalculator* CCalculator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCalculator* pInstance = new CCalculator(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCalculator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCalculator::Clone(CGameObject * pOwner, void* pArg)
{
	CCalculator* pInstance = new CCalculator(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCalculator::Free()
{
	__super::Free();
}

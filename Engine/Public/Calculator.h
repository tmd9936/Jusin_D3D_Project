#pragma once
#include "Component.h"

BEGIN(Engine)

//class CUITex;
class CVIBuffer_Rect;
class CVIBuffer_FlatTerrain;
class CTransform;

class ENGINE_DLL CCalculator : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_PHYSICS_CALCULATOR;

protected:
	explicit CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CCalculator(const CCalculator& rhs, CGameObject* pOwner);
	virtual ~CCalculator() = default;

public:
	HRESULT					Ready_Calculator();
	_float					Compute_HeightOnTerrain(_float3* pPos, const _float3* pTerrainVtxPos, const _ulong& dwCntX = 129, const _ulong& dwCntZ = 129, const _ulong& dwVtxItv = 1);
	virtual _float			Compute_HeightOnTerrain() { return 0.f; };
	virtual _float			Compute_HeightOnTerrain(_float3* pOut) { return 0.f; };
	_float3					Picking_OnTerrain(HWND hWnd, _float2 viewPortSize, const CVIBuffer_FlatTerrain* pTerrainBufferCom, const CTransform* pTerrainTransCom);


	CGameObject* Picking_Cube_Object(HWND hWnd, const _tchar* pLayerTag);
	CGameObject* Picking_UI_Object(HWND hWnd, const _tchar* pLayerTag);
	_bool		 Picking_UI_Check(HWND hWnd, const _tchar* pLayerTag, _float3* _v);

	CGameObject* Picking_Environment_Object(HWND hWnd, const _tchar* pLayerTag, _uint iLevelindex);

public:
	static CCalculator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

public:
	virtual void		Free(void);
};

END
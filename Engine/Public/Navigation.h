#pragma once

#include "Component.h"

BEGIN(Engine)

class CCell;

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int	iIndex = -1;
	}NAVIDESC;

public:
	static const FamilyId familyId = FAMILY_ID_NAVIGATION;

private:
	explicit CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject* pOwner);
	explicit CNavigation(const CNavigation& rhs, CGameObject* pOwner);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Prototype_HFile(const _tchar* pNavigationData);
	virtual HRESULT Initialize_Prototype_Json(const char* pNavigationData);
	virtual HRESULT Render();


public:
	HRESULT Change_Cell_Data_By_Json(const char* pNavigationData);
	void	Free_Cells();

	void	Set_Index_By_Position(_float3 vPosition);
	void	Set_Index_By_Position(_float4 vPosition);

public:
	void	Get_Cells_Point(vector<CELL_POINT_DESC>& points);

public:
	_bool Move_OnNavigation(_fvector vPosition);

	_bool Move_OnNavigation_Sliding(_fvector vPosition, _fvector vLook, _vector& vOutSlidLook, _vector& vAxis, _float& fY);

	_bool Move_OnNavigation_Set_Y(_fvector vPosition, _float& fY);

private:
	NAVIDESC				m_NaviDesc = {};
	vector<CCell*>			m_Cells;

#ifdef NAVIGATION_DEBUG
private:
	CShader* m_pShader = { nullptr };
#endif // _DEBUG

private:
	HRESULT SetUp_Neighbors();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pNavigationData);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free();

};

END
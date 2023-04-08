#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CFormation : public CComponent
{
public:
	typedef struct Formation_Desc
	{
		_vector m_RelativePos;
	} FORMATION_DESC;

public:
	static const FamilyId familyId = FAMILY_ID_ATTACK;
protected:
	explicit CFormation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CFormation(const CFormation& rhs, CGameObject* pOwner);
	virtual ~CFormation() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	const _vector		Get_RelativePos() const {
		return m_Desc.m_RelativePos;
	}

	void	Set_RelativePos(_fvector relativePos) {
		m_Desc.m_RelativePos = relativePos;
	}

	void	Swap_RelativePos(CFormation& target);

private:
	FORMATION_DESC		m_Desc = {};

public:
	static CFormation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END
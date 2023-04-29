#pragma once
#include "UI.h"

BEGIN(Client)

class CFood :
	public CUI
{
public:
	enum TYPE
	{
		TYPE_BLUE,
		TYPE_RED,
		TYPE_GREY,
		TYPE_YELLOW,
		TYPE_END
	};

public:
	enum STATE
	{
		STATE_PICKING,
		STATE_FOODBOX,
		STATE_STAGE_VIEW,
		STATE_NO_SHOW,
		STATE_END
	};

public:
	typedef struct Food_Desc
	{
		TYPE			m_foodType;
		STATE			m_ePreState;
		STATE			m_eCurState;

		UI_DESC			m_UIDesc;
	} FOOD_DESC;

private:
	explicit CFood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CFood(const CFood& rhs);
	virtual ~CFood() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint		Tick(_double TimeDelta) override;
	virtual _uint		LateTick(_double TimeDelta) override;
	virtual HRESULT		Render() override;

public:
	const TYPE				Get_FoodType() const {
		return m_Desc.m_foodType;
	}

private:
	void				Change_State();
	void				State_Tick(const _double& TimeDelta);

private:
	FOOD_DESC			m_Desc = {};

private:
	_uint				m_maskTextureIndex = { 0 };

public:
	static CFood* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END
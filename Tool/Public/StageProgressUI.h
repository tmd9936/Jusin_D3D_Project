#pragma once
#include "UI.h"

BEGIN(Client)

class CStage_Manager;

class CStageProgressUI :
	public CUI
{
public:
	typedef struct Stage_Progress_UI_Desc
	{
		_uint		m_waveCount;
	}STAGE_PROGRESS_UI_DESC;

private:
	explicit CStageProgressUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageProgressUI(const CStageProgressUI& rhs);
	virtual ~CStageProgressUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	void	Set_Wave_Progress(const _uint& waveIndex, _float progress);

private:
	HRESULT			Insert_In_Stage_Manager();

private:
	STAGE_PROGRESS_UI_DESC		m_desc = {};


public:
	static CStageProgressUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;

	virtual void Free() override;
};

END
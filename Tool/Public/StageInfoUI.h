#pragma once
#include "UI.h"

BEGIN(Client)

class CStageInfoUI :
    public CUI
{
public:
	typedef struct Stage_Info_Desc
	{
		_float4 m_vBaseColor;
		_uint	m_StageNumber;
		_tchar	 m_vStageNameText[MAX_PATH];
	} STAGE_INFO_DESC;

private:
	explicit CStageInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageInfoUI(const CStageInfoUI& rhs);
	virtual ~CStageInfoUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Change_Stage_Info(STAGE_INFO_DESC desc);

	void	Change_Stage_Info(_float4 vBaseColor, _uint stageNumber, wstring nameText);


public:
	static CStageInfoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera_Public.h"

BEGIN(Engine)
class CCamera;
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;

END

BEGIN(Client)

class CStageInfoUI;
class CStagePoint;
class CLevel_Loading;

class CWorldMap_Manager final : public CGameObject
{
public:
	enum WORLDMAP_MANAGER_STATE {
		MANAGER_IDLE,
		MANAGER_OPEN_STATE_INFO,
		MANAGER_CAMERA_FADE_OUT,
		MANAGET_STAGE_SELECT,
		MANAGER_END
	};

public:
	typedef struct WorldMap_Manager_Desc
	{
		_float m_FadeSecond;
		_float4 m_FadeStartColor;
		_float4 m_FadeEndColor;

		_float	m_FadeInStopIntervalDistance;

		_float	m_FadeInCameraSpeed;

	} WORLDMAP_MANAGER_DESC;

private:
	CWorldMap_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorldMap_Manager(const CWorldMap_Manager& rhs);
	virtual ~CWorldMap_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Stage_Select() {
		m_eCurState = MANAGER_CAMERA_FADE_OUT;
	}

public:
	void	Be_Idle() {
		m_eCurState = MANAGER_IDLE;
	}

private:
	HRESULT Init_PlayerPosition();

private:
	void Fade_out(const _double& TimeDelta);

private:
	WORLDMAP_MANAGER_DESC		m_Desc = {};
	WORLDMAP_MANAGER_STATE		m_ePreState = { MANAGER_END };
	WORLDMAP_MANAGER_STATE		m_eCurState = { MANAGER_IDLE };

	CCamera_Public*				p_MainCamera = { nullptr };
	//CGameObject* m_pPickingObject = { nullptr };

private:
	CCalculator*				m_pCalculator = { nullptr };
	CTransform*					m_pTransformCom = { nullptr };
	CRenderer*					m_pRendererCom = { nullptr };
	CShader*					m_pShaderCom = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

private:
	_float						m_fX = { 0.f };
	_float						m_fY = { 0.f };
	_float						m_fSizeX = {0.f};
	_float						m_fSizeY = {0.f};

	_float4x4					m_ViewMatrix = {};
	_float4x4					m_ProjMatrix = {};

	_float4						m_vCurrentFadeColor = {};
	_double						m_fCurrentFadeTIme = { 0.f };

	_float4						m_FocusPosition = {};

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	void   State_Tick(const _double& TimeDelta);

	void   Change_State();
	void   Picking();
	// TODO: UI 숨기기-> 알파로 서서히 사라지게 하기, 마우스 움직이면 다시 알파로 서서히 나타나게하기
	// 이 기능은 몬스터 충돌 다 하고나서 다다음주 부터 정도?

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

public:
	static CWorldMap_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
#pragma once

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"

BEGIN(Engine)

class CTimer_Manager;
class CGraphic_Device;
class CLevel_Manager;
class CComponent_Manager;
class CSound_Manager;
class CLight_Manager;
class CCollider_Manager;
class CFont_Manager;
class CFrustum;
class CTarget_Manager;

class ENGINE_DLL CGameInstance :
    public CBase
{
    DECLARE_SINGLETON(CGameInstance)

private:
    CGameInstance();
    virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, HINSTANCE hInstance, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	HRESULT Tick_Engine(_double TimeDelta);
	void Clear_Engine(_uint iLevelIndex);

public: /* For.Timer_Manager */
	HRESULT	Ready_Timer(const _tchar* pTimerTag);
	_double Get_TimeDelta(const _tchar* pTimerTag);
	void Set_TimeDelta(const _tchar* pTimerTag);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present(UINT interval, UINT flags);
	HRESULT SetRenderTargets(_bool bDepthSet = true);
	ID3D11Device* Get_Device();
	ID3D11DeviceContext* Get_ContextDevice();

	IDXGISwapChain* Get_SwapChain();
	ID3D11RenderTargetView* Get_RTV();

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	const _uint	Get_LevelIndex() const;
	const _bool	Get_LevelFirstInit(_uint m_iLevelIndex) const;

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectNameTag = nullptr, void* pArg = nullptr, CLONE_TYPE eCloneType = CLONE_ARGS); /* 원형을 복제하여 사본을 추가한다. */
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, CGameObject** ppOut, const _tchar* pObjectNameTag = nullptr, void* pArg = nullptr, CLONE_TYPE eCloneType = CLONE_ARGS);
	HRESULT Add_Component(const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Add_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	CComponent* Get_Component(const FamilyId& familyId, CGameObject* pObj) const;
	CComponent* Get_Component(const FamilyId& familyId, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const;
	HRESULT		Remove_Component(const FamilyId& familyId, CGameObject* pObj);
	CGameObject* Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const;
	CGameObject* Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, wstring objectTag) const;
	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	HRESULT Get_ComponentList(vector<T>& result, _uint iLevelIndex, const _tchar* pLayerTag);
	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	HRESULT Get_ObjectList(vector<CGameObject*>& result, _uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT	Get_Layer_Names(_uint iLevelIndex, vector<wstring>& vecNames);
	const unordered_map<const _tchar*, class CGameObject*>* Get_GameObject_Prototypes();
	HRESULT	Remove_All_GameObject_In_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT Get_All_GameObject_In_Layer(vector<CGameObject*>& result, _uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject* Clone_GameObject(const _tchar* pLayerTag, _uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	CGameObject* Clone_GameObject(const _tchar* pLayerTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CGameObject** ppOut, void* pArg);
	HRESULT		Layer_Tick_State_Change(const _tchar* pLayerTag, _uint iLevelIndex, _bool bTick);
	_bool		Is_Layer(_uint iLevelIndex, const wstring& layerTag);
	HRESULT		Change_Component(const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, class CGameObject* pGameObject, void* pArg = nullptr);
	_bool	Check_Prototype(const wstring prototypeTag);

public: /* For.Sound_Manager */
	HRESULT		Ready_Sound();
	void		PlaySoundW(const _tchar* pSoundKey, CHANNELID eID);
	void		PlayBGM(const _tchar* pSoundKey);
	void		StopSound(CHANNELID eID);
	void		StopAll();
	void		SetChannelVolume(CHANNELID eID, float fVolume);
	void		Set_SoundVolume(_float _fSoundVolume);
	void		Set_BGMVolume(_float _fMusicVolume) ;
	const _bool	Get_Sound_Ready_Finish();

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_float4x4 Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition();
	_matrix Get_ViewPort_Matrix(float x, float y, float w, float h, float minZ, float maxZ);
	_float Get_CameraFar();

public: /* For.InputDevice */
	const KEY_STATE Get_KeyState(KEY eKey);
	const MOUSE_STATE Get_MouseState(MOUSE eMouse);
	const _long Get_MouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID);

public: /*For.Light_Manager*/
	const LIGHTDESC* Get_Light(_uint iIndex);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);


public: /*For.Collider_Manager*/
	void Update_CollisionMgr(_uint iLevelIndex);
	HRESULT Add_Check_CollisionGroup(const _tchar* LeftLayerTag, const _tchar* RightLayerTag);
	void Reset_CollisionGroup();

public: /*For.Font_Manager*/
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vRotationOrigin = _float2(0.f, 0.f), const _float2& vScale = _float2(1.f, 1.f));

public: /*For.Frustum*/
	_bool  Is_In_Frustum(_fvector vPosition, _float fRange = 0.f);

public: /* For.Target_Manager */
	HRESULT Set_Shader_RTV(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);


private:
	CTimer_Manager*			m_pTimer_Manager = { nullptr };
	CGraphic_Device*		m_pGraphic_Device = { nullptr };
	CLevel_Manager*			m_pLevel_Manager = { nullptr };
	CObject_Manager*		m_pObject_Manager = { nullptr };
	CComponent_Manager*		m_pComponent_Manager = { nullptr };
	CSound_Manager*			m_pSound_Manager = { nullptr };
	CPipeLine*				m_pPipeLine = { nullptr };
	CInput_Device*			m_pInput_Device = { nullptr };
	CLight_Manager*			m_pLight_Manager = { nullptr };
	CCollider_Manager*		m_pCollider_Manager = { nullptr };
	CFont_Manager*			m_pFont_Manager = { nullptr };
	CFrustum*				m_pFrustum = { nullptr };
	CTarget_Manager*		m_pTarget_Manager = { nullptr };

public:
	static void Release_Engine();

public:
	virtual void Free(void) override;

};

template<typename T, typename>
HRESULT CGameInstance::Get_ComponentList(vector<T>& result, _uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Get_ComponentList<T>(result, iLevelIndex, pLayerTag);
}

template<typename T, typename>
HRESULT CGameInstance::Get_ObjectList(vector<CGameObject*>& result, _uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Get_ObjectList<T>(result, iLevelIndex, pLayerTag);
}

END


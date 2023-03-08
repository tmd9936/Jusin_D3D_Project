#include "..\Public\GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Component_Manager.h"
#include "Sound_Manager.h"
#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pSound_Manager(CSound_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pInput_Device);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, HINSTANCE hInstance, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 엔진을 사용하기위해 필수적으로 초기화가 필요한 매니져들의 기타 초기화작업을 수행한다. */
	/* 그래픽 디바이스 초기화. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iViewSizeX, GraphicDesc.iViewSizeY, ppDeviceOut, ppContextOut)))
		return E_FAIL;

	/* 다렉인풋 디바이스 초기화. */
	if (FAILED(m_pInput_Device->Ready_DInput(hInstance, GraphicDesc.hWnd)))
		return E_FAIL;

	/* 오브젝트 매니져의 공간 예약. */
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 컴포넌트 매니져의 공간 예약. */
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 레벨 매니져의 공간 예약. */
	if (FAILED(m_pLevel_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGameInstance::Tick_Engine(_double TimeDelta)
{
	/* 입력장치의 정보 받아오기.  */
	m_pInput_Device->Update_DInput();

	/* 레벨 매니져의 업데이트 */
	m_pLevel_Manager->Tick_Level(TimeDelta);

	/* 오브젝트 매니져의 업데이트 */
	m_pObject_Manager->Tick(TimeDelta);

	m_pPipeLine->Update();

	m_pObject_Manager->LateTick(TimeDelta);

	return S_OK;
}

void CGameInstance::Clear_Engine(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	m_pObject_Manager->Clear(iLevelIndex);
	m_pComponent_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Ready_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

_double CGameInstance::Get_TimeDelta(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0;

	return m_pTimer_Manager->Get_Timer(pTimerTag);
}

void CGameInstance::Set_TimeDelta(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	return m_pTimer_Manager->Set_Timer(pTimerTag);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present(UINT interval, UINT flags)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present(interval, flags);
}

HRESULT CGameInstance::SetRenderTargets(_bool bDepthSet)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->SetRenderTargets(bDepthSet);
}

ID3D11Device* CGameInstance::Get_Device()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;
	return m_pGraphic_Device->Get_Device();
}

IDXGISwapChain* CGameInstance::Get_SwapChain()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;
	return m_pGraphic_Device->Get_SwapChain();
}

ID3D11RenderTargetView* CGameInstance::Get_RTV()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;
	return m_pGraphic_Device->Get_RTV();
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

const _uint CGameInstance::Get_LevelIndex() const
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_LevelIndex();
}

const _bool CGameInstance::Get_LevelFirstInit(_uint m_iLevelIndex) const
{
	if (nullptr == m_pLevel_Manager)
		return true;

	return m_pLevel_Manager->Get_LevelFirstInit(m_iLevelIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectNameTag, void* pArg, CLONE_TYPE eCloneType)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(pPrototypeTag, iLevelIndex, pLayerTag, pObjectNameTag, pArg, eCloneType);
}

HRESULT CGameInstance::Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, 
	const _tchar* pLayerTag, CGameObject** ppOut, const _tchar* pObjectNameTag, void* pArg, CLONE_TYPE eCloneType)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(pPrototypeTag, iLevelIndex, pLayerTag, ppOut, pObjectNameTag, pArg, eCloneType);
}

HRESULT CGameInstance::Add_Component(const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Component(familyId, pGameObject, iLevelIndex, pPrototypeTag, ppOut, pArg);
}

HRESULT CGameInstance::Add_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Layer(iLevelIndex, pLayerTag);
}

CComponent* CGameInstance::Get_Component(const FamilyId& familyId, CGameObject* pObj) const
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(familyId, pObj);
}

CComponent* CGameInstance::Get_Component(const FamilyId& familyId, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(familyId, iLevelIndex, pLayerTag, pObjectTag);
}

HRESULT CGameInstance::Remove_Component(const FamilyId& familyId, CGameObject* pObj)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Remove_Component(familyId, pObj);
}

CGameObject* CGameInstance::Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Object(iLevelIndex, pLayerTag, pObjectTag);
}

HRESULT CGameInstance::Remove_All_GameObject_In_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Remove_All_GameObject_In_Layer(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Get_All_GameObject_In_Layer(vector<const CGameObject*>& result, _uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Get_All_GameObject_In_Layer(result, iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Get_Layer_Names(_uint iLevelIndex, vector<wstring>& vecNames)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Get_Layer_Names(iLevelIndex, vecNames);
}

const unordered_map<const _tchar*, class CGameObject*>* CGameInstance::Get_GameObject_Prototypes()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject_Prototypes();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, CGameObject* pGameObject, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pGameObject, pArg);
}


HRESULT CGameInstance::Ready_Sound()
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Ready_Sound();
}

void CGameInstance::PlaySoundW(const _tchar* pSoundKey, CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->PlaySoundW(pSoundKey, eID);
}

void CGameInstance::PlayBGM(const _tchar* pSoundKey)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->PlayBGM(pSoundKey);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->SetChannelVolume(eID, fVolume);
}

void CGameInstance::Set_SoundVolume(_float _fSoundVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->Set_SoundVolume(_fSoundVolume);
}

void CGameInstance::Set_BGMVolume(_float _fMusicVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->Set_BGMVolume(_fMusicVolume);
}

const _bool CGameInstance::Get_Sound_Ready_Finish()
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->Get_Finished();
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4{0.f, 0.f, 0.f, 1.f};

	return m_pPipeLine->Get_CamPosition();
}

const KEY_STATE CGameInstance::Get_KeyState(KEY eKey)
{
	if (nullptr == m_pInput_Device)
		return KEY_STATE::NONE;

	return m_pInput_Device->Get_KeyState(eKey);
}

const MOUSE_STATE CGameInstance::Get_MouseState(MOUSE eMouse)
{
	if (nullptr == m_pInput_Device)
		return MOUSE_STATE::NONE;

	return m_pInput_Device->Get_MouseState(eMouse);
}

const _long CGameInstance::Get_MouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_MouseMove(eMouseMoveID);
}

const LIGHTDESC* CGameInstance::Get_Light(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_Light(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}


void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();

	CObject_Manager::GetInstance()->DestroyInstance();

	CComponent_Manager::GetInstance()->DestroyInstance();

	CLevel_Manager::GetInstance()->DestroyInstance();

	CSound_Manager::GetInstance()->DestroyInstance();

	CTimer_Manager::GetInstance()->DestroyInstance();

	CPipeLine::GetInstance()->DestroyInstance();

	CLight_Manager::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->DestroyInstance();

	CGraphic_Device::GetInstance()->DestroyInstance();
}

void CGameInstance::Free(void)
{
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
}

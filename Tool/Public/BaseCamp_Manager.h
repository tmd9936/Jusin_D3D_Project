#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CCamera;
class CCalculator;
END

BEGIN(Client)

class CBaseCamp_Manager final : public CGameObject
{
public:
	enum BASECAMP_MANAGER_STATE {
		MANAGER_IDLE,
		MANAGER_CAMERA_FOCUS_IN,
		MANAGER_CAMERA_FOCUS_STAY,
		MANAGER_CAMERA_FOCUS_OUT,
		MANAGER_END
	};

public:
	typedef struct BaseCamp_Manager_Desc
	{
		_float4 pokemonFocusOffset;
		_float	pokemonFocusLimitDistance;
		_float	hideCommonUiTime;
		_float	visitInterval;
		_float	lookTime;
		_float  pokemonFocusStayTime;
		
	} BASECAMP_MANAGER_DESC;

private:
	CBaseCamp_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseCamp_Manager(const CBaseCamp_Manager& rhs);
	virtual ~CBaseCamp_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Focus_In(const _double& TimeDelta);
	void Focus_Stay(const _double& TimeDelta);
	void Focus_Out(const _double& TimeDelta);
	
private:
	CCalculator*				m_pCalculator = { nullptr };

private:
	BASECAMP_MANAGER_DESC		m_Desc = {};
	BASECAMP_MANAGER_STATE		m_ePreState = { MANAGER_END };
	BASECAMP_MANAGER_STATE		m_eCurState = { MANAGER_IDLE };
	CCamera_Public*				p_MainCamera = { nullptr };
	CGameObject*				m_pPickingObject = { nullptr };

private:
	_float						m_CurrentLookTime = { 0.f };
	_float4						m_FocusPosition = {};

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	void   State_Tick(const _double& TimeDelta);

	void   Change_State();
	void   Picking();
	// TODO: UI �����-> ���ķ� ������ ������� �ϱ�, ���콺 �����̸� �ٽ� ���ķ� ������ ��Ÿ�����ϱ�
	// �� ����� ���� �浹 �� �ϰ��� �ٴ����� ���� ����?

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */

public:
	static CBaseCamp_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
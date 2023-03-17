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
	typedef struct BaseCamp_Manager_Desc
	{
		_float4 pokemonFocusOffset;
		_float	hideCommonUiTime;
		_float	visitInterval;
		_float	lookTime;
		
	} BASECAMP_MANAGER_DESC;

private:
	CBaseCamp_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseCamp_Manager(const CBaseCamp_Manager& rhs);
	virtual ~CBaseCamp_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	CCalculator*				m_pCalculator = { nullptr };

private:
	BASECAMP_MANAGER_DESC		m_Desc = {};
	CCamera_Public*				p_MainCamera = { nullptr };
	CGameObject*				m_pPickingObject = { nullptr };

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	void   Picking();

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

public:
	static CBaseCamp_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
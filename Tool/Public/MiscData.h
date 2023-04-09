#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CMiscData final : public CGameObject
{
public:
	typedef struct HpBarMisc_Desc
	{
		_float SizeX;
		_float SizeY;
		_float PositionX;
		_float PositionY;
		_float PositionZ;
		_float4 playerColor;
		_float4 enemyColor;
	} HPBARMISC_DESC;

	typedef struct Misc_Desc
	{
		HPBARMISC_DESC hpBarMiscDesc;
	} MISC_DESC;

private:
	CMiscData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiscData(const CMiscData& rhs);
	virtual ~CMiscData() = default;

public:
	virtual HRESULT Initialize_Prototype(const char* filePath); /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	HPBARMISC_DESC Get_HpBarMisc_Desc() const {
		return m_MiscDesc.hpBarMiscDesc;
	}

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

private:
	MISC_DESC		m_MiscDesc = {};
	string			m_filePath = {};

public:
	static CMiscData* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* filePath);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
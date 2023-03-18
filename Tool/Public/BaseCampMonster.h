#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CBaseCampMonster : public CMonster
{
private:
	CBaseCampMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseCampMonster(const CMonster& rhs);
	virtual ~CBaseCampMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Add_TransitionState() override;
	virtual _uint Change_State() override;
	virtual _uint State_Tick(const _double& TimeDelta) override;

public:
	static CBaseCampMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
	virtual void Free() override;

};

END


#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
class CBone;
class CAABB;
END

BEGIN(Client)

class CWeapon final : public CGameObject
{
public:
	enum COLLIDERTYPE { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END };

public:
	typedef struct WeaponDesc
	{
		CBone* pBonePtr = { nullptr };
		CTransform* pParent = { nullptr };
		_float4x4	PivotMatrix;

	}WEAPONDESC;
private:
	explicit CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr }; /* �θ������ ����ǥ��. */
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CAABB* m_pAABB = { nullptr };

	WEAPONDESC			m_WeaponDesc = {};

	_float4x4			m_FinalWorldMatrix = {}; /* �������� (�� ���� * �θ����) */

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */
	_matrix Remove_Scale(_fmatrix Matrix);

public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
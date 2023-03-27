#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class CAABB;
END

BEGIN(Client)

class CStagePoint final : public CGameObject
{
public:
	typedef struct Stage_Point_Desc
	{
		_float3		vPos;
		_uint		eState;
		_uint		stageNumber;
		_float4		vColor;
		wstring		levelTitleText;

	} STAGE_POINT_DESC;

public:
	enum STAGE_POINT_STATE
	{
		STAGE_POINT_STATE_NO_VIEW,
		STAGE_POINT_STATE_STANDARD,
		STAGE_POINT_STATE_CLEAR,
		STAGE_POINT_STATE_BOSS,
		STAGE_POINT_STATE_EFFECT_CLEAR,
		STAGE_POINT_STATE_EFFECT_APPEAR,
		STAGE_POINT_STATE_EFFECT_APPEAR_LAST,
		STAGE_POINT_STATE_END
	};

private:
	CStagePoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStagePoint(const CStagePoint& rhs);
	virtual ~CStagePoint() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);


	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */

private:
	CTransform*		m_pTransformCom = { nullptr };
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CPickingCube*	m_pPickingCube = { nullptr };
	CAABB*			m_pAABB = { nullptr };

private:
	STAGE_POINT_DESC		m_Desc = { };
	STAGE_POINT_STATE		m_eState = { STAGE_POINT_STATE_STANDARD };

public:
	static CStagePoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END
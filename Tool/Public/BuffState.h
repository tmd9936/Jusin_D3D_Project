#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
class CModel;
class CHp;
class CAttack;
class CMonFSM;
END

/*
���� Ÿ�Կ� �°� �÷��̾��� ������Ʈ�� �����ͼ� �����Ű��
�߰��� ü���� ������Ʈ�� ������ �ؽ�ó �ٲٱ�
����/����� �������� ���� ������ �Ⱥ��̱�
*/


/*
Transform ������Ʈ : �̵��ӵ� ���� �� ����
HP ������Ʈ : ���� �� ü�� ƽ������
Attack ������Ʈ : ���ݷ� ���� �� ����
MonFSM ������Ʈ : ���� ���� ��Ű��
*/

BEGIN(Client)

class CBuffState final : public CGameObject
{
public:
	enum BUFF_TYPE {
		BUFF_TYPE_NONE,
		BUFF_TYPE_DAMAGE_UP,
		BUFF_TYPE_DAMAGE_DOWN,
		BUFF_TYPE_DEFENSE_UP,
		BUFF_TYPE_DEFENSE_DOWN,
		BUFF_TYPE_SPEED_UP,
		BUFF_TYPE_SPEED_DOWN,
		BUFF_TYPE_RESIST_UP,
		BUFF_TYPE_RESIST_DOWN,
		BUFF_TYPE_DOKU,
		BUFF_TYPE_MAHI,
		BUFF_TYPE_KOORI,
		BUFF_TYPE_YAKEDO,
		BUFF_TYPE_KONRAN,
		BUFF_TYPE_KANASIBARI,
		BUFF_TYPE_NEMURI,
	};

public:
	typedef struct BuffState_Desc
	{
		CTransform*			pParentTransform = { nullptr }; // ������ �Ǵ� �θ�
		CHP*				pParentHP = { nullptr };
		CAttack*			pParentAttack = { nullptr };
		CMonFSM*			pParentMonFSM = { nullptr };

		_float4x4			PivotMatrix;

		_float				m_fPositionX;
		_float				m_fPositinoY;
		_float				m_fPositinoZ;

		_float				m_fSizeX;
		_float				m_fSizeY;

		_tchar				m_TextureProtoTypeName[MAX_PATH];
		_uint				m_TextureLevelIndex;

		BUFF_TYPE			m_eBuffType;

		_double				m_EndTime;

	} BUFFSTATE_DESC;
private:
	CBuffState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBuffState(const CBuffState& rhs);
	virtual ~CBuffState() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Change_Texture(const _tchar* prototypeTag);

private:
	CTransform*				m_pTransformCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CTexture*				m_pMaskTextureCom = { nullptr };

private:
	BUFFSTATE_DESC			m_Desc = {};
	_float4x4				m_FinalWorldMatrix = {};

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_double					m_CurEndTimeAcc = { 0.0 };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	_matrix Remove_Scale(_fmatrix Matrix);

public:
	static CBuffState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
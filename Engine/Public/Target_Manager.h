#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const _tchar* pTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	/// <summary>
	/// ���ε� �Ǿ��ִ� ����ۺ並 �ӽ� ������ �Ŀ� �ش� �±��� MRT�� ���� Ÿ������ ����
	/// </summary>
	/// <param name="pContext">����̽� ���ؽ�Ʈ  ��ü </param>
	/// <param name="pMRTTag">ã�� MRT�� �±�</param>
	/// <returns></returns>
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);

	/// <summary>
	/// ���ε� �Ǿ��ִ� MRT ��� ������ ���ε� �Ǿ��ִ� ����� �並 �ٽ� ���ε� ��
	/// </summary>
	/// <param name="pContext"></param>
	/// <returns></returns>
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

	/// <summary>
	/// �ش� Ÿ�ٿ��� ���̴� ���ҽ� �並 ���̴��� ���ε� ��û
	/// </summary>
	/// <param name="pTargetTag"> �ش� Ÿ���� �±� </param>
	/// <param name="pShader">���ε� �� ���̴�</param>
	/// <param name="pConstantName">���̴� �ȿ� �ִ� ������ ���ý�Ʈ �̸�</param>
	/// <returns></returns>
	HRESULT Set_ShaderResourceView(const _tchar* pTargetTag, class CShader* pShader,
		const char* pConstantName);

#ifdef RENDERTARGET_DEBUG
public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_MRT(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	/* ����Ÿ���� ���ε� �ϱ� ������ ���ε� �Ǿ��ִ� ����۸� �ӽ÷� �����ϱ� ����*/
	ID3D11RenderTargetView* m_pBackBufferView = { nullptr };
	/* ���� ���Ľ��� ������ �ִ����� ����ؾ���*/
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

	UINT					m_CurBeginRederTargetNum = { 0 };

private: /* ����Ÿ���� �����Ͽ� ������ ������ �Ѵ�. */
	unordered_map<const _tchar*, class CRenderTarget*>				m_RenderTargets;
	typedef unordered_map<const _tchar*, class CRenderTarget*>		RENDERTARGETS;

private:
	/* ������ ����Ÿ�ٵ� ��, ���ÿ� ���ε� �Ǿ���� ����Ÿ�ٵ��� �̸� ������´�. 
	������ ���ε��� ���� �ϴ� ������ ���� ����Ÿ���� �ٸ� ����Ÿ�� ����Ʈ�� ������ �� �� ����*/
	unordered_map<const _tchar*, list<class CRenderTarget*>>			m_MRTs;
	typedef unordered_map<const _tchar*, list<class CRenderTarget*>>	MRTS;

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END
#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"

#ifdef _DEBUG
#include "Shader.h"
#include "VIBuffer_Rect.h"
#endif // _DEBUG

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
	const _tchar* pTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(pDevice, pContext, iSizeX, iSizeY, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);
		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	/* ���ο� Ÿ�ٵ��� ���ε��ϱ����� ������ ���ε� �Ǿ��ִ� Ÿ��(������ϳ�����)�� ���� Ÿ���� ���´�. */
	pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	ID3D11RenderTargetView* pRenderTargets[8] = { nullptr };

	_uint		iNumRenderTargets = 0;

	/* ���� Ÿ���� Begin ������ Ŭ���� ���ִ°� ���� 
	End ������ Ŭ���� �Ѵٸ� �ⲯ �׷ȴµ� ����� ������ �� �����*/
	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	m_CurBeginRederTargetNum = iNumRenderTargets;

	/*����ڰ� ������ Ÿ�ٵ��� ��ġ�� ���ε��Ѵ�, 
	�߰��� ������ ���ε� �Ǿ��ִ� ���� ���ٽ� ������ ���� �ٽ� ���ε� �Ѵ�.*/ 
	pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pContext)
{
	/*https://rockeracer.tistory.com/58 �ؽ��� ���� ��� �ذ�*/
	ID3D11ShaderResourceView* pSRV[8] = { nullptr };
	pContext->PSSetShaderResources(0, m_CurBeginRederTargetNum, pSRV);

	pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::Set_ShaderResourceView(const _tchar* pTargetTag, CShader* pShader, const char* pConstantName)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Set_ShaderResourceView(pShader, pConstantName);
}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_MRT(const _tchar* pMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render(pShader, pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTag_Finder(pTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _tchar* pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);

		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();
}

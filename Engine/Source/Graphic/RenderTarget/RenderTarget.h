#pragma once

#include <D3D11.h>
#include "Window/Window.h"
#include "Device/DirectX11Device.h"
#include "Graphic/Material/Texture.h"

class RenderTarget{
public:
	RenderTarget()
		: mpRenderTargetView(NULL)
		, mpTexture2D(NULL)
		, mpDepthStencilView(NULL)
	{
	}
	~RenderTarget()
	{
	}
	HRESULT Create(UINT Width, UINT Height)
	{
		HRESULT hr = S_OK;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = Width;
		tex_desc.Height = Height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		//�����_�[�^�[�Q�b�g�Ƃ��Ďg�p���V�F�[�_���\�[�X�Ƃ��ė��p
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;


		//�f�v�X�e�N�X�`���̏ꍇ
		//tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		//���̃����o�͏ȗ�
		hr = Device::mpd3dDevice->CreateTexture2D(&tex_desc, nullptr, &mpTexture2D);
		if (FAILED(hr))
			return hr;

		//Width�AHeight�͑S�����_�[�^�[�Q�b�g�ƃf�v�X�o�b�t�@���ׂē����ɂ���

		// RenderTargetView�쐬�@MRT�ɕK�v�Ȍ�
		hr = Device::mpd3dDevice->CreateRenderTargetView(mpTexture2D, nullptr, &mpRenderTargetView);
		if (FAILED(hr))
			return hr;
		// �f�v�X�o�b�t�@
		//Device::mpd3dDevice->CreateDepthStencilView(texture, nullptr, &dsview);

		ID3D11ShaderResourceView* pShaderResourceView;
		hr = Device::mpd3dDevice->CreateShaderResourceView(mpTexture2D, nullptr, &pShaderResourceView);
		if (FAILED(hr))
			return hr;

		hr = mTexture.Create(pShaderResourceView);
		if (FAILED(hr))
			return hr;

		ClearView();

		return S_OK;
	}
	HRESULT CreateDepth(UINT Width, UINT Height)
	{
		HRESULT hr = S_OK;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = Width;
		tex_desc.Height = Height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;


		//���̃����o�͏ȗ�
		hr = Device::mpd3dDevice->CreateTexture2D(&tex_desc, nullptr, &mpTexture2D);
		if (FAILED(hr))
			return hr;

		//Width�AHeight�͑S�����_�[�^�[�Q�b�g�ƃf�v�X�o�b�t�@���ׂē����ɂ���

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// �f�v�X�o�b�t�@
		hr = Device::mpd3dDevice->CreateDepthStencilView(mpTexture2D, &descDSV, &mpDepthStencilView);
		if (FAILED(hr))
			return hr;


		//ID3D11ShaderResourceView* pShaderResourceView;
		//hr = Device::mpd3dDevice->CreateShaderResourceView(mpTexture2D, nullptr, &pShaderResourceView);
		//if (FAILED(hr))
		//	return hr;
		//
		//mTexture.Create(shared_ptr<ID3D11ShaderResourceView>(pShaderResourceView));

		ClearView();

		return S_OK;
	}

	HRESULT CreateBackBuffer(UINT Width, UINT Height)
	{
		HRESULT hr = S_OK;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = Device::mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		//�����_�[�^�[�Q�b�g�̍쐬
		hr = Device::mpd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &mpRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;


		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = Width;
		descDepth.Height = Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//descDepth.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		//descDepth.MiscFlags = D3D11_RESOURCE_MISC_SHARED;


		hr = Device::mpd3dDevice->CreateTexture2D(&descDepth, NULL, &mpTexture2D);
		if (FAILED(hr))
			return hr;


		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = Device::mpd3dDevice->CreateDepthStencilView(mpTexture2D, &descDSV, &mpDepthStencilView);
		if (FAILED(hr))
			return hr;



		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��
		Device::mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)Width;
		vp.Height = (FLOAT)Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		Device::mpImmediateContext->RSSetViewports(1, &vp);


		//ID3D11ShaderResourceView* pShaderResourceView;
		//hr = Device::mpd3dDevice->CreateShaderResourceView(mpTexture2D, nullptr, &pShaderResourceView);
		//if (FAILED(hr))
		//	return hr;

		//hr = mTexture.Create(pShaderResourceView);
		//if (FAILED(hr))
		//	return hr;

		ClearView();

		return S_OK;
	}

	void ClearView() const{
		//
		// Clear the back buffer
		//
		float ClearColor[4] = { 0.125f, 0.125f, 0.125f, 1.0f };
		if (mpRenderTargetView)Device::mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, ClearColor);
	}
	void ClearDepth() const{
		if (mpDepthStencilView)Device::mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	Texture& GetTexture(){
		return mTexture;
	}

	void PSSetShaderResource(UINT Slot) const{
		mTexture.PSSetShaderResources(Slot);
	}

	void SetRendererTarget() const{
		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��
		Device::mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);
	}
	static void SetRendererTarget(UINT num, const RenderTarget* render, const RenderTarget* depth){

		_ASSERT(num<9);

		ID3D11RenderTargetView* r[8];
		for (UINT i = 0; i < num; i++){
			r[i] = render[i].mpRenderTargetView;
		}
		ID3D11DepthStencilView* d = depth ? depth->mpDepthStencilView : NULL;
		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��
		Device::mpImmediateContext->OMSetRenderTargets(num, r, d);
	}
	static void NullSetRendererTarget(){

		ID3D11RenderTargetView* r[8];
		for (UINT i = 0; i < 8; i++){
			r[i] = NULL;
		}
		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��
		Device::mpImmediateContext->OMSetRenderTargets(8, r, NULL);
	}

	void Release()
	{
		if (mpTexture2D){
			mpTexture2D->Release();
			mpTexture2D = NULL;
		}
		//ID3D11RenderTargetView* rt[8];
		//ID3D11DepthStencilView* ds;
		//Device::mpImmediateContext->OMGetRenderTargets(8, rt, &ds);

		if (mpDepthStencilView){
			//if (ds == mpDepthStencilView){
			//	NullSetRendererTarget();
			//}
			mpDepthStencilView->Release();
			mpDepthStencilView = NULL;
		}
		if (mpRenderTargetView){
			//for (int i = 0; i < 8; i++){
			//	if (rt[i] == mpRenderTargetView){
			//		NullSetRendererTarget();
			//	}
			//}
			mpRenderTargetView->Release();
			mpRenderTargetView = NULL;
		}

	}


	ID3D11RenderTargetView*	GetRT(){ return mpRenderTargetView; };
	ID3D11Texture2D* GetTexture2D(){ return mpTexture2D; };

private:
	ID3D11RenderTargetView*	mpRenderTargetView;
	ID3D11Texture2D*		mpTexture2D;
	ID3D11DepthStencilView*	mpDepthStencilView;
	Texture					mTexture;
};
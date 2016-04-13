#pragma once

#include "MySTL/Ptr.h"

#include "Game/Actor.h"
#include "Graphic/Model/Model.h"
#include "Window/Window.h"

#include "IComponent.h"

#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"

#include "Graphic/Shader/GeometryShader.h"
class ParticleComponent :public Component{
public:
#define NUM_PARTICLE 200
	struct ParticleVertex{
		XMFLOAT3 pos;
		XMFLOAT3 v0;
		FLOAT time;

	};

	ParticleComponent()
		:mFirstDraw(true){
		mInitialize = false;
		UINT stride[] = { sizeof(ParticleVertex) };

		D3D11_INPUT_ELEMENT_DESC layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �ʒu���W
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �����x
				{ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }   // ����
		};

		D3D11_SO_DECLARATION_ENTRY decl[] = {
				{ 0, "POSITION", 0, 0, 3, 0 },
				{ 0, "NORMAL", 0, 0, 3, 0 },
				{ 0, "TEXCOORD", 0, 0, 1, 0 },
		};
		mGeometryShader0.Create("EngineResource/ParticleGS.fx", "GS0_Main", decl, _countof(decl), stride, _countof(stride));
		mGeometryShader1.Create("EngineResource/ParticleGS.fx", "GS1_Main");
		mVertexShader.Create("EngineResource/ParticleGS.fx", "VS0_Main", layout, 3);
		mPixelShader.Create("EngineResource/ParticleGS.fx", "PS1_Main");


		// �p�[�e�B�N���̏����l��ݒ�
		ParticleVertex v[NUM_PARTICLE];
		for (int i = 0; i<NUM_PARTICLE; i++)
		{
			v[i].pos = XMFLOAT3(0.5f, 0.5f, 0.0f);
			v[i].v0 = XMFLOAT3((float)(rand() % 50 - 25) * 0.1f, (float)(rand() % 50) * 0.3f, (float)(rand() % 50 - 25) * 0.1f);
			v[i].time = (float)(rand() % 100) * 0.1f;
		};

		// �^�[�Q�b�g���o�̓o�b�t�@�[���쐬
		D3D11_BUFFER_DESC BufferDesc;
		::ZeroMemory(&BufferDesc, sizeof(BufferDesc));
		BufferDesc.ByteWidth = sizeof(v);  // �o�b�t�@�T�C�Y
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		BufferDesc.CPUAccessFlags = 0;
		BufferDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA mappedResource;
		mappedResource.pSysMem = v;  // �o�b�t�@�E�f�[�^�̏����l
		mappedResource.SysMemPitch = 0;
		mappedResource.SysMemSlicePitch = 0;
		for (int i = 0; i<2; i++)
		{
			auto hr = Device::mpd3dDevice->CreateBuffer(&BufferDesc, &mappedResource, &mpSOBuffer[i]);
			if (FAILED(hr))
				return;
		}
		mCBuffer = ConstantBuffer<CBChangesEveryFrame>::create(2);
		if (!mCBuffer.mBuffer)
			return;

		mInitialize = true;
	}
	~ParticleComponent(){
		mGeometryShader0.Release();
		mGeometryShader1.Release();
		mpSOBuffer[0]->Release();
		mpSOBuffer[1]->Release();
	}
	void Update() override{
		if (!mInitialize)return;
		if (!mMaterial){
			mMaterial = gameObject->GetComponent<MaterialComponent>();
			if (!mMaterial)return;
		}



		ID3D11DepthStencilState* pBackDS;
		UINT ref;
		Device::mpImmediateContext->OMGetDepthStencilState(&pBackDS, &ref);

		mMaterial->GetMaterial(0).PSSetShaderResources();

		// �^�[�Q�b�g�o�̓o�b�t�@�[�����ւ���
		ID3D11Buffer *pB = mpSOBuffer[0];
		mpSOBuffer[0] = mpSOBuffer[1];
		mpSOBuffer[1] = pB;

		// ���̓A�Z���u���[ �X�e�[�W�ɒ��_�o�b�t�@�[�̔z����o�C���h����B
		UINT stride = sizeof(ParticleVertex);
		UINT offset = 0;
		Device::mpImmediateContext->IASetVertexBuffers(0, 1, &mpSOBuffer[0], &stride, &offset);

		Device::mpImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		// �s�N�Z���V�F�[�_�[�𖳌��ɂ�����ԂŐ[�x�X�e���V����L���ɂ��Ă���ƃG���[�ɂȂ�̂Ŗ����ɂ���
		//ID3D11DepthStencilState*  pDepthStencilState = NULL;
		//D3D11_DEPTH_STENCIL_DESC ddsDesc;
		//UINT StencilRef;
		//::ZeroMemory(&ddsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		//Device::mpImmediateContext->OMGetDepthStencilState(&pDepthStencilState, &StencilRef);
		//pDepthStencilState->GetDesc(&ddsDesc);
		//ddsDesc.DepthEnable = FALSE;  // �[�x�e�X�g�𖳌��ɂ���
		//ddsDesc.StencilEnable = FALSE;
		//pDepthStencilState->Release();
		//Device::mpd3dDevice->CreateDepthStencilState(&ddsDesc, &pDepthStencilState);
		//Device::mpImmediateContext->OMSetDepthStencilState(pDepthStencilState, StencilRef);
		//pDepthStencilState->Release();

		D3D11_DEPTH_STENCIL_DESC descDS = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
		descDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDS.DepthFunc = D3D11_COMPARISON_LESS;
		descDS.DepthEnable = FALSE;
		descDS.StencilEnable = FALSE;

		ID3D11DepthStencilState* pDS = NULL;
		Device::mpd3dDevice->CreateDepthStencilState(&descDS, &pDS);
		if (!pDS)return;

		Device::mpImmediateContext->OMSetDepthStencilState(pDS, 0);


		//Device::mpImmediateContext->OMSetDepthStencilState(NULL, NULL);

		mVertexShader.SetShader();

		mGeometryShader0.SetShader();

		// �s�N�Z���V�F�[�_�[���f�o�C�X�ɖ����ɂ���B
		Device::mpImmediateContext->PSSetShader(NULL, NULL, 0);

		//Device::mpImmediateContext->VSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->HSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->DSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->GSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->PSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->CSSetShader(NULL, NULL, 0);

		// �o�̓o�b�t�@�[�̔z��̃I�t�Z�b�g�l
		// �p�C�v���C���̃X�g���[���o�̓X�e�[�W�Ŏg�p�����^�[�Q�b�g�o�̓o�b�t�@�[��ݒ肵�܂��B
		Device::mpImmediateContext->SOSetTargets(1, &mpSOBuffer[1], &offset);


		mCBuffer.mParam.mWorld = gameObject->mTransform->GetMatrix();
		mCBuffer.UpdateSubresource();
		mCBuffer.GSSetConstantBuffers();

		// �ŏ��͒��_�����킩��̂� ID3D11DeviceContext::Draw() �ŕ`�悷��
		if (mFirstDraw)
		{
			Device::mpImmediateContext->Draw(NUM_PARTICLE, 0);
			mFirstDraw = false;
		}
		// �Q��ڈڍs�͏������钸�_�����s���Ȃ̂� ID3D11DeviceContext::DrawAuto() �ŕ`�悷��
		else
		{
			Device::mpImmediateContext->DrawAuto();
		}

		ID3D11Buffer* pNullBuffer[] = { NULL };
		// �p�C�v���C���̃X�g���[���o�̓X�e�[�W�Ŏg�p�����^�[�Q�b�g�o�̓o�b�t�@�[�𖳌��ɂ���B
		Device::mpImmediateContext->SOSetTargets(1, pNullBuffer, NULL);


		mGeometryShader1.SetShader();
		mPixelShader.SetShader();

		Device::mpImmediateContext->DrawAuto();

		Device::mpImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Device::mpImmediateContext->GSSetShader(NULL, NULL, 0);

		pDS->Release();


		Device::mpImmediateContext->OMSetDepthStencilState(pBackDS, ref);
		if (pBackDS)pBackDS->Release();
	}
	void CreateInspector() override{

		auto data = Window::CreateInspector();
		Window::AddInspector(new InspectorLabelDataSet("Particle"), data);
		Window::ViewInspector("Particle", this, data);
	}

	void IO_Data(I_ioHelper* io) override{
		(void)io;
#define _KEY(x) io->func( x , #x)

#undef _KEY
	}


	bool mInitialize;
	GeometryStreamOutputShader mGeometryShader0;
	GeometryStreamOutputShader mGeometryShader1;
	VertexShader	mVertexShader;
	PixelShader		mPixelShader;
	ID3D11Buffer*	mpSOBuffer[2];

	weak_ptr<MaterialComponent> mMaterial;
	ConstantBuffer<CBChangesEveryFrame> mCBuffer;

	bool mFirstDraw;

};

#include"ParticleComponent.h"


#include "Game/Actor.h"

#include "Game/Component/MaterialComponent.h"
#include "Window/Window.h"
#include "Game/RenderingSystem.h"
#include "Game/Game.h"


#define _MAX_CAPACITY_PARTICLE 5000

class ParticleInitializeHelper{
public:
	ParticleInitializeHelper(){
		// �p�[�e�B�N���̏����l��ݒ�
		for (int i = 0; i<_MAX_CAPACITY_PARTICLE; i++)
		{
			mInitParticle[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			mInitParticle[i].v0 = XMFLOAT3(0, rand() * 0.001f, 0);
			mInitParticle[i].v = XMFLOAT3(0, 0, 0);
			mInitParticle[i].time = -10000.0f;
		};

		// �^�[�Q�b�g���o�̓o�b�t�@�[���쐬
		::ZeroMemory(&mBufferDesc, sizeof(mBufferDesc));
		mBufferDesc.ByteWidth = sizeof(mInitParticle);  // �o�b�t�@�T�C�Y
		mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		mBufferDesc.CPUAccessFlags = 0;
		mBufferDesc.MiscFlags = 0;
		mAppedResource.pSysMem = mInitParticle;  // �o�b�t�@�E�f�[�^�̏����l
		mAppedResource.SysMemPitch = 0;
		mAppedResource.SysMemSlicePitch = 0;
	}

	static ParticleInitializeHelper* Instance(){
		static ParticleInitializeHelper This;
		return &This;
	}


	void CreateBuffer(UINT num,ID3D11Buffer** buf){
		mBufferDesc.ByteWidth = sizeof(ParticleComponent::ParticleVertex) * num;  // �o�b�t�@�T�C�Y
		Device::mpd3dDevice->CreateBuffer(&mBufferDesc, &mAppedResource, buf);
	}

private:
	ParticleComponent::ParticleVertex mInitParticle[_MAX_CAPACITY_PARTICLE];
	D3D11_BUFFER_DESC mBufferDesc;
	D3D11_SUBRESOURCE_DATA mAppedResource;
};

ParticleComponent::ParticleComponent()
	:mFirstDraw(true){
	mpSOBuffer[0] = NULL;
	mpSOBuffer[1] = NULL;
	mInitialize = false;
	UINT stride[] = { sizeof(ParticleVertex) };

	D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �ʒu���W
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �����x
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �����x
			{ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }   // ����
	};

	D3D11_SO_DECLARATION_ENTRY decl[] = {
			{ 0, "POSITION", 0, 0, 3, 0 },
			{ 0, "NORMAL", 0, 0, 3, 0 },
			{ 0, "BINORMAL", 0, 0, 3, 0 },
			{ 0, "TEXCOORD", 0, 0, 1, 0 },
	};
	mGeometryShader0.Create("EngineResource/ParticleGS.fx", "GS0_Main", decl, _countof(decl), stride, _countof(stride));
	mGeometryShader1.Create("EngineResource/ParticleGS.fx", "GS1_Main");
	mVertexShader.Create("EngineResource/ParticleGS.fx", "VS0_Main", layout, 4);
	mPixelShader.Create("EngineResource/ParticleGS.fx", "PS1_Main");




	mCBuffer = ConstantBuffer<CBChangesEveryFrame>::create(2);
	if (!mCBuffer.mBuffer)
		return;

	mCParticleBuffer = ConstantBuffer<CBChangesPaticleParam>::create(10);
	if (!mCParticleBuffer.mBuffer)
		return;

	mParticleCapacity = 256;

	mParticleParam.Vector = XMFLOAT4(0, 0.1f, 0, 0.5f);
	mParticleParam.Point = XMFLOAT4(0, 0, 0, 0);
	mParticleParam.Rot = XMFLOAT4(90, 360, 0, 0.99f);
	mParticleParam.MinMaxScale = XMFLOAT4(0.05f, 0.05f, 0.5f, 1.0f);
	mParticleParam.G = XMFLOAT4(0, -0.0098f, 0, 0);
	mParticleParam.Time = XMFLOAT4(1, 3, 0.01f, 1);
	mParticleParam.Param = XMFLOAT4(mParticleCapacity, 0, 0, 0);

	mParticleParam.Param.w = rand();

	ParticleCapacityChange((UINT)mParticleCapacity);

	mBlendAdd = true;

	{

		pBlendState = NULL;
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable = FALSE;

		// TRUE�̏ꍇ�A�}���`�����_�[�^�[�Q�b�g�Ŋe�����_�[�^�[�Q�b�g�̃u�����h�X�e�[�g�̐ݒ���ʂɐݒ�ł���
		// FALSE�̏ꍇ�A0�Ԗڂ݂̂��g�p�����
		BlendDesc.IndependentBlendEnable = FALSE;

		//���Z�����ݒ�
		D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;
		RenderTarget.BlendEnable = TRUE;
		RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		RenderTarget.DestBlend = D3D11_BLEND_ONE;
		RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
		RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
		RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		BlendDesc.RenderTarget[0] = RenderTarget;

		Device::mpd3dDevice->CreateBlendState(&BlendDesc, &pBlendState);
	}


	mInitialize = true;
}
ParticleComponent::~ParticleComponent(){
	mGeometryShader0.Release();
	mGeometryShader1.Release();
	if (mpSOBuffer[0])mpSOBuffer[0]->Release();
	if (mpSOBuffer[1])mpSOBuffer[1]->Release();


	if (pBlendState)pBlendState->Release();
}

void ParticleComponent::ParticleCapacityChange(UINT num){

	mParticleCapacity = max(min(num, _MAX_CAPACITY_PARTICLE),1);

	auto helper = ParticleInitializeHelper::Instance();

	for (int i = 0; i < 2; i++){
		if (mpSOBuffer[i])mpSOBuffer[i]->Release();
		ID3D11Buffer* buf;
		helper->CreateBuffer((UINT)mParticleCapacity,&buf);
		mpSOBuffer[i] = buf;
		
	}
	mFirstDraw = true;
}
void ParticleComponent::EngineUpdate(){
	Update();
}
void ParticleComponent::Update(){
	if (!mInitialize)return;
	if (!mMaterial){
		mMaterial = gameObject->GetComponent<MaterialComponent>();
		if (!mMaterial)return;
	}
	mParticleParam.Param.w++;
	// �^�[�Q�b�g�o�̓o�b�t�@�[�����ւ���
	std::swap(mpSOBuffer[0], mpSOBuffer[1]);
	//mpSOBufferVector[0].swap(mpSOBufferVector[1]);

	//for (int i = 0; i < 2; i++){
	//	for (int j = 0; j < mpSOBufferVector[i].size(); j++){
	//		mpSOBufferArray[i][j] = &mpSOBufferVector[i].data()[j];
	//	}
	//}

	Game::AddDrawList(DrawStage::Particle, [&](){

		auto render = RenderingEngine::GetEngine(ContextType::MainDeferrd);
		render->PushSet(DepthStencil::Preset::DS_Zero_Less);



		if (mBlendAdd){
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			render->m_Context->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);
		}


		mCBuffer.mParam.mWorld = XMMatrixTranspose(gameObject->mTransform->GetMatrix());
		mCBuffer.UpdateSubresource(render->m_Context);
		mCBuffer.GSSetConstantBuffers(render->m_Context);


		mCParticleBuffer.mParam = mParticleParam;
		mCParticleBuffer.UpdateSubresource(render->m_Context);
		mCParticleBuffer.GSSetConstantBuffers(render->m_Context);

		mMaterial->GetMaterial(0).PSSetShaderResources(render->m_Context);


		// ���̓A�Z���u���[ �X�e�[�W�ɒ��_�o�b�t�@�[�̔z����o�C���h����B
		UINT stride = sizeof(ParticleVertex);
		UINT offset = 0;

		//ID3D11Buffer* buf1[] = { mpSOBufferVector[0][0], mpSOBufferVector[0][1], mpSOBufferVector[0][2], mpSOBufferVector[0][3], mpSOBufferVector[0][4], mpSOBufferVector[0][5], mpSOBufferVector[0][6] };
		render->m_Context->IASetVertexBuffers(0, 1, &mpSOBuffer[0], &stride, &offset);

		render->m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

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




		// �s�N�Z���V�F�[�_�[�𖳌��ɂ�����ԂŐ[�x�X�e���V����L���ɂ��Ă���ƃG���[�ɂȂ�̂Ŗ����ɂ���
		//ddsDesc.DepthEnable = FALSE;  // �[�x�e�X�g�𖳌��ɂ���
		//ddsDesc.StencilEnable = FALSE;




		mVertexShader.SetShader(render->m_Context);

		mGeometryShader0.SetShader(render->m_Context);

		// �s�N�Z���V�F�[�_�[���f�o�C�X�ɖ����ɂ���B
		render->m_Context->PSSetShader(NULL, NULL, 0);

		//Device::mpImmediateContext->VSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->HSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->DSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->GSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->PSSetShader(NULL, NULL, 0);
		//Device::mpImmediateContext->CSSetShader(NULL, NULL, 0);
		// �o�̓o�b�t�@�[�̔z��̃I�t�Z�b�g�l
		// �p�C�v���C���̃X�g���[���o�̓X�e�[�W�Ŏg�p�����^�[�Q�b�g�o�̓o�b�t�@�[��ݒ肵�܂��B

		render->m_Context->SOSetTargets(1, &mpSOBuffer[1], &offset);


		// �ŏ��͒��_�����킩��̂� ID3D11DeviceContext::Draw() �ŕ`�悷��
		if (mFirstDraw)
		{
			render->m_Context->Draw(mParticleCapacity, 0);
			mFirstDraw = false;
		}
		// �Q��ڈڍs�͏������钸�_�����s���Ȃ̂� ID3D11DeviceContext::DrawAuto() �ŕ`�悷��
		else
		{
			render->m_Context->DrawAuto();
		}

		ID3D11Buffer* pNullBuffer[] = { NULL};
		// �p�C�v���C���̃X�g���[���o�̓X�e�[�W�Ŏg�p�����^�[�Q�b�g�o�̓o�b�t�@�[�𖳌��ɂ���B
		render->m_Context->SOSetTargets(1, pNullBuffer, NULL);


		mGeometryShader1.SetShader(render->m_Context);
		mPixelShader.SetShader(render->m_Context);

		render->m_Context->DrawAuto();

		render->m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		render->m_Context->GSSetShader(NULL, NULL, 0);

		render->PopDS();

		if (mBlendAdd){
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			render->m_Context->OMSetBlendState(NULL, blendFactor, 0xffffffff);
		}
	});
}
void ParticleComponent::CreateInspector(){

	auto data = Window::CreateInspector();
	Window::AddInspector(new InspectorLabelDataSet("Particle"), data);

	std::function<void(float)> collbackVx = [&](float f){
		mParticleParam.Vector.x = f;
	};
	std::function<void(float)> collbackVy = [&](float f){
		mParticleParam.Vector.y = f;
	};
	std::function<void(float)> collbackVz = [&](float f){
		mParticleParam.Vector.z = f;
	};
	std::function<void(float)> collbackSpeed = [&](float f){
		mParticleParam.Vector.w = f;
	};
	std::function<void(float)> collbackMinVx = [&](float f){
		mParticleParam.Rot.x = f;
	};
	std::function<void(float)> collbackMinVy = [&](float f){
		mParticleParam.Rot.y = f;
	};
	std::function<void(float)> collbackMinVz = [&](float f){
		mParticleParam.Rot.z = f;
	};
	std::function<void(float)> collbackPx = [&](float f){
		mParticleParam.Point.x = f;
	};
	std::function<void(float)> collbackPy = [&](float f){
		mParticleParam.Point.y = f;
	};
	std::function<void(float)> collbackPz = [&](float f){
		mParticleParam.Point.z = f;
	};
	std::function<void(float)> collbackPT = [&](float f){
		mParticleParam.Point.w = f;
	};

	std::function<void(float)> collbackGx = [&](float f){
		mParticleParam.G.x = f;
	};
	std::function<void(float)> collbackGy = [&](float f){
		mParticleParam.G.y = f;
	};
	std::function<void(float)> collbackGz = [&](float f){
		mParticleParam.G.z = f;
	};
	std::function<void(float)> collbackMinS = [&](float f){
		mParticleParam.MinMaxScale.x = f;
	};
	std::function<void(float)> collbackMaxS = [&](float f){
		mParticleParam.MinMaxScale.y = f;
	};

	std::function<void(float)> collbackRef = [&](float f){
		mParticleParam.MinMaxScale.z = f;
	};
	std::function<void(float)> collbackFri = [&](float f){
		mParticleParam.MinMaxScale.w = f;
	};
	std::function<void(float)> collbackAirFri = [&](float f){
		mParticleParam.Rot.w = f;
	};

	std::function<void(float)> collbackMinT = [&](float f){
		mParticleParam.Time.x = f;
	};
	std::function<void(float)> collbackMaxT = [&](float f){
		mParticleParam.Time.y = f;
	};
	std::function<void(float)> collbackInp = [&](float f){
		mParticleParam.Param.y = f;
	};
	std::function<void(float)> collbackZCol = [&](float f){
		mParticleParam.Time.z = f;
	};
	std::function<void(float)> collbackVB = [&](float f){
		mParticleParam.Time.w = f;
	};
	std::function<void(float)> collbackPosG = [&](float f){
		mParticleParam.G.w = f;
	};
	std::function<void(float)> collbackGR = [&](float f){
		mParticleParam.Param.z = f;
	};
	std::function<void(float)> collbackNum = [&](float f){
		mParticleParam.Param.x = max(min((int)f,mParticleCapacity),0);
	};
	std::function<void(int)> collbackCap = [&](int f){
		ParticleCapacityChange((UINT)f);
	};
	std::function<void(bool)> collbackAdd = [&](bool f){
		mBlendAdd = f;
	};

	Window::AddInspector(new TemplateInspectorDataSet<int>("Capacity", &mParticleCapacity, collbackCap), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("Num", &mParticleParam.Param.x, collbackNum), data);
	Window::AddInspector(new InspectorVector3DataSet("Vector", &mParticleParam.Vector.x, collbackVx, &mParticleParam.Vector.y, collbackVy, &mParticleParam.Vector.z, collbackVz), data);
	Window::AddInspector(new InspectorVector3DataSet("RandRot", &mParticleParam.Rot.x, collbackMinVx, &mParticleParam.Rot.y, collbackMinVy, &mParticleParam.Rot.z, collbackMinVz), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("SubSpeed", &mParticleParam.Vector.w, collbackSpeed), data);

	Window::AddInspector(new InspectorVector3DataSet("EmitPoint", &mParticleParam.Point.x, collbackPx, &mParticleParam.Point.y, collbackPy, &mParticleParam.Point.z, collbackPz), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("EmitType", &mParticleParam.Point.w, collbackPT), data);

	Window::AddInspector(new InspectorVector3DataSet("Gravity", &mParticleParam.G.x, collbackGx, &mParticleParam.G.y, collbackGy, &mParticleParam.G.z, collbackGz), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("MinScale", &mParticleParam.MinMaxScale.x, collbackMinS), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("MaxScale", &mParticleParam.MinMaxScale.y, collbackMaxS), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("MinTime", &mParticleParam.Time.x, collbackMinT), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("MaxTime", &mParticleParam.Time.y, collbackMaxT), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("Impact", &mParticleParam.Param.y, collbackInp), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("Z-Collision", &mParticleParam.Time.z, collbackZCol), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("Bound", &mParticleParam.MinMaxScale.z, collbackRef), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("Friction", &mParticleParam.MinMaxScale.w, collbackFri), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("AirFriction", &mParticleParam.Rot.w, collbackAirFri), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("PointGravity", &mParticleParam.G.w, collbackPosG), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("PointGravityRot", &mParticleParam.Param.z, collbackGR), data);
	Window::AddInspector(new TemplateInspectorDataSet<float>("V-Rot&Bura", &mParticleParam.Time.w, collbackVB), data);
	Window::AddInspector(new TemplateInspectorDataSet<bool>("BlendModeAdd", &mBlendAdd, collbackAdd), data);


	Window::ViewInspector("Particle", this, data);
}

void ParticleComponent::IO_Data(I_ioHelper* io){
	(void)io;
#define _KEY(x) io->func( x , #x)

	_KEY(mParticleCapacity);
	_KEY(mParticleParam.Vector.x);
	_KEY(mParticleParam.Vector.y);
	_KEY(mParticleParam.Vector.z);
	_KEY(mParticleParam.Vector.w);
	_KEY(mParticleParam.Rot.x);
	_KEY(mParticleParam.Rot.y);
	_KEY(mParticleParam.Rot.z);
	_KEY(mParticleParam.Rot.w);
	_KEY(mParticleParam.Point.x);
	_KEY(mParticleParam.Point.y);
	_KEY(mParticleParam.Point.z);
	_KEY(mParticleParam.Point.w);
	_KEY(mParticleParam.G.x);
	_KEY(mParticleParam.G.y);
	_KEY(mParticleParam.G.z);
	_KEY(mParticleParam.G.w);
	_KEY(mParticleParam.MinMaxScale.x);
	_KEY(mParticleParam.MinMaxScale.y);
	_KEY(mParticleParam.MinMaxScale.z);
	_KEY(mParticleParam.MinMaxScale.w);
	_KEY(mParticleParam.Time.x);
	_KEY(mParticleParam.Time.y);
	_KEY(mParticleParam.Time.z);
	_KEY(mParticleParam.Time.w);
	_KEY(mParticleParam.Param.x);
	_KEY(mParticleParam.Param.y);
	_KEY(mParticleParam.Param.z);
	_KEY(mBlendAdd);

	ParticleCapacityChange((UINT)mParticleCapacity);

#undef _KEY
}
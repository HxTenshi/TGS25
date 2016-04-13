#pragma once

#include "IComponent.h"

#include "Graphic/Model/Model.h"
#include "Graphic/Material/Material.h"
#include "ConstantBuffer/ConstantBuffer.h"
#include "ConstantBuffer/ConstantBufferDif.h"

class PointLightComponent :public Component{
public:

	PointLightComponent();
	~PointLightComponent();

	void Initialize() override;
	void Finish() override;
	void EngineUpdate() override;
	void Update() override;
	void CreateInspector() override;
	void IO_Data(I_ioHelper* io) override;

	//�e���͈͔��a
	float m_Radius;
	//�����J�n
	float m_AttenuationStart;
	//�����Ȑ��p�����[�^
	float m_AttenuationParam;
	//�J���[
	XMFLOAT3 m_Color;

	Material mMaterial;

	Model mModel;

	ConstantBuffer<cbChangesPointLight> mPointLightBuffer;

};


#pragma once
#include "main.h"
#include <vector>

class RainbowShiningWing :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//ÉÅÉìÉoïœêî
	int mColorNumber;
	SERIALIZE
	float mColorChangeSpeed;
	SERIALIZE
	float mColorAlpha;
	float mRedColor;
	float mGreenColor;
	float mBlueColor;
	typedef std::vector<float> ColorContainer;
	ColorContainer mColorContainer;
};
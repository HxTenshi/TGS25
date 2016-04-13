
#pragma once
#include "main.h"


class testdes :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollide(Actor* target)override;

private:
	//ƒƒ“ƒo•Ï”
};
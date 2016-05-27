
#pragma once
#include "main.h"
#include "Enemy.h"

class LongSpinePorcupineFish : public Enemy {
public:
	virtual void Initialize()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Finish()override;
	virtual void OnCollideBegin(Actor* target)override;
	virtual void OnCollideEnter(Actor* target)override;
	virtual void OnCollideExit(Actor* target)override;

private:
};
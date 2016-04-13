#include "Script.h"
//#include "Game\Game.h"
#include "Game/Actor.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/TextComponent.h"
#include "Input/Input.h"
#include "MySTL/ptr.h"
#include "Game/Script/IGame.h"

#include <functional>
#include <map>

//#define INITGUID
#include <dinput.h>


class FuctorySetter{
public:
	FuctorySetter(){
#define _ADD(x) mFactory[typeid(x).name()] = [](){ return new x(); }
		
		//Ç±Ç±Ç…çÏê¨ÇµÇΩÉNÉâÉXÇí«â¡ÇµÇ‹Ç∑

		_ADD(CameraScript);
		_ADD(PlayerScript);
		_ADD(CoinScript);
		_ADD(CoinManagerScript);
		_ADD(Cursor);
		_ADD(BlockManager);
		_ADD(Block);
		_ADD(GoalPocket);

#undef _ADD
	}
	std::map<std::string, std::function<IDllScriptComponent*()>> mFactory;
};

FuctorySetter mFact;

IDllScriptComponent* CreateInstance(const char* ClassName){
	std::string name = "class ";
	name += ClassName;
	auto func = mFact.mFactory.find(name);
	if (func == mFact.mFactory.end())return NULL;
	return func->second();
}
void ReleseInstance(IDllScriptComponent* p){
	delete p;
}

void CameraScript::Start(){
	mRotateY = 0.0f;
	mLength = 5.0f;
	mClear = false;
}
void CameraScript::Update(){

	if (Input::Down(KeyCoord::Key_Q)){
		mRotateY -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_E)){
		mRotateY += 0.05f;
	}

	if (mClear){
		mRotateY += 0.01f;
	}

	auto rotate = XMQuaternionInverse(gameObject->mTransform->GetParent()->mTransform->Rotate());
	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mRotateY);
	gameObject->mTransform->Rotate(XMQuaternionMultiply(rotatey, rotate));
	//rotatey;
	auto back = XMVector3Normalize(gameObject->mTransform->Forward()) * -mLength;
	auto pos = XMVector3Rotate(XMVectorSet(0, 1, 0, 1) + back, rotate);
	gameObject->mTransform->Position(pos);
}

void CameraScript::GameClear(){
	mClear = true;
}

PlayerScript::PlayerScript(){
	mRotateY = 0.0f;
}
void PlayerScript::Update(){
	
	auto& list = gameObject->mTransform->Children();
	Actor* camera = NULL;
	for (auto act : list){
		if (act->Name() == "MainCamera"){
			camera = act;
		}
	}

	if (!camera)return;
	float speed = 50.0f;
	if (Input::Down(KeyCoord::Key_A)){
		gameObject->mTransform->AddTorque(camera->mTransform->Forward()*speed);
	}
	if (Input::Down(KeyCoord::Key_D)){
		gameObject->mTransform->AddTorque(camera->mTransform->Forward()*-speed);
	}
	if (Input::Down(KeyCoord::Key_W)){
		gameObject->mTransform->AddTorque(camera->mTransform->Left()*speed);
	}
	if (Input::Down(KeyCoord::Key_S)){
		gameObject->mTransform->AddTorque(camera->mTransform->Left()*-speed);
	}
	if (Input::Down(KeyCoord::Key_Z)){
		gameObject->mTransform->AddForce(camera->mTransform->Up()*10);
	}
	if (Input::Down(KeyCoord::Key_SPACE)){

	}

}

void PlayerScript::OnCollide(Actor* target){
	if (target->Name() == "coin"){
		game->DestroyObject(target);

		auto cm = game->FindActor("CoinManager");
		if (!cm)return;
		auto cms = cm->GetScript<CoinManagerScript>();
		if (!cms)return;
		cms->GetCoin();
	}
}

void CoinScript::Start(){
	mRotateY = 0.0f;
}

void CoinScript::Update(){
	mRotateY += 0.01f;
	gameObject->mTransform->Rotate(XMVectorSet(0, mRotateY, 0,1));
}

void CoinManagerScript::Start(){
	mGetCoinCount = 0;
	mMaxCoin = 0;
	isClear = false;

	auto obj = game->FindActor("GetCoinText");
	if (!obj)return;
	auto text = obj->GetComponent<TextComponent>();
	if (!text)return;
	text->ChangeText("GetCoin:0");
}
void CoinManagerScript::Update(){
	mMaxCoin = gameObject->mTransform->Children().size();

	if (mMaxCoin == 0 && !isClear){
		auto a = game->CreateActor("Assets/gameclear");
		if (a)game->AddObject(a);
		isClear = true;

		auto c = game->FindActor("MainCamera");
		if (c){
			auto s = c->GetScript<CameraScript>();
			if (s)
			{
				s->GameClear();
			}
		}
	}

}


void CoinManagerScript::GetCoin(){
	mGetCoinCount++;

	auto obj = game->FindActor("GetCoinText");
	if (!obj)return;
	auto text = obj->GetComponent<TextComponent>();
	if (!text)return;
	text->ChangeText("GetCoin:"+std::to_string(mGetCoinCount));
}

Cursor::Cursor(){
}
void Cursor::Update(){

	float s = gameObject->mTransform->LossyScale().x;
	float speed = 1.0f * s;
	XMVECTOR pos = gameObject->mTransform->Position();

	XMVECTOR move = XMVectorZero();

	if (Input::Trigger(KeyCoord::Key_W)){
		move = gameObject->mTransform->Up() * speed;
	}
	else if (Input::Trigger(KeyCoord::Key_S)){
		move = gameObject->mTransform->Up() * -speed;
	}
	else if(Input::Trigger(KeyCoord::Key_D)){
		move = gameObject->mTransform->Left() * speed;
	}
	else if(Input::Trigger(KeyCoord::Key_A)){
		move = gameObject->mTransform->Left() * -speed;
	}

	if (Input::Down(KeyCoord::Key_Z)){


		auto bm = game->FindActor("BlockManager");
		if (!bm)return;

		auto bms = bm->GetScript<BlockManager>();
		if (!bms)return;


		bms->MoveBlock(pos, move);
	}


	gameObject->mTransform->Position(pos + move);
}


void BlockManager::Initialize(){

	x = 10;
	y = 10;
	for (int i = 0; i < x*y; i++){
		int _x = i % y;
		int _y = i / y;
		mBlockMap[_y][_x] = NULL;
	}

}
void BlockManager::SetBlock(Block* block){
	if (!block)return;
	int _x = block->gameObject->mTransform->Position().x;
	int _y = block->gameObject->mTransform->Position().y;
	if (0 > _x || 0 > _y || _x >= x || _y >= y)return;

	mBlockMap[_y][_x] = block;
}
void BlockManager::MoveBlock(XMVECTOR& pos, XMVECTOR& move){
	if (move.x == 0 && move.y==0)return;

	int _x = pos.x;
	int _y = pos.y;
	if (0 > _x || 0 > _y || _x >= x || _y >= y)return;

	int move_x = move.x + pos.x;
	int move_y = move.y + pos.y;
	if (0 > move_x || 0 > move_y || move_x >= x || move_y >= y)return;

	if (mBlockMap[move_y][move_x] != NULL)return;
	if (mBlockMap[_y][_x] == NULL)return;

	auto block = mBlockMap[_y][_x];
	block->Move(XMVectorSet(move.x, move.y, 0, 0));

	std::swap(mBlockMap[_y][_x], mBlockMap[move_y][move_x]);
}

void Block::Initialize(){
	mIsMovement = true;
	mMoveTimer = 0;
	mMoveParam = XMVectorZero();
}
void Block::Start(){
	auto bm = game->FindActor("BlockManager");
	if (!bm)return;

	auto bms = bm->GetScript<BlockManager>();
	if (!bms)return;

	bms->SetBlock(this);


}
void Block::Update(){
	if (!mMoveTimer)return;

	mMoveTimer--;

	auto p = gameObject->mTransform->Position();
	gameObject->mTransform->Position(p+mMoveParam/10);

}
void Block::Move(XMVECTOR& move){
	mMoveParam = move;
	mMoveTimer = 10;

}
bool Block::IsMovement(){
	return mIsMovement;
}
void GoalPocket::Initialize(){
	mWaterGoalCount = 0;
	mWaterMax = 0;
}
void GoalPocket::Start(){
	auto wb = game->FindActor("WaterBase");
	if (!wb)return;

	mWaterMax = wb->mTransform->Children().size();

}
void GoalPocket::OnCollide(Actor* target){
	if (target->Name() == "water"){
		game->DestroyObject(target);
		mWaterGoalCount++;

		if (mWaterMax == mWaterGoalCount){
			mWaterGoalCount = -1;
			auto act = game->CreateActor("Assets/GameClear");
			if (act){
				game->AddObject(act);
			}
		}
	}
}
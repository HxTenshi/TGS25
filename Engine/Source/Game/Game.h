#pragma once

#include <map>
#include "Device/DirectX11Device.h"
#include "Graphic/RenderTarget/RenderTarget.h"

#include "Input/Input.h"
#include "Game/Component/Component.h"
#include "Actor.h"

#include "Sound/Sound.h"

#include "../PhysX/PhysX3.h"
#include "Engine/ICommand.h"

#include "Scene.h"

#include "PostEffectRendering.h"
#include "DeferredRendering.h"


#include "Engine/SelectActor.h"

#include "Game/EngineObject/FPSChecker.h"
#include "Game/EngineObject/EditorCamera.h"
#include "Game/EngineObject/ProfileViewer.h"
#include "Engine/WorldGrid.h"


class CameraComponent;

enum class DrawStage{
	Init,
	Diffuse,
	Depth,
	Normal,
	Light,
	Particle,
	PostEffect,
	Forward,
	Engine,
	UI,
	Count
};
typedef void(*DrawFunc)();



class Game{
public:
	Game();
	~Game();

	static void AddObject(Actor* actor);
	static void AddEngineObject(Actor* actor);
	static void DestroyObject(Actor* actor);
	static void ActorMoveStage();
	static PxRigidActor* CreateRigitBody();
	static PxRigidActor* CreateRigitBodyEngine();
	static PhysX3Main* GetPhysX();
	static PhysXEngine* GetPhysXEngine();
	static void RemovePhysXActor(PxActor* act);
	static void RemovePhysXActorEngine(PxActor* act);
	static void AllDestroyObject();
	static void GetAllObject(const std::function<void(Actor*)>& collbak);
	static Actor* FindActor(Actor* actor);
	static Actor* FindNameActor(const char* name);
	static Actor* FindUID(UINT uid);
	static void AddDrawList(DrawStage stage, std::function<void()> func);
	static void SetUndo(ICommand* command);
	static void SetMainCamera(CameraComponent* Camera);
	static CameraComponent* GetMainCamera();
	static RenderTarget GetMainViewRenderTarget();
	static bool IsGamePlay();
	static void LoadScene(const std::string& FilePath);

	void ChangePlayGame(bool isPlay);
	void SaveScene();

	void Draw();

	float GetDeltaTime();
	void Update();
	void GameStop();
	void GamePlay();

	void ClearDrawList();
	void PlayDrawList(DrawStage Stage);

	typedef std::map<UINT, Actor*> ListMapType;
	typedef std::map<DrawStage, std::vector<std::function<void()>>> DrawListMapType;

private:
	Game(const Game&);
	Game operator = (Game&);

	enum class ActorMove{
		Create,
		Delete,
		Count,
	};
	//�c���[�r���[�̃A�C�e���폜�Ɏ��s�����A�N�^�[
	std::list<Actor*> mTreeViewItem_ErrerClearList;
	//�ǉ��ƍ폜
	std::queue<std::pair<ActorMove, Actor*>> mActorMoveList;
	//�Q�[���I�u�W�F�N�g�̃��X�g
	ListMapType mList;
	DrawListMapType mDrawList;
	static Actor* mRootObject;
	static Actor* mEngineRootObject;

	SelectActor mSelectActor;
	EditorCamera mCamera;
	WorldGrid mWorldGrid;
	FPSChecker mFPS;
	ProfileViewer mProfileViewer;

	PhysX3Main* mPhysX3Main;

	CommandManager mCommandManager;

	CameraComponent* mMainCamera;
	RenderTarget mMainViewRenderTarget;
	DeferredRendering m_DeferredRendering;
	PostEffectRendering mPostEffectRendering;

	static Scene m_Scene;

	bool mIsPlay;

	ConstantBuffer<cbGameParameter> mCBGameParameter;


};
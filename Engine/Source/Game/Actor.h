#pragma once

#include <d3d11.h>
#include "XNAMath/xnamath.h"
#include <functional>
#include <queue>
#include "Game/Component/ComponentList.h"
class ITransformComponent;
class File;
//PhysXテスト用
namespace physx{
	class PxTransform;
}

enum class DrawBit : unsigned char{
	Diffuse = 0x0001,
	Depth = 0x0002,
	Normal = 0x0004,
};

unsigned char operator & (const unsigned char& bit, const DrawBit& bit2);
unsigned char operator | (const DrawBit& bit, const DrawBit& bit2);
unsigned char operator | (const unsigned char& bit, const DrawBit& bit2);


class IActor{
public:
	virtual ~IActor(){}
	virtual void* _GetScript(const char* name) = 0;
};

class Actor : public IActor{
public:
	Actor();
	virtual ~Actor();
	virtual void Initialize();
	virtual void Start();
	virtual void Finish();
	virtual void EngineUpdateComponent(float deltaTime);
	virtual void UpdateComponent(float deltaTime);
	virtual void Update(float deltaTime);

	void SetUpdateStageCollQueue(const std::function<void()> coll);

	template<class T>
	weak_ptr<T> GetComponent(){
		return mComponents.GetComponent<T>();
	}
	weak_ptr<Component> GetComponent(const size_t& hash){
		return mComponents.GetComponent(hash);
	}

	template<class T>
	T* GetScript(){
		return (T*)_GetScript(typeid(T).name());
	}

	void* _GetScript(const char* name) override;

	template<class T>
	shared_ptr<T> AddComponent(shared_ptr<T> component){
		return mComponents.AddComponent<T>(component);
	}
	template<class T>
	shared_ptr<T> AddComponent(){
		return mComponents.AddComponent<T>();
	}
	template<class T>
	void RemoveComponent(){
		mComponents.RemoveComponent<T>();
	}
	void RemoveComponent(Component* comptr){
		mComponents.RemoveComponent(comptr);
	}

	void CreateInspector();

	std::string Name(){return mName;}
	void Name(std::string name){mName = name;}

	UINT GetUniqueID(){
		return mUniqueID;
	}

	void CopyData(Actor* post, Actor* base);

	void ExportSceneDataStart(const std::string& pass, File& sceneFile);
	void ExportSceneData(const std::string& pass, File& sceneFile);
	void ExportData(const std::string& pass, const std::string& fileName);
	void ExportData(const std::string& pass);
	void ImportData(const std::string& fileName);
	bool ImportDataAndNewID(const std::string& fileName);

	void ExportData(picojson::value& json);
	void ImportDataAndNewID(picojson::value& json);

	void CreateNewID();

	shared_ptr<ITransformComponent> mTransform;


	//PhysXテスト用
	void SetTransform(physx::PxTransform* t);

	void* mTreeViewPtr;

protected:
	ComponentList mComponents;

private:

	std::queue<std::function<void()>> mUpdateStageCollQueue;
	std::string mName;

	UINT mUniqueID;
};
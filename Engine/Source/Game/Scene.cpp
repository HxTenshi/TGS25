
#include "Scene.h"
#include "MySTL/Utility.h"
#include "MySTL/File.h"

#include "Game.h"

Scene::Scene(){

}
Scene::~Scene(){

}

void Scene::LoadScene(const std::string& fileName){

	m_FilePath = fileName;
	m_Name = behind_than_find_last_of(fileName, "/");
	if (m_Name == ""){
		m_Name = fileName;
	}
	File scenefile(m_FilePath);
	if (scenefile){
		UINT id;
		while (scenefile){
			if (!scenefile.In(&id))break;
			auto a = new Actor();
			a->ImportData("./Scenes/" + m_Name + "/Object_" + std::to_string(id) + ".json");
			Game::AddObject(a);
		}
	}
}
void Scene::SaveScene(Actor* SceneRoot){
	File scenefile;
	if (!scenefile.Open(m_FilePath)){
		scenefile.FileCreate();
	}
	scenefile.Clear();

	SceneRoot->ExportSceneDataStart("./Scenes/" + m_Name, scenefile);
}
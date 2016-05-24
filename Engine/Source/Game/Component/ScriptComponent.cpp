
#include <list>
#include <iostream>
#include <algorithm>
#include <filesystem> // std::tr2::sys::path etc.

#include "ScriptComponent.h"
#include "MySTL/Reflection/Reflection.h"
#include "Window/window.h"

#include "../ScriptComponent/main.h"
typedef IDllScriptComponent* (__cdecl *CreateInstance_)(const char*);
typedef void(__cdecl *DeleteInstance_)(IDllScriptComponent*);

typedef decltype(Reflection::map) (__cdecl *GetReflectionData_)();

typedef void(__cdecl *IDllFunction0)(IDllScriptComponent*,IDllScriptComponent::Func0);
typedef void(__cdecl *IDllFunction1)(IDllScriptComponent*,IDllScriptComponent::Func1, Actor*);

#include "Game/Script/SGame.h"
SGame gSGame;

void CreateScriptFileExtension(const std::string& classNmae, const std::string& extension){

	std::fstream file;

	auto srcname = "EngineResource/ScriptTemplate/ScriptTemplate" + extension;
	file.open(srcname, std::ios::in | std::ios::ate);

	std::fstream outFile;
	auto outfilename = "ScriptComponent/Scripts/"+ classNmae + extension;
	outFile.open(outfilename, std::ios::out);

	int length = (int)file.tellg();
	file.seekg(0, file.beg);//�t�@�C���|�C���^���ŏ��ɖ߂�
	char * buf = new char[length+1];

	memset(buf, 0, (length+1)*sizeof(char));
	file.read(buf, length);

	std::string buffer = buf;
	std::string from = "$temp$";
	std::string to = classNmae;

	auto pos = buffer.find(from);
	while (pos != std::string::npos){
		buffer.replace(pos, from.size(), to);
		pos = buffer.find(from, pos + to.size());
	}

	outFile.write(buffer.c_str(), buffer.size());

	file.close();
	outFile.close();

	delete[] buf;
}



#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

std::string outConsole;
bool create_cmd_process(){
	//	�p�C�v�̍쐬
	HANDLE readPipe;
	HANDLE writePipe;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if (CreatePipe(&readPipe, &writePipe, &sa, 0) == 0){
		Window::AddLog("�p�C�v���쐬�ł��܂���ł���");
		return false;
	}

	Window::AddLog("�R���p�C���J�n...");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = writePipe;
	si.hStdError = writePipe;
	si.wShowWindow = SW_HIDE;
#ifdef _DEBUG
	char cmd[] = "cmd.exe /K \".\\ScriptComponent\\createdll_auto_d.bat\"";
#else
	char cmd[] = "cmd.exe /K \".\\ScriptComponent\\createdll_auto.bat\"";
#endif

	if (!PathFileExists("ScriptComponent\\createdll_auto.bat")){
		Window::AddLog("�R���p�C�����s");
		return false;
	}

	//	�v���Z�X�̋N��(cmd.exe)
	if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == 0){
		Window::AddLog("�v���Z�X�̍쐬�Ɏ��s���܂���");
		return false;
	}
	HANDLE childProcess = pi.hProcess;
	CloseHandle(pi.hThread);

	outConsole.reserve(20000);
	char readBuf[1025];
	bool end = false;
	do{
		DWORD totalLen, len;
		if (WaitForSingleObject(childProcess, 100) == WAIT_OBJECT_0)
			end = true;
		if (PeekNamedPipe(readPipe, NULL, 0, NULL, &totalLen, NULL) == 0)
			break;
		if (0 < totalLen){
			if (ReadFile(readPipe, readBuf, sizeof(readBuf) - 1, &len, NULL) == 0){
				Window::AddLog("�G���[");
				return false;
			}
			readBuf[len] = 0;

			//if (sizeof(mem) - 1<memSz + len){	//�����������ӂ�Ȃ��悤�ɃN���A����
			//	mem[0] = 0;
			//	memSz = 0;
			//}
			//memSz += len;
			//strcat_s(mem, sizeof(mem), readBuf);
			outConsole += readBuf;
			//SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);	//	�X�N���[���o�[����ԉ��ֈړ�������
			if (totalLen>len)	//	�v���Z�X�͏I�����Ă��邪�܂��f�[�^���[���c���Ă���̂ŏI����ۗ�
				end = false;

			{
				auto out = forward_than_find_first_of(outConsole, "\n");
				if (out == "")continue;
				outConsole = behind_than_find_first_of(outConsole, "\n");
				out.pop_back();
				Window::AddLog(out);
			}
		}
	} while (end == false);

	if (CloseHandle(writePipe) == 0){
		Window::AddLog("�p�C�v����邱�Ƃ��ł��܂���ł����B");
		return false;
	}
	if (CloseHandle(readPipe) == 0){
		Window::AddLog("�p�C�v����邱�Ƃ��ł��܂���ł����B");
		return false;
	}
	CloseHandle(pi.hProcess);
	for (int i = 0; i < 100; i++){
		auto out = forward_than_find_first_of(outConsole, "\n");
		outConsole = behind_than_find_first_of(outConsole, "\n");
		if (out == "")break;
		out.pop_back();
		Window::AddLog(out);
	}
	Window::AddLog("�R���p�C���I��");
	return true;
}

//����DLL�������Ă�R���|�[�l���g���Ǘ�
class UseScriptActors{
public:
	UseScriptActors(){
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		mGetReflect = NULL;
		mFunction0 = NULL;
		mFunction1 = NULL;

	}
	~UseScriptActors(){
		UnLoad();
	}
	void ReCompile(){
		for (auto& p : mList){
			p->Unload();
		}

		UnLoad();

		CreateIncludeClassFile();




		if (!create_cmd_process()){
			//MessageBox(Window::GetMainHWND(), "�r���h���蓮�ōs���ĉ������B", "DLL�ǂݍ���", MB_OK);

					char cdir[255];
					GetCurrentDirectory(255, cdir);
					std::string  pass = cdir;
			#ifdef _DEBUG
					pass += "/ScriptComponent/createdll_auto_d.bat";
			#else
					pass += "/ScriptComponent/createdll_auto_.bat";
			#endif

			SHELLEXECUTEINFO	sei = { 0 };
			//�\���̂̃T�C�Y
			sei.cbSize = sizeof(SHELLEXECUTEINFO);
			//�N�����̃E�C���h�E�n���h��
			sei.hwnd = Window::GetMainHWND();
			//�N����̕\�����
			sei.nShow = SW_SHOWNORMAL;
			//���̃p�����[�^���d�v�ŁA�Z�b�g���Ȃ���SHELLEXECUTEINFO�\���̂�hProcess�����o���Z�b�g����Ȃ��B
			sei.fMask = SEE_MASK_NOCLOSEPROCESS;
			//�N���v���O����
			sei.lpFile = pass.c_str();
			//�v���Z�X�N��
			if (!ShellExecuteEx(&sei) || (const int)sei.hInstApp <= 32){
				MessageBox(Window::GetMainHWND(), "�t�@�C�����J���܂���ł���", "���s", MB_OK);
				return;
			}
			
			
			//�I����҂�
			WaitForSingleObject(sei.hProcess, INFINITE);
			
			CloseHandle(sei.hProcess);
		}

		DllLoad();

		for (auto& p : mList){
			p->Load();
		}
	}

	void CreateIncludeClassFile(){
		//�t�@�C�����X�V���ăR���p�C��������
		{
			std::ofstream h1("./ScriptComponent/Script.cpp", std::ios::app | std::ios::out);
			h1 << "";
			h1.close();
		}
		{

			std::ofstream h1("./ScriptComponent/Reflection/ReflectionSetter.cpp", std::ios::app | std::ios::out);
			h1 << "";
			h1.close(); 
		}
		//�C���N���[�h�̍\�z
		File includesFile("./ScriptComponent/System/include");
		includesFile.FileCreate();
		File factorysFile("./ScriptComponent/System/factory");
		factorysFile.FileCreate();
		File reflectionsFile("./ScriptComponent/System/reflection");
		reflectionsFile.FileCreate();

		namespace sys = std::tr2::sys;
		sys::path p("./ScriptComponent/Scripts/"); // �񋓂̋N�_
		//std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
			//  �ċA�I�ɑ�������Ȃ�R�`����
		std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
			[&](const sys::path& p) {
			if (sys::is_regular_file(p)) { // �t�@�C���Ȃ�...
				if (p.extension() == ".h"){
					auto out = "\n#include\"../" + p.directory_string() + "\"";
					includesFile.Out(out);

					auto outf = "\n_ADD(" + p.stem() + ");";
					factorysFile.Out(outf);

					findSerialize(&reflectionsFile, p.stem());
				}
			}
			else if (sys::is_directory(p)) { // �f�B���N�g���Ȃ�...
				//std::cout << "dir.: " << p.string() << std::endl;
			}
		});
	}

	void findSerialize(File* file, const std::string& classname){

		auto filepath = "./ScriptComponent/Scripts/" +classname+".h";
		std::ifstream classfile(filepath, std::ios::in);

		if (classfile.fail())
		{
			return;
		}
		//�S���ǂݍ���
		std::istreambuf_iterator<char> it(classfile);
		std::istreambuf_iterator<char> last;
		std::string str(it, last);

		unsigned int findpoint = 0;
		while (1){
			//�V���A���C�Y����
			auto serialize = str.find("SERIALIZE", findpoint);

			if (std::string::npos == serialize){
				break;
			}

			auto serializeend = str.find(";", serialize);
			if (std::string::npos == serializeend){
				break;

			}
			findpoint = serializeend;

			//�V���A���C�Y�͈͂𔲂��o��
			auto sub = serializeend - serialize;
			auto serializestr = str.substr(serialize, sub);

			findMember(std::move(serializestr), file, classname);
		}
	}

	void findMember(std::string&& pickstr, File* file, const std::string& classname){

		//�����ɕ���킵������ȕ������폜
		filter(pickstr);


		//�X�y�[�X��؂�ɂ���
		std::vector<std::string> v;
		split(v, pickstr, " ");
		auto num = (int)v.size();

		//������̌���Member�ϐ�������
		std::string member;
		for (int i = num - 1; i >= 0; i--){
			auto l = (int)v[i].size();
			if (l == 0)continue;
			member = v[i];
			break;
		}

		if (member != ""){
			reflect(member,file, classname);
		}
	}

	void reflect(const std::string& member, File* file, const std::string& classname){
		auto outf = "\n_REF(" + classname + "," + member + ");";
		file->Out(outf);
	}

	void filter(std::string& str){
		//while(1){
		//	auto temp = str.find("\\\\");
		//	if (temp == std::string::npos){
		//		break;
		//	}
		//	str.replace(temp,2,"  ");
		//}
		//while (1){
		//	auto temp = str.find("\\\"");
		//	if (temp == std::string::npos){
		//		break;
		//	}
		//	str.replace(temp, 2, "  ");
		//}
		//������Ă��邩

		while (1){
			auto start = str.find('\"');
			if (start == std::string::npos){
				break;
			}
			auto end = str.find('\"', start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}

		
		while (1){
			auto start = str.find("/*");
			if (start == std::string::npos){
				break;
			}
			auto end = str.find("*/", start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}
		while (1){
			auto start = str.find("//");
			if (start == std::string::npos){
				break;
			}
			auto end = str.find('\n', start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}

		while (1){
			auto temp = str.find('\n');
			if (temp == std::string::npos){
				break;
			}
			str.replace(temp, 1, " ");
		}
		while (1){
			auto temp = str.find('\t');
			if (temp == std::string::npos){
				break;
			}
			str.replace(temp, 1, " ");
		}
		while (1){
			auto start = str.find('\'');
			if (start == std::string::npos){
				break;
			}
			auto end = str.find('\'', start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}


		//������Ă��邩
		auto equal = str.find('=');
		if (std::string::npos != equal){
			str.erase(equal);
		}

	}

	void split(std::vector<std::string> &v, const std::string &input_string, const std::string &delimiter)
	{
		std::string::size_type index = input_string.find_first_of(delimiter);

		if (index != std::string::npos) {
			v.push_back(input_string.substr(0, index));
			split(v, input_string.substr(index + 1), delimiter);
		}
		else {
			v.push_back(input_string);
		}
	}

	void UnLoad(){
		FreeLibrary(hModule);
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		mGetReflect = NULL;
		mFunction0 = NULL;
		mFunction1 = NULL;
		Reflection::map = NULL;
	}
	void DllLoad(){

		// DLL�̃��[�h
#ifdef _DEBUG
		hModule = LoadLibrary("../ScriptComponent/Debug/ScriptComponent.dll");
#else
		hModule = LoadLibrary("../ScriptComponent/Release/ScriptComponent.dll");
#endif
		if (hModule == NULL)
		{
#ifdef _DEBUG
			hModule = LoadLibrary("ScriptComponent/Debug/ScriptComponent.dll");
#else
			hModule = LoadLibrary("ScriptComponent/Release/ScriptComponent.dll");
#endif
		}
		if (!hModule){
			_SYSTEM_LOG_ERROR("�X�N���v�gDLL�̓ǂݍ���");
		}


		mCreate = (CreateInstance_)GetProcAddress(hModule, "CreateInstance");
		if (mCreate == NULL)
		{
			UnLoad();
			return;
		}
		mDelete = (DeleteInstance_)GetProcAddress(hModule, "ReleseInstance");
		if (mDelete == NULL)
		{
			UnLoad();
			return;
		}
		mGetReflect = (GetReflectionData_)GetProcAddress(hModule, "GetReflectionData");
		if (mGetReflect == NULL)
		{
			UnLoad();
			return;
		}

		mFunction0 = (IDllFunction0)GetProcAddress(hModule, "Function0");
		if (mFunction0 == NULL)
		{
			UnLoad();
			return;
		}
		mFunction1 = (IDllFunction0)GetProcAddress(hModule, "Function1");
		if (mFunction1 == NULL)
		{
			UnLoad();
			return;
		}
		Reflection::map = ((GetReflectionData_)mGetReflect)();
	}
	IDllScriptComponent* Create(const std::string& ClassName){
		if (!mCreate){
			_SYSTEM_LOG_ERROR("�X�N���v�g[" + ClassName + "]�̍쐬");
			return NULL;
		}
		//dll�ō쐬�����N���X�C���X�^���X���쐬����
		return ((CreateInstance_)mCreate)(ClassName.c_str());
	}

	void Deleter(IDllScriptComponent* script){
		((DeleteInstance_)mDelete)(script);
	}
	void Function(IDllScriptComponent* com,IDllScriptComponent::Func0 func){
		((IDllFunction0)mFunction0)(com,func);
	}
	void Function(IDllScriptComponent* com,IDllScriptComponent::Func1 func, Actor* tar){
		((IDllFunction1)mFunction1)(com,func, tar);
	}
	std::list<ScriptComponent*> mList;
	void* mCreate;
	void* mDelete;
	void* mGetReflect;
	void* mFunction0;
	void* mFunction1;
	HMODULE hModule;

};

UseScriptActors actors;


//static
void ScriptManager::ReCompile(){
	actors.ReCompile();
}
void ScriptManager::CreateScriptFile(const std::string& className){
	CreateScriptFileExtension(className, ".h");
	CreateScriptFileExtension(className, ".cpp");
}



ScriptComponent::ScriptComponent(){
	mEndInitialize = false;
	mEndStart = false;
	pDllClass = NULL;
}
ScriptComponent::~ScriptComponent(){
}
void ScriptComponent::Initialize(){
	mCollideMap.clear();
	Load();
	actors.mList.push_back(this);
	mEndInitialize = true;
	if (pDllClass){
		pDllClass->game = &gSGame;
		pDllClass->gameObject = gameObject;

		actors.Function(pDllClass,&IDllScriptComponent::Initialize);

	}
}
void ScriptComponent::Load(){
	if (pDllClass)return;

	//dll�ō쐬�����N���X�C���X�^���X���쐬����
	pDllClass = actors.Create(mClassName);

	if (pDllClass){
		pDllClass->game = &gSGame;
		pDllClass->gameObject = gameObject;

		if (mEndInitialize)
			actors.Function(pDllClass, &IDllScriptComponent::Initialize);
		if (mEndStart)
			actors.Function(pDllClass, &IDllScriptComponent::Start);
	}
}
void ScriptComponent::Unload(){

	if (pDllClass){
		actors.Function(pDllClass, &IDllScriptComponent::Finish);
		actors.Deleter(pDllClass);
	}

	pDllClass = NULL;

}
void ScriptComponent::ReCompile(){
	Unload();
	Load();

}
void ScriptComponent::Start(){
	mEndStart = true;
	if (pDllClass){
		actors.Function(pDllClass, &IDllScriptComponent::Start);
	}
}
void ScriptComponent::Update(){

	if (pDllClass){
		actors.Function(pDllClass, &IDllScriptComponent::Update);

		for (auto& tar : mCollideMap){

			actors.Function(pDllClass, &IDllScriptComponent::OnCollideEnter, tar.second);
		}
	}

}
void ScriptComponent::Finish(){

	Unload();
	actors.mList.remove(this);
	mEndInitialize = false;
	mEndStart = false;
}

void ScriptComponent::OnCollide(Actor* target){
	mCollideMap[(int)target] = target;

	if (pDllClass){
		actors.Function(pDllClass, &IDllScriptComponent::OnCollideBegin, target);
	}
}

void ScriptComponent::LostCollide(Actor* target){
	mCollideMap.erase((int)target);
	if (pDllClass){
		actors.Function(pDllClass, &IDllScriptComponent::OnCollideExit, target);
	}
}


template<class T, class U>
bool reflect(MemberInfo& info,U& data){

	if (info.GetTypeName() != typeid(T).name()){
		return false;
	}

	T* paramdata = Reflection::Get<T>(info);

	std::function<void(T)> collback = [=](T f){
		*paramdata = f;
	};
	Window::AddInspector(new TemplateInspectorDataSet<T>(info.GetName(), paramdata, collback), data);

	return true;
}
template<class T>
bool reflect_io(MemberInfo& info,I_ioHelper* io){

	if (info.GetTypeName() != typeid(T).name()){
		return false;
	}

	T* paramdata = Reflection::Get<T>(info);

	io->func(*paramdata, info.GetName().c_str());

	return true;
}

void ScriptComponent::CreateInspector(){

	auto data = Window::CreateInspector();
	std::function<void(std::string)> collback = [&](std::string name){
		mClassName = name;
		ReCompile();
	};
	Window::AddInspector(new TemplateInspectorDataSet<std::string>("Class", &mClassName, collback), data);

	if (Reflection::map){
		auto infos = Reflection::GetMemberInfos(pDllClass, mClassName);

		for (auto i : infos){
			auto info = Reflection::GetMemberInfo(infos,i);

			do{
				if (reflect<float>(info, data))break;
				if (reflect<int>(info, data))break;
				if (reflect<bool>(info, data))break;
				if (reflect<std::string>(info, data))break;
			} while (0);
		}

	}

	Window::ViewInspector("Script", this, data);
}

void ScriptComponent::IO_Data(I_ioHelper* io){
#define _KEY(x) io->func( x , #x)
	_KEY(mClassName);
	Load();

	if (pDllClass){

		if (Reflection::map){
			auto infos = Reflection::GetMemberInfos(pDllClass, mClassName);

			for (auto i : infos){
				auto info = Reflection::GetMemberInfo(infos, i);

				do{
					if (reflect_io<float>(info, io))break;
					if (reflect_io<int>(info, io))break;
					if (reflect_io<bool>(info, io))break;
					if (reflect_io<std::string>(info, io))break;
				} while (0);
			}
		}
	}

#undef _KEY
}
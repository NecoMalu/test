#include "resourceLoader.h"
#include <unordered_map>
#include <DxLib.h>
#include <windows.h>
#include <assert.h>

namespace {
	enum TYPE {
		T_IMAGE,
		T_MODEL,
		T_SOUND,
	};

	struct FILE_HANDLE {
		int handle;
		TYPE type;
		FILE_HANDLE() : handle(-1) {}
	};
	std::unordered_map<std::string, FILE_HANDLE> files;
};

void ResourceLoader::Init()
{
	files.clear();
}

int ResourceLoader::LoadGraph(std::string filename)
{
	FILE_HANDLE f = files[filename];
	if (f.handle==-1) {
		f.handle = DxLib::LoadGraph(filename.c_str());
		f.type = T_IMAGE;
		files[filename] = f;
	}
	return f.handle;
}

void ResourceLoader::DeleteGraph(int handle)
{
}

int ResourceLoader::MV1LoadModel(std::string filename)
{
	FILE_HANDLE f = files[filename];
	if (f.handle==-1) {
		f.handle = DxLib::MV1LoadModel(filename.c_str());
		f.type = T_MODEL;
		files[filename] = f;
	}
	return DxLib::MV1DuplicateModel(f.handle); // モデルはコピーを返す
}

void ResourceLoader::MV1DeleteModel(int handle)
{
	DxLib::MV1DeleteModel(handle);
}

int ResourceLoader::LoadSoundMem(std::string filename)
{
	FILE_HANDLE f = files[filename];
	if (f.handle==-1) {
		f.handle = DxLib::LoadSoundMem(filename.c_str());
		f.type = T_SOUND;
		files[filename] = f;
	}
	return f.handle;
}

void ResourceLoader::DeleteSoundMem(int handle)
{
}

void ResourceLoader::LoadFolderFile(std::string folder, bool loadSubFolder)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	hFind = FindFirstFile(folder.c_str(), &win32fd);
	if (hFind==INVALID_HANDLE_VALUE)
		assert(false);
	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// ディレクトリが見つかったら、再帰的にファイルを読みたい
		} else {
			std::string fname = win32fd.cFileName;
			// ファイルが見つかったので、unordered_mapで登録する
			// フォルダ名とか、ファイル名の相対表示に気を付けるよ
		}
	} while (FindNextFile(hFind, &win32fd) != 0);
}

void ResourceLoader::SetAsync(bool async)
{
	SetUseASyncLoadFlag(async);
}

bool ResourceLoader::IsLoading()
{
	return (GetASyncLoadNum() > 0);
}

void ResourceLoader::ReleaseAllFile()
{
	for(auto f : files) {
		switch (f.second.type) {
		case T_IMAGE:
			DeleteGraph(f.second.handle);
			break;
		case T_MODEL:
			MV1DeleteModel(f.second.handle);
			break;
		case T_SOUND:
			DeleteSoundMem(f.second.handle);
			break;
		}
	}
	files.clear();
}

#pragma once
#include <string>

namespace ResourceLoader {
	void Init();
	int LoadGraph(std::string filename);
	void DeleteGraph(int handle);
	int MV1LoadModel(std::string filename);
	void MV1DeleteModel(int handle);
	int LoadSoundMem(std::string filename);
	void DeleteSoundMem(int handle);
	void LoadFolderFile(std::string folder, bool loadSubFolder=false);
	void SetAsync(bool async);
	bool IsLoading();
	void ReleaseAllFile();
};
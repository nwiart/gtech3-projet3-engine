#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>

#include "RessourceManager.h"


RessourceManager::RessourceManager()
{
}

RessourceManager::~RessourceManager()
{
}

RessourcePtr RessourceManager::createRessourceFromFile(const wchar_t* file_path)
{
	std::wstring full_path = std::experimental::filesystem::absolute(file_path);

	auto it = m_map_ressource.find(full_path);
	if (it != m_map_ressource.end())
	{
		return it->second;
	}

	Ressource* raw_res = this->createRessourceFromFileConcrete(full_path.c_str());

	if (raw_res)
	{
		RessourcePtr res(raw_res);
		m_map_ressource[full_path] = res;
		return res;
	}
	return nullptr;
}

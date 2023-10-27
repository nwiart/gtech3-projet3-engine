#pragma once

#include <unordered_map>
#include <string>
#include "Prerequisites.h"

class RessourceManager
{
public : 
	RessourceManager();
	~RessourceManager();

	RessourcePtr createRessourceFromFile(const wchar_t * file_path);

protected:
	virtual Ressource* createRessourceFromFileConcrete(const wchar_t * file_path) = 0;

private:	
	std::unordered_map<std::wstring, RessourcePtr> m_map_ressource;
};


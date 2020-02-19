#pragma once

#include "Auxiliary.h"
#include <string>

class Encryptor
{

public:

	Encryptor() = default;

	void Encrypt(const std::string& path, 
		const std::string& path_key, 
		const std::string& output_path, 
		bool display = true);
};

#pragma once

#include "Auxiliary.h"

class Decryptor
{

public:

	Decryptor() = default; 

	void Decrypt(const std::string& path, 
		const std::string& path_key, 
		const std::string& output_path, 
		bool display, bool weak = false);
};

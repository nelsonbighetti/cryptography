#pragma once
#include "Auxiliary.h"
#include <time.h>
#include <vector>
#include <mutex>

class BruteforceAsync
{
	std::mutex mut;
	std::string keyFound;
	bool stopFlag;

public:

	void Execute(uint32_t start, uint32_t end, std::string first_block_plaintext, std::string first_block_crypted_text, bool display);

	std::string GetKey();

	BruteforceAsync() : stopFlag(false) {}
};

class Bruteforcer
{
	uint32_t MAX = 4294967295;

public:

	void Run(const std::string& path_plaintext, const std::string& path_crypted_text, const std::string& found_key_path, const std::string& decrypted_path, bool display, int threads);

	Bruteforcer() = default;
};


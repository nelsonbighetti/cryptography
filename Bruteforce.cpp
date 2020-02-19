#pragma once
#include "Bruteforce.h"


void BruteforceAsync::Execute(uint32_t start, uint32_t end, std::string first_block_plaintext, std::string first_block_crypted_text, bool display)
{
	{
		const std::lock_guard<std::mutex> lock(mut);
		std::cout << "Thr : " << start << " : " << end << " start" << std::endl;
	}

	clock_t start_time = clock();
	std::string new_key;

	for (uint32_t j = start; j < end; j++)
	{
		std::bitset<32> k(j);
		new_key = aux::Bitset32ToString(k);

		if (display)
		{
			const std::lock_guard<std::mutex> lock(mut);
			std::cout << std::endl << j << " - " << new_key;
		}


		if (first_block_crypted_text == aux::EncryptBlock(first_block_plaintext, new_key))
		{
			const std::lock_guard<std::mutex> lock(mut);
			std::cout << " -> This key (" << new_key << ") is suitable for decrypting" << std::endl \
				<< "Key search time = " << (clock() - start_time) / 1000.0 << " sec" << std::endl;
			keyFound = new_key;
			stopFlag = true;
			return;
		}

		if (j == (end - start) / 4)
		{
			const std::lock_guard<std::mutex> lock(mut);
			std::cout << "Thr : " << start << " : " << end << " 1/4 done" << std::endl;
		}

		if (j == (end - start) / 2)
		{
			const std::lock_guard<std::mutex> lock(mut);
			std::cout << "Thr : " << start << " : " << end << " 1/2 done" << std::endl;
		}

		if (stopFlag)
			return;
	}

	{
		const std::lock_guard<std::mutex> lock(mut);
		std::cout << "Thr : " << start << " : " << end << " stop" << std::endl;
		std::cout << std::endl << "Key not found" << std::endl;
	}
}

std::string BruteforceAsync::GetKey()
{
	return keyFound;
}

void Bruteforcer::Run(const std::string& path_plaintext, const std::string& path_crypted_text, const std::string& found_key_path, const std::string& decrypted_path, bool display, int threads)
{
	const std::string plaintext = aux::ReadFile(path_plaintext);
	if (plaintext.size() < 4)
	{
		std::cout << "The plaintext length must be 4 characters or more" << std::endl;
		return;
	}

	if (display)
		std::cout << "plaintext:" << std::endl << plaintext << std::endl;

	const std::string crypted_text = aux::ReadFile(path_crypted_text);
	if (crypted_text.size() < 8 || crypted_text.size() % 4)
	{
		std::cout << "Crypted text length must be 8 characters or more" << std::endl;
		return;
	}

	if (display)
		std::cout << "Crypted_text:" << std::endl << plaintext << std::endl;

	const std::string first_block_plaintext = plaintext.substr(0, 4);
	const std::string first_block_crypted_text = crypted_text.substr(0, 4);


	std::vector<std::thread> threadsVec;
	BruteforceAsync* taskPtr = new BruteforceAsync();

	for (int i = 0; i < threads; i++)
	{

		uint32_t start = i * (MAX / threads);
		uint32_t end = i * (MAX / threads) + (MAX / threads) - 1;
		if (i == threads - 1)
			end = MAX;

		threadsVec.push_back(std::thread(&BruteforceAsync::Execute, taskPtr, start, end, first_block_plaintext, first_block_crypted_text, display));
	}

	for (auto it = threadsVec.begin(); it != threadsVec.end(); it++)
		it->join();

	const std::string keyFound = taskPtr->GetKey();

	if (keyFound == "")
	{
		std::cout << "Empty key" << std::endl;
		return;
	}

	std::string decrypted_text;
	for (auto it = crypted_text.begin(); it != crypted_text.end(); it += 4)
	{
		std::string block = crypted_text.substr(it - crypted_text.begin(), 4);
		decrypted_text += aux::DecryptBlock(block, keyFound);
	}

	aux::Truncate(decrypted_text);

	if (display)
		std::cout << std::endl << "Decrypted text:" << std::endl << decrypted_text << std::endl;

	aux::WriteFile(found_key_path, keyFound);
	aux::WriteFile(decrypted_path, decrypted_text);

	std::cout << "Done" << std::endl;
}

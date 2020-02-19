#pragma once

#include <iostream>
#include <string>
#include <sstream> 
#include <fstream>
#include <bitset>

namespace aux
{
	const unsigned char sub[16] = { 4,10,9,2,13,8,0,14,6,11,1,12,7,15,5,3 };

	std::string BitsetToString(const std::bitset<16>& input);

	std::bitset<16> StringToBitset(const std::string& input);

	std::string Bitset32ToString(const std::bitset<32>& input);

	std::bitset<32> StringToBitset32(const std::string& input);

	std::string ReadFile(const std::string& filename);

	void WriteFile(const std::string& filename, const std::string& data);

	std::bitset<16> HalfRoundFunc(const std::bitset<16>& first, const std::bitset<16>& second, const std::bitset<16>& key);

	std::bitset<16> SP(std::bitset<16> right_block, const std::bitset<16> key);

	std::string Feistel(const std::string& openblock, const std::string& key, bool flag);

	std::string RightBlock(const std::string& left_block, const std::string& right_block, const std::string& key);

	std::string LaiMassey(const std::string& openblock, const std::string& key);

	void Truncate(std::string& crypted_text);

	std::string DecryptBlock(const std::string& crypted_block, const std::string& key);

	std::string FeistelDec(const std::string& crypted_block, const std::string& key, bool flag);

	void AddLastBlock(std::string& opentext);

	std::string EncryptBlock(const std::string& openblock, const std::string& key);
}

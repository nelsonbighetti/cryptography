#include "Auxiliary.h"

namespace aux
{
	std::string ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);
		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	void WriteFile(const std::string& filename, const std::string& data)
	{
		std::ofstream file(filename, std::ios::binary);
		file.clear();
		file << data;
		file.close();
	}

	std::string BitsetToString(const std::bitset<16>& input)
	{
		uint32_t val = input.to_ulong();
		return std::string((char*)&val, 2);
	}

	std::bitset<16> StringToBitset(const std::string& input)
	{
		uint32_t val = (*((uint32_t*)input.c_str()));
		return std::bitset<16>(val);
	}

	std::string Bitset32ToString(const std::bitset<32>& input)
	{
		uint32_t val = input.to_ulong();
		return std::string((char*)&val, 4);
	}

	std::bitset<32> StringToBitset32(const std::string& input)
	{
		uint32_t val = (*((uint32_t*)input.c_str()));
		return std::bitset<32>(val);
	}

	std::bitset<16> HalfRoundFunc(const std::bitset<16>& first, const std::bitset<16>& second, const std::bitset<16>& key)
	{
		std::bitset<16> tmp = first ^ second;
		tmp = SP(tmp, key);
		return first ^ tmp;
	}

	std::bitset<16> SP(std::bitset<16> right_block, const std::bitset<16> key)
	{
		std::bitset<4> new_right;
		std::bitset<16> tmp_result, result;

		right_block ^= key;

		//substisution 
		for (char i = 0; i < 4; i++)
		{
			for (char j = 0; j < 4; j++)
			{
				new_right[j] = right_block[4 * i + j];
			}

			unsigned long tmp = new_right.to_ulong();
			tmp = sub[tmp];

			std::bitset<4> tmp_bitset(tmp);

			for (int j = 0; j < 4; j++)
			{
				tmp_result[4 * i + j] = tmp_bitset[j];
			}
		}

		//permutation 
		for (int i = 0; i < 16; i++)
		{
			result[(13 * i + 7) % 16] = tmp_result[i];
		}

		return result;
	}

	std::string RightBlock(const std::string& left_block, const std::string& right_block, const std::string& key)
	{
		std::bitset<16> sp_right_block = SP(aux::StringToBitset(right_block), aux::StringToBitset(key));
		std::bitset<16> tmp = aux::StringToBitset(left_block);
		auto result = tmp ^ sp_right_block;
		return aux::BitsetToString(result);
	}

	std::string Feistel(const std::string& openblock, const std::string& key, bool flag)
	{
		std::string left_block = openblock.substr(0, 2);
		std::string right_block = openblock.substr(2, 2);

		if(flag)
			return right_block + RightBlock(left_block, right_block, key);
		else
			return RightBlock(left_block, right_block, key)+right_block;
	}

	std::string LaiMassey(const std::string& openblock, const std::string& key)
	{
		std::bitset<16> left_block = aux::StringToBitset(openblock.substr(0, 2));
		std::bitset<16> right_block = aux::StringToBitset(openblock.substr(2, 2));

		std::string left = aux::BitsetToString(HalfRoundFunc(left_block, right_block, aux::StringToBitset(key)));
		std::string right = aux::BitsetToString(HalfRoundFunc(right_block, left_block, aux::StringToBitset(key)));

		return left + right;
	}

	void Truncate(std::string& crypted_text)
	{
		char count = *(crypted_text.rbegin());
		if (count >= 0 && count <= 3) count ? count = 8 - count : count = 4;
		else count = 4;
		crypted_text.resize(crypted_text.size() - count);
	}

	std::string DecryptBlock(const std::string& crypted_block, const std::string& key)
	{
		const std::string first_key = key.substr(0, 2);
		const std::string second_key = key.substr(2, 2);
		std::string result = crypted_block;

		result = LaiMassey(result, second_key);
		result = LaiMassey(result, first_key);
		result = FeistelDec(result, second_key, 0);
		result = FeistelDec(result, first_key, 1);

		return result;
	}

	std::string FeistelDec(const std::string& crypted_block, const std::string& key, bool flag)
	{
		std::string left_block = crypted_block.substr(2, 2);
		std::string right_block = crypted_block.substr(0, 2);
		
		if(flag)
			return aux::RightBlock(left_block, right_block, key) + right_block;
		else
			return right_block+aux::RightBlock(left_block, right_block, key);
	}

	void AddLastBlock(std::string& plaintext)
	{
		int addition = plaintext.size() % 4;
		if (addition)
			plaintext.resize(plaintext.size() + 4 - addition, 0);
		plaintext.resize(plaintext.size() + 3, 0);
		plaintext.push_back(addition);
	}

	std::string EncryptBlock(const std::string& openblock, const std::string& key)
	{
		const std::string first_key = key.substr(0, 2);
		const std::string second_key = key.substr(2, 2);
		std::string result = openblock;

		result = Feistel(result, first_key, 0);
		result = Feistel(result, second_key, 1);
		result = LaiMassey(result, first_key);
		result = LaiMassey(result, second_key);

		return result;
	}
}
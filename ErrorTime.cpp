#include "ErrorTime.h"

bool ErrorMultiplied(const int round, const std::bitset<32>& block, const std::bitset<32>& error_block, const std::string& first_key, const std::string& second_key)
{
	//Зашифрование текста и текста с ошибкой на одном ключе, XOR
	//Поочередное выполнение 4 тактов, побитовое "ИЛИ" для ошибки 
	std::string result_block = aux::Feistel(aux::Bitset32ToString(block), first_key,0);
	std::string result_error_block = aux::Feistel(aux::Bitset32ToString(error_block), first_key, 0);
	std::bitset<32> error_accumulation = aux::StringToBitset32(result_block) ^ aux::StringToBitset32(result_error_block);
	std::cout << "1st round : " << error_accumulation << std::endl;

	if (round < 2)
	{
		std::cout << "Accumulated error : " << error_accumulation << std::endl;
		return error_accumulation.count() == 32;
	}

	result_block = aux::Feistel(result_block, second_key, 1);
	result_error_block = aux::Feistel(result_error_block, second_key, 1);
	error_accumulation |= aux::StringToBitset32(result_block) ^ aux::StringToBitset32(result_error_block);
	std::cout << "2nd round : " << error_accumulation << std::endl;

	if (round < 3)
	{
		std::cout << "Accumulated error : " << error_accumulation << std::endl;
		return error_accumulation.count() == 32;
	}

	result_block = aux::LaiMassey(result_block, first_key);
	result_error_block = aux::LaiMassey(result_error_block, first_key);
	error_accumulation |= aux::StringToBitset32(result_block) ^ aux::StringToBitset32(result_error_block);
	std::cout << "3rd round : " << error_accumulation << std::endl;

	if (round < 4)
	{
		std::cout << "Accumulated error : " << error_accumulation << std::endl;
		return error_accumulation.count() == 32;
	}

	result_block = aux::LaiMassey(result_block, second_key);
	result_error_block = aux::LaiMassey(result_error_block, second_key);
	error_accumulation |= aux::StringToBitset32(result_block) ^ aux::StringToBitset32(result_error_block);
	std::cout << "4th round : " << error_accumulation << std::endl;

	std::cout << "Accumulated error : " << error_accumulation << std::endl;
	return error_accumulation.count() == 32;
}

int ErrorTime(int round, const char* c_key, int value, int bit_start)
{
	/*
		рекурсивный перебор раундов
	*/

	std::string key(c_key);
	std::string first_key = key.substr(0, 2);
	std::string second_key = key.substr(2, 2);

	round++;
	if (round > 4)
		return 0;
	std::cout << "---------------\n" << "Round " << round << std::endl;

	if (bit_start < 32 && bit_start >=0 )
		std::cout << "Fixed : " << bit_start << " bit\n";
	else
		std::cout << "Error : incorrect bit" << std::endl;

	if (value < 4294967299 && value >= 0)
		std::cout << "Value : " << value << " bit\n";
	else
		std::cout << "Error : incorrect value" << std::endl;

	// Составление 2 открытых текстов, с отличием в i-ом бите 
	std::bitset<32>block(value);
	std::bitset<32>error_block(value);

	// Выставление ошибочного бита 
	error_block.set(bit_start, !block[bit_start]);

	// Вывод 
	std::cout << "Block (" << block << ") and block with error in " << bit_start << " bit (" << error_block << ")" << std::endl;
	if (!ErrorMultiplied(round, block, error_block, first_key, second_key))
	{
		std::cout << "Error : accumulation is not 32bits"
			<< "\nFound insignificant variable : " << bit_start
			<< "\nError propagation time for this cryptosystem is more than : " << round << "\n\n";
		return ErrorTime(round, c_key, value, bit_start);
	}

	return round;
}
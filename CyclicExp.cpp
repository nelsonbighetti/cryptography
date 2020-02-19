#include "CyclicExp.h"
#include <ctime> 

void CyclicalExperiments(const std::string& cyclePath, bool display)
{
	srand(time(0));
	std::ofstream out;
	out.open(cyclePath);

	if (!out.is_open())
	{
		std::cout << "File opening error" << std::endl;
		return;
	}

	out << "i\tX0\t\tkey1\t\tkey2\t\tT\t\tTime\n";
	out.close();

	std::string Xt, X0, key1;
	for (int i = 0; i < 10; i++)
	{
		unsigned int t = 0;
		key1 = GenerateKey();
		X0 = GenerateX0();
		Xt = X0;
		clock_t start = clock();
		do
		{
			Xt = aux::EncryptBlock(Xt, key1);
			Xt = aux::EncryptBlock(Xt, key1);
			t++;
		} while (X0 != Xt && t != 0);

		out.open(cyclePath, std::ios::app);
		if (!out.is_open()) 
		{ 
			std::cout << "File opening error" << std::endl; 
			return; 
		}

		out << i << "\t" << StringToHex(X0) << "\t" << StringToHex(key1) << "\t" << "\t" << t << "\t" << clock() - start << std::endl;
		
		if(display)
			std::cout << i << "\t" << StringToHex(X0) << "\t" << StringToHex(key1) << "\t"  << "\t" << t << "\t" << (clock() - start) / 1000 << std::endl;

		out.close();
	}
}

std::string GenerateKey()
{
	std::string key;
	for (int i = 0; i < 2; i++)
		key.push_back(rand() % 256);

	key = key + key;
	return key;
}

std::string GenerateX0()
{
	std::string X0;
	for (int i = 0; i < 4; i++)
		X0.push_back(rand() % 256);

	return X0;
}

std::string StringToHex(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
		output.push_back(' ');
	}
	return output;
}
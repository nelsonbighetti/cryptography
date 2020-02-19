#include "Encryption.h"

void Encryptor::Encrypt(const std::string& path, 
	const std::string& path_key, 
	const std::string& output_path,
	bool display)
{
	const std::string key = aux::ReadFile(path_key);
	if (key.size() != 4)
	{
		std::cout << "Key length must be 4 characters" << std::endl;
		return;
	}

	std::string plain_text = aux::ReadFile(path), crypted_text;
	
	if (display)
		std::cout << "Open text : \n\n" << plain_text << std::endl;

	aux::AddLastBlock(plain_text);

	for (auto it = plain_text.begin(); it != plain_text.end(); it += 4)
	{
		std::string block = plain_text.substr(it - plain_text.begin(), 4);
		crypted_text += aux::EncryptBlock(block, key);
	}

	if(display)
		std::cout << std::endl << "Crypted text : \n" << crypted_text << std::endl;

	aux::WriteFile(output_path, crypted_text);

	std::cout << "\nDone." << std::endl;
}

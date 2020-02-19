#include "Decryption.h"


void Decryptor::Decrypt(const std::string& path,
	const std::string& path_key,
	const std::string& output_path,
	bool display, bool weak)
{
	const std::string key = aux::ReadFile(path_key);
	if (key.size() != 4)
	{
		std::cout << "Key length must be 4 characters" << std::endl;
		return;
	}

	std::string crypted_text = aux::ReadFile(path), decrypted_text;

	if (weak && (crypted_text.size() % 4))
	{
		if (crypted_text.size() > 4)
		{
			std::cout << "Size of crypted text is not dividing to 4, truncating..." << std::endl;
			crypted_text.resize(crypted_text.size() - (crypted_text.size() % 4));
		}
		else
		{
			std::cout << "Size of crypted text is less than 4, adding zeros..." << std::endl;
			crypted_text.resize(crypted_text.size() + (4-(crypted_text.size() % 4)), 0);
		}
	}


	if (display)
		std::cout << "Crypted text : \n\n" << crypted_text << std::endl;

	for (auto it = crypted_text.begin(); it != crypted_text.end(); it += 4)
	{
		std::string block = crypted_text.substr(it - crypted_text.begin(), 4);
		decrypted_text += aux::DecryptBlock(block, key);
	}

	if(!weak)
		aux::Truncate(decrypted_text);

	if(display)
		std::cout << std::endl << "\tDecrypted text:" << std::endl << decrypted_text << std::endl;

	aux::WriteFile(output_path, decrypted_text);

	std::cout << "\nDone." << std::endl;
}

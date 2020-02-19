#include "CBC.h"

void EncryptionCBC(const std::string& plaintext_path, const std::string& key_path, const std::string& sync_pack_path, const std::string& output_path, bool display)
{
	const std::string key = aux::ReadFile(key_path);
	if (key.size() != 4)
	{
		std::cout << "The key length must be 4 characters" << std::endl;
		return;
	}

	std::string plaintext = aux::ReadFile(plaintext_path), crypted_text;

	if(display)
		std::cout << "Plain text:" << std::endl << plaintext << std::endl;

	const std::string sync_pack = aux::ReadFile(sync_pack_path);
	if (sync_pack.size() != 4)
	{
		std::cout << "sync_pack length must be 4 characters" << std::endl;
		return;
	}

	aux::AddLastBlock(plaintext);

	std::string Yi = sync_pack;
	for (auto it = plaintext.begin(); it != plaintext.end(); it += 4)
	{
		auto block = plaintext.substr(it - plaintext.begin(), 4);
		auto bl_xor = aux::Bitset32ToString(aux::StringToBitset32(block) ^ aux::StringToBitset32(Yi));
		Yi = aux::EncryptBlock(bl_xor, key);
		crypted_text += Yi;
	}
	
	if (display)
		std::cout << std::endl << "Crypted text:" << std::endl << crypted_text << std::endl;

	aux::WriteFile(output_path, crypted_text);

	std::cout << std::endl << "Encryption done" << std::endl;
}

void DecryptionCBC(const std::string& crypted_path, const std::string& key_path, const std::string& sync_pack_path, const std::string& output_path, bool display)
{
	const std::string key = aux::ReadFile(key_path);
	if (key.size() != 4)
	{
		std::cout << "The key length must be 4 characters" << std::endl;
		return;
	}

	std::string crypted_text = aux::ReadFile(crypted_path), decrypted_text;
	
	if (display)
		std::cout << "Crypted text:" << std::endl << crypted_text << std::endl;

	const std::string sync_pack = aux::ReadFile(sync_pack_path);
	if (sync_pack.size() != 4)
	{
		std::cout << "sync_pack length must be 4 characters" << std::endl;
		return;
	}

	std::string Yi = sync_pack, F_Yj;
	for (auto it = crypted_text.begin(); it != crypted_text.end(); it += 4)
	{
		std::string block = crypted_text.substr(it - crypted_text.begin(), 4);
		F_Yj = aux::DecryptBlock(block, key);
		decrypted_text += aux::Bitset32ToString(aux::StringToBitset32(Yi) ^ aux::StringToBitset32(F_Yj));
		Yi = block;
	}

	aux::Truncate(decrypted_text);
	
	if (display)
		std::cout << std::endl << "Decrypted text:" << std::endl << decrypted_text << std::endl;

	aux::WriteFile(output_path, decrypted_text);

	std::cout << std::endl << "Decryption done" << std::endl;
}
#include "Encryption.h"
#include "Decryption.h"
#include "ErrorTime.h"
#include "Bruteforce.h"
#include "CBC.h"
#include "CyclicExp.h"

enum class LabNumber
{
	L2ENCRYPT,
	L2DECRYPT, 
	L2ERROR,
	L2WEAKDEC,
	L3,
	L4ENCRYPT,
	L4DECRYPT,
	L5,
	UNDEFINED
};

struct argsParsed
{
	std::vector<std::string> argsVec; 
	LabNumber number = LabNumber::UNDEFINED;
	bool verbose = false; 
};

bool showHelp()
{
	std::cout << "Usage:\n" << std::endl;
	std::cout << "l2 -enc plain_text_path path_key crypted_text_path [-verbose]" << std::endl;
	std::cout << "l2 -dec crypted_text_path path_key decrypted_text_path [-verbose]" << std::endl;
	std::cout << "l2 -decweak crypted_text_path path_key decrypted_text_path [-verbose]" << std::endl;
	std::cout << "l2 -err key_path value_start(0-4294967295) bit_start(0-31) [-verbose]" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "l3 plain_text_path path_crypted_text found_key_path path_decrypted_text [-verbose]" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "l4 -enc plain_text_path key_path sync_pack_path crypted_text_path [-verbose]" << std::endl;
	std::cout << "l4 -dec crypted_text_path key_path wrong_sync_pack_path decrypted_text_path [-verbose]" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "l5 cyc_path [-verbose]" << std::endl;
	return false;
}

bool parseArgs(int argc, char** argv, argsParsed& args)
{
	if (argc < 3)
		return showHelp();

	std::vector<std::string> argsVecSource; 
	for (int i = 0; i < argc; i++)
		argsVecSource.push_back(argv[i]);

	if (argsVecSource[1] == "l2")
	{
		if (argsVecSource[2] == "-err")
		{
			args.number = LabNumber::L2ERROR;
			if (argc < 6)
				return showHelp();

			for (int i = 3; i < 6; i++)
				args.argsVec.push_back(argsVecSource[i]);

			if (argc == 7 && argsVecSource[6] == "-verbose")
				args.verbose = true;

			return true; 
		}
		if (argsVecSource[2] == "-enc" || argsVecSource[2] == "-dec" || argsVecSource[2] == "-decweak")
		{
			if (argsVecSource[2] == "-enc")
				args.number = LabNumber::L2ENCRYPT;
			else if(argsVecSource[2] == "-dec")
				args.number = LabNumber::L2DECRYPT;
			else
				args.number = LabNumber::L2WEAKDEC;

			if(argc<6)
				return showHelp();
			
			for (int i = 3; i < 6; i++)
				args.argsVec.push_back(argsVecSource[i]);

			if (argc == 7 && argsVecSource[6] == "-verbose")
				args.verbose = true;

			return true; 
		}
	}
	else if (argsVecSource[1] == "l3")
	{
		args.number = LabNumber::L3;
		if (argc < 6)
			return showHelp();

		for (int i = 2; i < 6; i++)
			args.argsVec.push_back(argsVecSource[i]);

		if (argc == 7 && argsVecSource[6] == "-verbose")
			args.verbose = true;
		
		return true;
	}
	else if (argsVecSource[1] == "l4")
	{
		if (argsVecSource[2] == "-enc" || argsVecSource[2] == "-dec")
		{
			if (argsVecSource[2] == "-enc")
				args.number = LabNumber::L4ENCRYPT;
			else
				args.number = LabNumber::L4DECRYPT;

			if (argc < 7)
				return showHelp();

			for (int i = 3; i < 7; i++)
				args.argsVec.push_back(argsVecSource[i]);

			if (argc == 8 && argsVecSource[7] == "-verbose")
				args.verbose = true;

			return true;
		}
	}
	else if (argsVecSource[1] == "l5")
	{
		args.number = LabNumber::L5;

		if (argc < 3)
			return showHelp();

		args.argsVec.push_back(argsVecSource[2]);

		if (argc == 4 && argsVecSource[3] == "-verbose")
			args.verbose = true;

		return true;
	}
	else
		return showHelp(); 
}

void L2EncRun(argsParsed args)
{
	std::cout << "L2EncRun started" << std::endl; 
	std::string plain_text_path = args.argsVec[0];
	std::string path_key = args.argsVec[1];
	std::string crypted_text_path = args.argsVec[2];

	Encryptor enc_text;
	enc_text.Encrypt(plain_text_path, path_key, crypted_text_path, args.verbose);
}

void L2DecRun(argsParsed args)
{
	std::cout << "L2DecRun started" << std::endl;
	std::string crypted_text_path = args.argsVec[0];
	std::string path_key = args.argsVec[1];
	std::string decrypted_text_path = args.argsVec[2];

	Decryptor dec_text;
	dec_text.Decrypt(crypted_text_path, path_key, decrypted_text_path, args.verbose);
}

void L2WeakDecRun(argsParsed args)
{
	std::cout << "L2WeakDecRun started" << std::endl;
	std::string crypted_text_path = args.argsVec[0];
	std::string path_key = args.argsVec[1];
	std::string decrypted_text_path = args.argsVec[2];

	Decryptor dec_text;
	dec_text.Decrypt(crypted_text_path, path_key, decrypted_text_path, args.verbose, true);
}

void L2ErrorRun(argsParsed args)
{
	std::cout << "L2ErrorRun started" << std::endl;
	std::string key_path = args.argsVec[0];
	int value_start = std::stoi(args.argsVec[1]);
	int bit_start = std::stoi(args.argsVec[2]);

	const std::string key = aux::ReadFile(key_path);
	std::cout << "Key : " << key << std::endl; 
	ErrorTime(0, key.c_str(), value_start, bit_start);
}

void L3Run(argsParsed args)
{
	std::cout << "L3Run started" << std::endl;
	std::string plain_text_path = args.argsVec[0];
	std::string path_crypted_text = args.argsVec[1];
	std::string found_key_path = args.argsVec[2];
	std::string path_decrypted_text = args.argsVec[3];

	Bruteforcer bf;
	bf.Run(plain_text_path, path_crypted_text, found_key_path, path_decrypted_text, args.verbose, 8);
}

void L4EncRun(argsParsed args)
{
	std::cout << "L4EncRun started" << std::endl;
	std::string plain_text_path = args.argsVec[0];
	std::string key_path = args.argsVec[1];
	std::string sync_pack_path = args.argsVec[2];
	std::string crypted_text_path = args.argsVec[3];

	EncryptionCBC(plain_text_path, key_path, sync_pack_path, crypted_text_path, args.verbose);
}

void L4DecRun(argsParsed args)
{
	std::cout << "L4DecRun started" << std::endl;
	std::string crypted_text_path = args.argsVec[0];
	std::string key_path = args.argsVec[1];
	std::string wrong_sync_pack_path = args.argsVec[2];
	std::string decrypted_text_path = args.argsVec[3];

	DecryptionCBC(crypted_text_path, key_path, wrong_sync_pack_path, decrypted_text_path, args.verbose);
}

void L5Run(argsParsed args)
{
	std::cout << "L5Run started" << std::endl;
	std::string cyc_path = args.argsVec[0];
	CyclicalExperiments(cyc_path, args.verbose);
}

int main(int argc, char** argv)
{
	argsParsed args; 
	if (parseArgs(argc, argv, args))
	{
		switch (args.number)
		{
		case LabNumber::L2ENCRYPT:
			L2EncRun(args);
			break;

		case LabNumber::L2DECRYPT:
			L2DecRun(args);
			break;

		case LabNumber::L2WEAKDEC:
			L2WeakDecRun(args);
			break;

		case LabNumber::L2ERROR:
			L2ErrorRun(args);
			break;

		case LabNumber::L3:
			L3Run(args);
			break;

		case LabNumber::L4ENCRYPT:
			L4EncRun(args);
			break;

		case LabNumber::L4DECRYPT:
			L4DecRun(args);
			break;

		case LabNumber::L5:
			L5Run(args);
			break;

		default:
			std::cout << "Unknown error" << std::endl; 
		}
	}
	system("pause");
	return 0;
}
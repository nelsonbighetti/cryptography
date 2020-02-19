#include "Auxiliary.h"

void EncryptionCBC(const std::string& plaintext_path, const std::string& key_path, const std::string& sync_pack_path, const std::string& output_path, bool display);
void DecryptionCBC(const std::string& crypted_path, const std::string& key_path, const std::string& sync_pack_path, const std::string& output_path, bool display);
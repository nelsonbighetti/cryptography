# cryptography

#Usage:

Encryption/Decryption: 

l2 -enc plain_text_path path_key crypted_text_path [-verbose]
l2 -dec crypted_text_path path_key decrypted_text_path [-verbose]

Decryption with weak key:
l2 -decweak crypted_text_path path_key decrypted_text_path [-verbose]

Obtaining of error propagation time 
l2 -err key_path value_start(0-4294967295) bit_start(0-31) [-verbose]

Bruteforce
l3 plain_text_path path_crypted_text found_key_path path_decrypted_text [-verbose]

Encryption/Decryption with sync package: 
l4 -enc plain_text_path key_path sync_pack_path crypted_text_path [-verbose]
l4 -dec crypted_text_path key_path wrong_sync_pack_path decrypted_text_path [-verbose]

Cyclical experiments: 
l5 cyc_path [-verbose]
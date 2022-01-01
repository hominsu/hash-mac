//
// Created by Homin Su on 2021/10/28.
//

#include "../../crypt/des_encrypt_cbc.h"

#include <iostream>
#include <memory>

int main(int _argc, char *_argv[]) {
  crypt::DesCBC crypt;
  uint64_t iv = UINT64_MAX;

  ::std::string in_str(_argv[2]);

  crypt.Init(_argv[1], &iv);

  auto arr = crypt.sub_key_;

  char cipher_text[1024]{'\0'};
  auto encrypt_size = crypt.Encrypt(in_str.c_str(), in_str.size(), cipher_text, true);
  ::std::cout << "encrypt_size = " << encrypt_size << ", encrypt content = \"" << cipher_text << "\"" << ::std::endl;

  crypt.Init(_argv[1], &iv);

  char plain_text[1024]{'\0'};
  auto decrypt_size = crypt.Decrypt(cipher_text, encrypt_size, plain_text, true);
  ::std::cout << "decrypt_size = " << decrypt_size << ", decrypt content = \"" << plain_text << "\"" << ::std::endl;


  return 0;
}

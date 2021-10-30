//
// Created by Homin Su on 2021/10/28.
//

#include <iostream>
#include <memory>

#include "../../des/des.h"

int main(int _argc, char *_argv[]) {
  des::Des des;
  des.Init(_argv[1]);

  char cipher_text[8];
  char plain_text[8];

  des.Encrypt(_argv[2], cipher_text);

  des.Decrypt(cipher_text, plain_text);

  std::cout << "plain_text: [";
  for (auto &c: plain_text) {
    std::cout << c;
  }
  std::cout << "]" << std::endl;

  return 0;
}

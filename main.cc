//
// Created by Homin Su on 2021/10/28.
//

#include <iostream>

#include "des/des.h"

int main() {
  des::Des des;
  des.Init("12345678");

  char cipher_text[8];
  char plain_text[8];

  des.Encrypt("hello!!!", cipher_text);

  des.Decrypt(cipher_text, plain_text);

  std::cout << "cipher_text: [";
  for (auto &c: cipher_text) {
    std::cout << c;
  }
  std::cout << "]" << std::endl;

  std::cout << "plain_text: [";
  for (auto &c: plain_text) {
    std::cout << c;
  }
  std::cout << "]" << std::endl;

  return 0;
}

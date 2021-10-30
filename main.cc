//
// Created by Homin Su on 2021/10/28.
//

#include <iostream>
#include <list>
#include <filesystem>

int main(int _argc, char *_argv[]) {
  if (_argc != 5) {
    std::cerr << "para error!" << std::endl;
    std::cout << "\tEncrypt folder: " << _argv[0] << " -e src_dir dst_dir password" << std::endl;
    std::cout << "\tDecrypt folder: " << _argv[0] << " -d src_dir dst_dir password" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string option = _argv[1];    // 加解密选项
  std::string in_file = _argv[2];   // 输入文件夹
  std::string out_file = _argv[3];  // 输入文件夹
  std::string password = _argv[4];  // 密码

  bool is_encrypt;
  if ("-e" == option) {
    is_encrypt = true;
  } else if ("-d" == option) {
    is_encrypt = false;
  }

  std::filesystem::is_regular_file(in_file);

  return 0;
}

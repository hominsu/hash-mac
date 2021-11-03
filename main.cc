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
  std::string src_dir = _argv[2];   // 输入文件夹
  std::string dst_dir = _argv[3];   // 输入文件夹
  std::string password = _argv[4];  // 密钥

  bool is_encrypt;
  if ("-e" == option) {
    is_encrypt = true;
  } else if ("-d" == option) {
    is_encrypt = false;
  }

  // 源文件目录是否存在
  if (!std::filesystem::exists(src_dir)) {
    std::cerr << "src_dir not exist" << std::endl;
    return EXIT_FAILURE;
  }

  // 创建输出文件夹
  if (!std::filesystem::exists(dst_dir)) {
    std::filesystem::create_directories(dst_dir);
  }

  // 文件列表
  auto files_entry = std::list<std::filesystem::directory_entry>();

  // 如果是文件就直接 append 到文件列表
  if (std::filesystem::is_regular_file(src_dir)) {
    files_entry.emplace_back(src_dir);
  } else if (std::filesystem::is_directory(src_dir)) {
    // 遍历目录中的文件
    for (auto &iter: std::filesystem::directory_iterator(src_dir)) {
      // 只处理文件
      if (!iter.is_regular_file()) {
        continue;
      }
      files_entry.push_back(iter);
    }
  }

  for (auto &file_it: files_entry) {
    std::cout << file_it.path().string() << std::endl;
  }

  return 0;
}

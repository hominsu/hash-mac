//
// Created by Homin Su on 2021/10/28.
//

#include <iostream>
#include <list>
#include <filesystem>

#include "task/file_crypt.h"

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

  // 线程安全的内存资源
  auto memory_resource = std::make_shared<std::pmr::synchronized_pool_resource>();

  // 文件列表
  auto file_crypt_list = std::list<std::shared_ptr<FileCrypt>>();

  // 如果是文件就直接 append 到文件列表
  if (std::filesystem::is_regular_file(src_dir)) {
    std::filesystem::directory_entry it(src_dir);

    auto file_crypt = std::make_shared<FileCrypt>();

    file_crypt->Start(it.path().string(),
                      dst_dir + "/" + it.path().filename().string(),
                      password,
                      is_encrypt,
                      memory_resource);
  } else if (std::filesystem::is_directory(src_dir)) {
    // 遍历目录中的文件
    for (auto &iter: std::filesystem::directory_iterator(src_dir)) {
      // 只处理文件
      if (!iter.is_regular_file()) {
        continue;
      }

      auto file_crypt = std::make_shared<FileCrypt>();

      file_crypt->Start(iter.path().string(),
                        dst_dir + "/" + iter.path().filename().string(),
                        password,
                        is_encrypt,
                        memory_resource);
    }
  }

  // 等待任务执行完成
  size_t task_num = 0;
  for (auto &file_crypt: file_crypt_list) {
    file_crypt->Wait();
    std::cout << ++task_num << ": in: [" << file_crypt->in_file_ << "], out: [" << file_crypt->out_file_ << "]"
              << std::endl;
  }

  return 0;
}

//
// Created by Homin Su on 2021/10/28.
//

#include "crypt/des_encrypt_ecb.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>

enum class Unit {
  Byte, KB, MB, GB
};

template<typename Ts>
double convert(Ts _size, Unit _unit) {
  auto result = static_cast<double>(_size);

  switch (_unit) {
    case Unit::GB: result /= 1024;
    case Unit::MB: result /= 1024;
    case Unit::KB: result /= 1024;
    case Unit::Byte: result /= 1;
    default:break;
  }

  return result;
}

int main(int _argc, char *_argv[]) {
  if (_argc != 5) {
    std::cerr << "param error!" << std::endl;
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

  if (dst_dir[dst_dir.size() - 1] != '/') {
    dst_dir += '/';
  }

  size_t read_bytes;
  size_t total_read_bytes = 0;

  // 任务开始执行时间
  auto start_time_point = std::chrono::system_clock::now();

  const size_t data_size = 1024 * 512;
  size_t data_bytes;
  auto in_buf = new char[data_size];
  auto out_buf = new char[data_size];
  bool is_end = false;

  std::ifstream ifs(src_dir, std::ios::binary);
  std::ofstream ofs(dst_dir, std::ios::binary);

  // 计算文件长度
  ifs.seekg(0, std::ios::end);
  data_bytes = ifs.tellg();
  ifs.seekg(0, std::ios::beg);


  crypt::DesECB des_ecb;
  des_ecb.Init(password);


  while (true) {
    if (ifs.eof()) {
      break;
    }

    // 读取文件
    ifs.read(in_buf, data_size);
    read_bytes = ifs.gcount();

    if (read_bytes <= 0) {
      break;
    }

    total_read_bytes += read_bytes;

    if (data_bytes == total_read_bytes) {
      is_end = true;
    } else {
      is_end = false;
    }

    auto en_size = des_ecb.Encrypt(in_buf, read_bytes, out_buf, is_end);
    ofs.write(out_buf, en_size);
  }

  ifs.close();
  ofs.close();
  delete[] in_buf;
  delete[] out_buf;

  auto usage_times = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now() - start_time_point).count();

  printf("\nUsage time: %lld ms\n", usage_times);
  printf("\tRead bytes:  %lf MB\n", convert(read_bytes, Unit::MB));

  auto megabytes_per_second = static_cast<double>(read_bytes) / (static_cast<double>(usage_times) / 1000);

  printf("\nSpeed: %lf MB/s, %lf mbps/s\n",
         convert(megabytes_per_second, Unit::MB),
         convert(megabytes_per_second, Unit::MB) * 8);

  return 0;
}

// 加密: build/Des -e test/src test/enc 123456
// 解密: build/Des -d test/enc test/dec 123456

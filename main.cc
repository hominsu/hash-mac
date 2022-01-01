//
// Created by Homin Su on 2021/10/28.
//

#include "crypt/des_encrypt_cbc.h"

#include <cstring>

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
    ::std::cerr << "param error!" << ::std::endl;
    ::std::cout << "\t" << _argv[0] << " length password src dst" << ::std::endl;
    exit(EXIT_FAILURE);
  }

  auto length = ::std::strtol(_argv[1], nullptr, 10); // 加解密选项
  ::std::string password = _argv[2];  // 密钥
  ::std::string src = _argv[3];   // 输入文件夹
  ::std::string dst = _argv[4];   // 输出文件夹

  // 源文件目录是否存在
  if (!::std::filesystem::exists(src)) {
    ::std::cerr << "src_dir: " << src << " not exist" << ::std::endl;
    return EXIT_FAILURE;
  }

  ::std::size_t read_bytes;
  ::std::size_t total_read_bytes = 0;

  // 任务开始执行时间
  auto start_time_point = ::std::chrono::system_clock::now();

  ::std::size_t data_size = 8;
  auto in_buf = new char[data_size];
  auto out_buf = new char[data_size];
  bool is_end;

  ::std::ifstream ifs(src, ::std::ios::binary);
  ::std::ofstream ofs(dst, ::std::ios::binary);

  // 计算文件长度
  ::std::size_t data_bytes;
  ifs.seekg(0, ::std::ios::end);
  data_bytes = ifs.tellg();
  ifs.seekg(0, ::std::ios::beg);

  crypt::DesCBC des_cbc;
  uint64_t iv = 0;
  des_cbc.Init(password, &iv);

  // 当长度生成 MAC 要求的长度大于 DES 的 Block Size 时, 需要将文件划分
  auto nums = length / des::kBlockSize / 8;
  auto batch_size = data_bytes / nums;

  for (int num = 0; num < nums; ++num) {
    ::std::size_t size;
    ::std::size_t batch_read_bytes = 0;
    while (batch_read_bytes < batch_size) {
      if (ifs.eof()) {
        break;
      }

      ::std::size_t read_size;
      if (batch_size - batch_read_bytes < data_size) {
        read_size = batch_size - batch_read_bytes;
      } else {
        read_size = data_size;
      }

      // 读取文件
      ifs.read(in_buf, read_size);
      read_bytes = ifs.gcount();

      if (read_bytes <= 0) {
        break;
      }

      batch_read_bytes += read_bytes;
      total_read_bytes += read_bytes;

      if (data_bytes == batch_read_bytes) {
        is_end = true;
      } else {
        is_end = false;
      }

      size = des_cbc.Encrypt(in_buf, read_bytes, out_buf, is_end);
    }
    ofs.write(out_buf, size);
  }

  ifs.close();
  ofs.close();
  delete[] in_buf;
  delete[] out_buf;

  auto usage_times = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
      ::std::chrono::system_clock::now() - start_time_point).count();

  printf("\nUsage time: %lld ms\n", usage_times);
  printf("\tRead bytes:  %lf MB\n", convert(total_read_bytes, Unit::MB));

  auto megabytes_per_second = static_cast<double>(total_read_bytes) / (static_cast<double>(usage_times) / 1000);

  printf("\nSpeed: %lf MB/s, %lf mbps/s\n",
         convert(megabytes_per_second, Unit::MB),
         convert(megabytes_per_second, Unit::MB) * 8);

  return 0;
}

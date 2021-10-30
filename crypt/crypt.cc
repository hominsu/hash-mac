//
// Created by Homin Su on 2021/10/30.
//

#include <cstring>

#include "crypt.h"
#include "des/des.h"

Crypt::Crypt() {
  des_ = std::make_shared<des::Des>();
}

Crypt::~Crypt() = default;

/**
 * @brief 初始化密钥
 * @param _password 8 字节密钥
 */
void Crypt::Init(const std::string &_password) {
  des_->Init(_password);
}

/**
 * @brief 加密一个数据块，加密到结尾填充数据
 * @detail 加密到结尾会使用 PKCS7 Padding 数据填充算法进行填充
 * @param _in_data 输入数据
 * @param _in_size 输入数据大小
 * @param _out_data 输出数据
 * @param _is_end 是否加密到结尾
 * @return 返回加密的数据大小，有可能大于输入（结尾处）
 */
size_t Crypt::Encrypt(const char *_in_data, size_t _in_size, char *_out_data, bool _is_end) {
  if (nullptr == _in_data || nullptr == _out_data || _in_size <= 0) {
    return 0;
  }

  char in_data[des::kBlockSize]{0};
  char out_data[des::kBlockSize]{0};

  auto padding_num = static_cast<int>(GetMaxPaddingSize(_in_size));// 填充数量，同时也是填充的内容，如果是 8 就填充 8
  auto padding_offset = _in_size % des::kBlockSize; // 填充位置

  size_t write_size = 0;  // 加密后的字节数

  size_t data_size; // 每次加密的数据大小

  for (size_t i = 0; i < _in_size; i += des::kBlockSize) {

    // 处理结尾处字节数小于 8 的情况
    if (_in_size - i < des::kBlockSize) {
      data_size = _in_size - i;
    } else {
      data_size = des::kBlockSize;
    }

    // 输入数据拷贝到 in 中
    memcpy(in_data, _in_data + write_size, data_size);

    // PKCS7 Padding
    // 缺多少个字节就填充相对应的大小，如果没缺，也要填充 8 个 8
    if (_is_end && i + des::kBlockSize >= _in_size) {  // 处理最后一块数据
      // ????????88888888
      if (padding_num == des::kBlockSize) {
        // 加密原来的数据
        des_->Encrypt(&in_data, &out_data);
        write_size += des::kBlockSize;

        memset(in_data, padding_num, sizeof(in_data));  // 填充 8
      } else {
        memset(in_data + padding_offset, padding_num, padding_num);
      }
    }

    // 加密
    des_->Encrypt(&in_data, &out_data);

    // 加密好的数据拷贝到 _out_data 中
    memcpy(_out_data + write_size, &out_data, des::kBlockSize);

    write_size += des::kBlockSize;
  }

  return write_size;
}

/**
 * @brief 解密数据，解密到结尾会删除填充数据
 * @param _in_data 输入数据
 * @param _in_size 输入数据大小
 * @param _out_data 输出数据
 * @param _is_end 是否加密到结尾
 * @return 返回解密后的数据，有可能小雨输入（结尾处）
 */
size_t Crypt::Decrypt(const char *_in_data, size_t _in_size, char *_out_data, bool _is_end) {
  if (nullptr == _out_data || nullptr == _in_data || _in_size <= 0) {
    return 0;
  }
  size_t write_size = 0;  // 加密后的字节数

  char in[8]{0};
  char out[8]{0};
  size_t data_size; // 每次加密的数据大小

  for (size_t i = 0; i < _in_size; i += des::kBlockSize) {
    // 输入数据拷贝到 in 中
    memcpy(in, _in_data + write_size, des::kBlockSize);

    // 解密
    des_->Decrypt(&in, &out);
    data_size = des::kBlockSize;

    // 处理结尾填充: ??22    ?????55555
    if (_is_end && _in_size - i <= des::kBlockSize) {
      data_size = des::kBlockSize - out[7];

      // ????????88888888
      if (0 == data_size) { break; }
      else if (data_size < 0) {
      }
    }

    memcpy(_out_data + write_size, &out, data_size);
    write_size += data_size;
  }
  return write_size;
}

/**
 * @brief 获取需要填充的字节数
 * @param _data_size
 * @return 需要填充的字节数
 */
inline size_t Crypt::GetMaxPaddingSize(size_t _data_size) {
  size_t padding_num = des::kBlockSize - _data_size % des::kBlockSize;  // 填充数量，同时也是填充的内容，如果是 8 就填充 8
  if (0 == padding_num) {
    padding_num = des::kBlockSize;
  }
  return padding_num;
}

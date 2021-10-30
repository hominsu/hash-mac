//
// Created by Homin Su on 2021/10/30.
//

#ifndef DES_CRYPT_CRYPT_H_
#define DES_CRYPT_CRYPT_H_

#include <memory>

namespace des {
class Des;
} // namespace des

class Crypt {
 private:
  std::shared_ptr<des::Des> des_;

 public:
  Crypt();
  ~Crypt();

  /**
   * @brief 初始化密钥
   * @param _password 8 字节密钥
   */
  void Init(const std::string &_password);

  /**
   * @brief 加密数据，加密到结尾填充数据
   * @detail 加密到结尾会使用 PKCS7 Padding 数据填充算法进行填充
   * @param _in_data 输入数据
   * @param _in_size 输入数据大小
   * @param _out_data 输出数据
   * @param _is_end 是否加密到结尾
   * @return 返回加密后的数据，有可能大于输入（结尾处）
   */
  size_t Encrypt(const char *_in_data, size_t _in_size, char *_out_data, bool _is_end = false);

  /**
   * @brief 解密数据，解密到结尾会删除填充数据
   * @param _in_data 输入数据
   * @param _in_size 输入数据大小
   * @param _out_data 输出数据
   * @param _is_end 是否加密到结尾
   * @return 返回解密后的数据，有可能小雨输入（结尾处）
   */
  size_t Decrypt(const char *_in_data, size_t _in_size, char *_out_data, bool _is_end = false);

  /**
   * @brief 获取需要填充的字节数
   * @param _data_size
   * @return 需要填充的字节数
   */
  static size_t GetMaxPaddingSize(size_t _data_size);
};

#endif //DES_CRYPT_CRYPT_H_

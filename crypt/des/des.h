//
// Created by Homin Su on 2021/10/28.
//

#ifndef DES_CRYPT_DES_DES_H_
#define DES_CRYPT_DES_DES_H_

#include <cstddef>
#include <cstdint>

#include <array>
#include <string>

/**
 * @brief DES 加密算法
 * @example
 * @verbatim
 *  auto sub_key = des::Init(_argv[1]);
 *
 *  char plain_text[8];
 *  char cipher_text[8];
 *  des::Encrypt(plain_text, cipher_text, sub_key);
 *
 *  char decrypt_tet[8];
 *  des::Decrypt(cipher_text, decrypt_tet, sub_key);
 * @endverbatim
 */
namespace des {
constexpr size_t kBlockSize = 8;  // 块大小

/**
 * @brief 二进制转十进制
 * @tparam Te 经过 e 表扩展的数据数组
 * @tparam Args 要转化的数据索引
 * @param _e 经过 e 表扩展的数据
 * @param args 要转化的数据索引, 由高到地
 * @return 十进制
 */
template<typename Te, typename ... Args>
inline unsigned char ExpendBin2Dec(Te &&_e, Args ... args) {
  unsigned char BCD_8421 = 0;
  return (0 + ... + ((_e >> args) & 0x1 * (BCD_8421 > 1 ? BCD_8421 *= 2 : ++BCD_8421)));
}

/**
 * @brief 将 8 个字节转换为一个 64 位的 uint64_t
 * @param c 字符数组指针
 * @return uint64_t
 */
uint64_t CharToByte(const char c[8]);

/**
 * @brief 将一个 28 位的子密钥左移
 * @param _k 子密钥
 * @param _shift_num 左移位数
 * @return 左移后的密钥
 */
uint32_t KeyLeftShift(uint32_t &_k, const unsigned char &_shift_num);

/**
 * @brief 初始化密钥, 生成 16 个 48 位的子密钥
 * @details 密钥 8 位，多余丢弃，不足补 0
 * @param _password std::array<uint64_t, 16>
 */
std::array<uint64_t, 16> Init(const std::string &_password);

/**
 * @brief 轮函数
 * @param _r 上一轮右 32 位
 * @param _k 48 位子密钥
 * @return 加密后的 32 位数据
 */
uint32_t RoundFunc(const uint32_t &_r, const uint64_t &_k);

/**
 * @brief 加解密, 单次加密 8 个字节
 * @param _in 输入数据
 * @param _out 输出数据
 * @param _sub_key 十六轮子密钥
 * @param _is_encrypt 加密解密
 */
void Crypt(const void *_in, void *_out, std::array<uint64_t, 16> &_sub_key, bool _is_encrypt);

/**
 * @brief 加密, 单次加密 8 个字节
 * @param _in 输入数据
 * @param _out 输出数据
 * @param _sub_key 十六轮子密钥
 */
inline void Encrypt(const void *_in, void *_out, std::array<uint64_t, 16> &_sub_key) {
  Crypt(_in, _out, _sub_key, true);
}

/**
 * @brief 解密, 单次解密 8 个字节
 * @param _in 输入数据
 * @param _out 输出数据
 * @param _sub_key 十六轮子密钥
 */
inline void Decrypt(const void *_in, void *_out, std::array<uint64_t, 16> &_sub_key) {
  Crypt(_in, _out, _sub_key, false);
}

} // namespace des

#endif //DES_CRYPT_DES_DES_H_

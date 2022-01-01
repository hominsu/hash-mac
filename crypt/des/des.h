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
 * @brief Implementation of Data Encryption Standard
 * @defgroup des
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
constexpr ::std::size_t kBlockSize = 8;  // 块大小

/**
 * @brief Unsigned Binary to Decimal
 * @details 这里使用了 c++17 的折叠表达式，编译期展开后为：0 + ((_e >> _index_1) & 0x1 * 1) + ((_e >> _index_2) & 0x1 * (BCD_8421 *= 2)）+ ((_e >> _index_3) & 0x1 * (BCD_8421 *= 2))···, 即：0 + ((_e >> _index_1) & 0x1 * 1) + ((_e >> _index_2) & 0x1 * 2）+ ((_e >> _index_3) & 0x1 * 4） ···
 * @tparam Te Array of data expanded by extended permutation
 * @tparam Args Index of the data to be transformed
 * @param _e Data extended by extended permutation
 * @param _index Index of the data to be transformed, from high to low
 * @return Decimal
 * @retval unsigned char
 * @callby uint32_t RoundFunc(const uint32_t &_r, const uint64_t &_k)
 */
template<typename Te, typename ... Index>
inline unsigned char ExpendBin2Dec(Te &&_e, Index ... _index) {
  // 这里使用了 c++17 的折叠表达式，编译期展开后为：0 + ((_e >> _index_1) & 0x1 * 1) +
  // ((_e >> _index_2) & 0x1 * (BCD_8421 *= 2)）+ ((_e >> _index_3) & 0x1 * (BCD_8421 *= 2)） ···
  // 即：0 + ((_e >> _index_1) & 0x1 * 1) + ((_e >> _index_2) & 0x1 * 2）+ ((_e >> _index_3) & 0x1 * 4） ···
  unsigned char BCD_8421 = 0;
  return (0 + ... + ((_e >> _index) & 0x1 * (BCD_8421 > 1 ? BCD_8421 *= 2 : ++BCD_8421)));
}

/**
 * @brief Convert 8 bytes to uint64_t(64 bits)
 * @details 使用 memcpy 将指针指向的数组的 8 个字节拷贝到一个 uint64_t 中
 * @param c Char array ptr
 * @return 64 bits data
 * @retval uint64_t
 * @callby ::std::array<uint64_t, 16> Init(const ::std::string &_password)
 * @callby void Crypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key, bool _is_encrypt)
 */
uint64_t CharToBits(const char c[8]);

/**
 * @brief Left circular shift a 28 bits sub key
 * @param _k sub key
 * @param _shift_num shift digital
 * @return Left shifted sub key
 * @retval uint32_t
 * @callby ::std::array<uint64_t, 16> Init(const ::std::string &_password)
 */
uint32_t KeyLeftShift(uint32_t &_k, const unsigned char &_shift_num);

/**
 * @brief Initial Key, Generate 16 48-bit sub-key
 * @details Initialize key, the key length is 8 bytes, excess is discarded and insufficient is 0
 * @param _password 8 bytes key
 * @return 16 wheels sub-keys
 * @retval ::std::array<uint64_t, 16>
 * @call inline uint64_t CharToBits(const char c[8])
 * @call inline uint32_t KeyLeftShift(uint32_t &_k, const unsigned char &_shift_num)
 * @callby DesCBC::Init(const ::std::string &_password) in ../des_encrypt_cbc.cc
 */
::std::array<uint64_t, 16> Init(const ::std::string &_password);

/**
 * @brief round function
 * @param _r Previous round function right 32 bits
 * @param _k 48 位子密钥
 * @return Encrypted 32-bit data
 * @retval uint32_t
 * @call template<typename Te, typename ... Args> inline unsigned char ExpendBin2Dec(Te &&_e, Args ... args)
 * @callby void Crypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key, bool _is_encrypt)
 */
uint32_t RoundFunc(const uint32_t &_r, const uint64_t &_k);

/**
 * @brief Encryption and decryption, single encryption and decryption of 8 bytes
 * @param _in Input Data
 * @param _out Output Data
 * @param _sub_key Sixteen Wheels sub-key
 * @param _is_encrypt Encryption/Decryption
 * @call inline uint64_t CharToBits(const char c[8])
 * @call uint32_t RoundFunc(const uint32_t &_r, const uint64_t &_k)
 * @callby inline void Encrypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key)
 * @callby inline void Decrypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key)
 */
void Crypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key, bool _is_encrypt);

/**
 * @brief Encryption, single encryption of 8 bytes
 * @param _in Input Data
 * @param _out Output Data
 * @param _sub_key Sixteen Wheels sub-key
 * @call void Crypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key, bool _is_encrypt)
 * @callby ::std::size_t DesCBC::Encrypt(const char *_in_data, ::std::size_t _in_size, char *_out_data, bool _is_end) in ../des_encrypt_cbc.cc
 */
inline void Encrypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key) {
  Crypt(_in, _out, _sub_key, true);
}

/**
 * @brief Decryption, single decryption of 8 bytes
 * @param _in Input Data
 * @param _out Output Data
 * @param _sub_key Sixteen Wheels sub-key
 * @call void Crypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key, bool _is_encrypt)
 * @callby ::std::size_t DesCBC::Decrypt(const char *_in_data, ::std::size_t _in_size, char *_out_data, bool _is_end) in ../des_encrypt_cbc.cc
 */
inline void Decrypt(const void *_in, void *_out, ::std::array<uint64_t, 16> &_sub_key) {
  Crypt(_in, _out, _sub_key, false);
}

} // namespace des

#endif //DES_CRYPT_DES_DES_H_

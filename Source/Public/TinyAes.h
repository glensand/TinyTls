/* Copyright (C) 2023 Gleb Bezborodov - All Rights Reserved
* You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/TinyTls
 */
#pragma once

struct evp_cipher_ctx_st;

namespace TinyTls
{
// A wrapper around openssl library, implements AES ciphering (5 rounds and hardcoded salt, 'cause i want this way)
class FAes final
{
public:
	/**
     * @brief Initialize internal structures
     * @param Key Just an any sequence you want, by this sequence cipher will create a 32byte sequence
     */
    explicit FAes(const char* Key);
    ~FAes();

	/**
     * @brief Encrypt the given sequence of data using recently generated key
     * @param Data Ptr to the buffer to be encrypted
     * @param Size Size of the buffer [bytes]
     * @return Pointer to the encrypted sequence, and the Size contains length of the sequence (in bytes)
     * NOTE! After the work done, you must delete encrypted sequence manually (delete[] seq)
     */
    unsigned char* Encrypt(const void* Data, int32& Size) const;

	/**
	* @brief Decrypt the given sequence of data using recently generated key
	* @param Data Ptr to the buffer to be decrypted
	* @param Size Size of the buffer [bytes]
	* @return Pointer to the encrypted sequence, and the Size contains length of the sequence (in bytes)
	* NOTE! After the work done, you must delete decrypted sequence manually (delete[] seq)
	*/
    unsigned char* Decrypt(const void* Data, int32& Size) const;
	
private:
    evp_cipher_ctx_st* DecoderContext{ nullptr };
    evp_cipher_ctx_st* EncoderContext{ nullptr };
};

}

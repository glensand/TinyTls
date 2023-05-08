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

class FAes final
{
public:
    explicit FAes(const char* Key);
    ~FAes();

    unsigned char* Encrypt(const void* Data, int32& Size) const;
    unsigned char* Decrypt(const void* Data, int32& Size) const;
	
private:
    evp_cipher_ctx_st* DecoderContext{ nullptr };
    evp_cipher_ctx_st* EncoderContext{ nullptr };
};

}

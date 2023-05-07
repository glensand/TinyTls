/* Copyright (C) 2023 Gleb Bezborodov - All Rights Reserved
* You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/TinyTls
 */

#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
THIRD_PARTY_INCLUDES_END
#undef UI
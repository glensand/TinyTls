/* Copyright (C) 2023 Gleb Bezborodov - All Rights Reserved
* You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/TinyTls
 */

#pragma once

namespace TinyTls
{

/**
* @brief Sort of interface for tls stream helps to implement pimple so as not to shine the OpenSSL stuff out
*/
class FStream
{
public:
	virtual ~FStream() = default;

	/**
	* @brief Tries to establish TLS connection with the server, using proved IP and Port,
	*	the client certificate is not required, the server's authority check is not implemented (but might be if needed).
	*	For Auth purposes you can use some sort of "hand-made" checks
	* @param Ip Specified ip address or name to be connected to. Might be an alias, in this case, it will be resolved 
	* @param Port Specified port the server is listening on
	* @return True if succeeded, false if any error occurred (see LogTinyTls for details)
	*/
	virtual bool Connect(const char* Ip, const char* Port) = 0;
	
	/**
	 * @brief Kills tcp and tls connection
	 */
	virtual void Disconnect() = 0;

	/**
	* @brief Sets the maximum amount of time the stream will be blocked on receive operation,
	* After the limit will be exceeded current operation will be interrupted 
	* @param TimeOut Max time limit in [msec]
	* @return True if succeeded, false if any error occurred (see LogTinyTls for details)
	*/
	virtual bool SetReceiveTimeout(int32 TimeOut) = 0;

	/**
	 * @brief Sets the maximum amount of time the stream will be blocked on send operation,
	 * After the limit will be exceeded current operation will be interrupted 
	 * @param TimeOut Max time limit in [msec]
	 * @return True if succeeded, false if any error occurred (see LogTinyTls for details)
	 */
	virtual bool SetSendTimeout(int32 TimeOut) = 0;

	/**
	 * @brief Tries to send a specified amount of data
	 * @param Data Ptr to the buffer to be sent
	 * @param Size Buffer size [bytes]
	 * @return True if succeeded, false if any error occurred (see LogTinyTls for details)
	 */
	virtual bool Send(const void* Data, int32 Size) const = 0;

	
	/**
	 * @brief Tries to receive a specified amount of data
	 * @param Data Buffer to write in
	 * @param Size Buffer size [bytes]
	 * @return Number of received bytes, -1 of any error occurred
	 */
	virtual int32 Receive(void* Data, int32 Size) const = 0;

	/**
	 * @brief Template versions of send/receive operations, intended to be used with primitive types only
	 */
	template<typename T>
	bool Send(const T& Value)
	{
		return Send(&Value, sizeof(T));
	}

	template<typename T>
	T Receive()
	{
		T Dummy;
		Receive(&Dummy, sizeof(T));
		return Dummy;
	}
};

/**
 * @brief The only entry point of the plugin
 * @return Pointer to the tls stream implementation, the ptr could not be null (if not OOM)
 */
TINYTLS_API FStream* CreateTlsStream();
	
}
